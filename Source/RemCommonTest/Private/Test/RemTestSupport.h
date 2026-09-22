// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include <atomic>
#include "CoreGlobals.h"
#include "GameFramework/Actor.h"
#include "HAL/CriticalSection.h"
#include "Logging/LogCategory.h"
#include "Logging/LogVerbosity.h"
#include "Misc/AssertionMacros.h"
#include "Misc/OutputDevice.h"
#include "Misc/OutputDeviceRedirector.h"
#include "Misc/ScopeLock.h"
#include "Templates/Function.h"

#include "GameplayTag/RemTagEventSourceFeature.h"
#include "Test/RemTagEventSourceTestTypes.h"

/**
 * Test support the RemCommon specs share: the tag event source factories and the two scopes that count
 * the mechanism's development-only reports (an ensure-handler scope and a log-capture scope). Each
 * scope is compiled only where the reports it counts can exist, so a spec that uses one has to guard
 * its cases the same way.
 */
namespace Rem::Common::Private
{
/** a contribution that applies to every actor and hands out @p Worker */
inline GameplayTag::FTagEventSourceFeature::FContribution MakeWorkerContribution(URemTestTagEventWorker* Worker)
{
    return {
        UObject::StaticClass(),
        [Worker](const GameplayTag::FTagEventSourceFeature::FRequest)
        {
            return Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
        }
    };
}

/** worker owned by the given actor, so it stays alive for the test scope */
inline URemTestTagEventWorker* MakeWorker(AActor* Owner)
{
    return NewObject<URemTestTagEventWorker>(Owner);
}

#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE

/**
 * Installs an ensure handler for the scope and restores the previous one on exit; the same shape as the
 * engine's FEnsureScope (Core/Private/Tests/EnsureScope.h), which a game module cannot reach. Returning
 * true from the handler skips the regular ensure handling, so a case that only counts reports stays out
 * of the log; returning false lets the lightweight path continue to the log and to
 * FCoreDelegates::OnEnsureFailed.
 * @note The behaviour exists only with DO_ENSURE on and the lightweight reporter in use, which is what a
 * case using this scope has to be guarded by.
 */
class FRemEnsureHandlerScope
{
public:
    explicit FRemEnsureHandlerScope(TFunction<bool(const FEnsureHandlerArgs& Args)> InHandler)
    {
        PreviousHandler = SetEnsureHandler(MoveTemp(InHandler));
    }

    FRemEnsureHandlerScope(const FRemEnsureHandlerScope&)            = delete;
    FRemEnsureHandlerScope& operator=(const FRemEnsureHandlerScope&) = delete;

    ~FRemEnsureHandlerScope()
    {
        SetEnsureHandler(MoveTemp(PreviousHandler));
    }

private:
    TFunction<bool(const FEnsureHandlerArgs& Args)> PreviousHandler;
};
#endif // DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE

#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING

/**
 * Counts the matching reports of one log category for as long as the scope is alive, so a case can
 * assert how often a development-only report was written.
 * @note The device is unbuffered on purpose: a buffered device is served by the primary logging thread,
 * which is not the test thread, so its records arrive after the assertion - the engine's own test device
 * returns true from CanBeUsedOnMultipleThreads for the same reason.
 * @note The behaviour exists only where the mechanism's development-only block is compiled and logging
 * is on, which is what a case using this scope has to be guarded by.
 */
class FRemLogCaptureScope
{
public:
    /**
     * @param InCategory the category whose records are counted
     * @param InMessagePart a substring every counted record has to carry, so an unrelated record of the
     *        same category cannot pollute the count
     */
    FRemLogCaptureScope(const FLogCategoryName& InCategory, const TCHAR* InMessagePart)
        : Category(InCategory)
      , MessagePart(InMessagePart)
    {
        // Adding a device replays the redirector's backlog into it, so recording is armed afterwards:
        // the scope counts what is written inside it, not what was already logged.
        GLog->AddOutputDevice(&Device);
        Device.Owner.store(this, std::memory_order_release);
    }

    FRemLogCaptureScope(const FRemLogCaptureScope&)            = delete;
    FRemLogCaptureScope& operator=(const FRemLogCaptureScope&) = delete;

    ~FRemLogCaptureScope()
    {
        Device.Owner.store(nullptr, std::memory_order_release);
        GLog->RemoveOutputDevice(&Device);
    }

    /** how many matching warnings or worse were written inside the scope */
    [[nodiscard]] int32 CountReports() const
    {
        UE::TScopeLock Lock(RecordsGuard);
        return Records.Num();
    }

private:
    /** the redirector hands every record to this device on the writing thread; the scope filters it */
    struct FCaptureDevice final : FOutputDevice
    {
        std::atomic<FRemLogCaptureScope*> Owner{};

        /** unbuffered: a buffered device is served by the primary logging thread, not by this one */
        virtual bool CanBeUsedOnMultipleThreads() const override
        {
            return true;
        }

        virtual void Serialize(const TCHAR* Message, const ELogVerbosity::Type Verbosity,
            const FName& InCategory) override
        {
            FRemLogCaptureScope* const Capturing = Owner.load(std::memory_order_acquire);
            if (Capturing == nullptr || Verbosity > ELogVerbosity::Warning || InCategory != Capturing->Category)
            {
                return;
            }

            FString Record{Message};
            if (!Capturing->MessagePart.IsEmpty() && !Record.Contains(Capturing->MessagePart))
            {
                return;
            }

            UE::TScopeLock Lock(Capturing->RecordsGuard);
            Capturing->Records.Emplace(MoveTemp(Record));
        }
    };

    mutable FCriticalSection RecordsGuard;
    TArray<FString> Records;
    FName Category;
    FString MessagePart;
    FCaptureDevice Device;
};

/** the mechanism's duplicate-owner report, as the log writes it: the cases count this one record */
inline constexpr auto DuplicateOwnerReportPart = TEXT("a second contribution registers for");
#endif // REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING
}
