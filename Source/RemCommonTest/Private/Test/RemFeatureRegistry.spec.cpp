// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Templates/RemFeatureRegistry.h"

#include "GameplayTag/RemTagEventSourceFeature.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interface/RemGetLifetimeBoundObjectPtrInterface.h"
#include "Interface/RemTagEventSourceInterface.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Templates/Function.h"

#include "RemCommonLog.h"
#include "RemNotNull.h"
#include "RemTestWorld.h"
#include "Test/RemTagEventSourceTestTypes.h"
#include "Test/RemTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

#include <concepts>
#include <string_view>
#include <type_traits>

namespace
{
using Rem::Common::Private::ARemTestNullGetterActor;
using Rem::Common::Private::ARemTestTagEventGetterActor;
using Rem::Common::Private::FRemTestCustomHandle;
using Rem::Common::Private::FRemTestCustomHandleFamily;
using Rem::Common::Private::FRemTestLifetimeBoundStruct;
using Rem::Common::Private::TRemTestCustomTypedHandle;
using Rem::Common::Private::URemTestBareTagEventWorker;
using Rem::Common::Private::URemTestTagEventWorker;

using Rem::Common::Private::MakeWorker;
using Rem::Common::Private::MakeWorkerContribution;

#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
using Rem::Common::Private::FRemEnsureHandlerScope;
#endif

#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING
using Rem::Common::Private::DuplicateOwnerReportPart;
using Rem::Common::Private::FRemLogCaptureScope;
#endif

using FTagEventSourceHandle       = Rem::GameplayTag::FTagEventSourceFeature::FHandle;
using FRequest                    = Rem::GameplayTag::FTagEventSourceFeature::FRequest;
using FTagEventSourceRegistration = Rem::GameplayTag::FTagEventSourceRegistration;

/** the test module's own family: the registration a case constructs and the request its contributions answer */
using FTestRegistration = Rem::TFeatureRegistration<FRemTestCustomHandleFamily>;
using FTestRequest      = FRemTestCustomHandleFamily::FRequest;

/** the contribution's provider shape, spelled once for the case that registers an unbound one */
using FProvide = TFunction<FTagEventSourceHandle(const FRequest&)>;

/** a contribution that answers nothing: a case uses it where the registration itself is the point */
FTagEventSourceRegistration MakeEmptyContribution(UClass* OwnerType)
{
    return FTagEventSourceRegistration{
        {
            OwnerType,
            [](const FRequest)
            {
                return FTagEventSourceHandle{};
            },
        }
    };
}
}

DEFINE_SPEC(FRemFeatureRegistryTest, "Rem.Common.FeatureRegistry",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemFeatureRegistryTest::Define()
{
    Describe(TEXT("TFeatureHandleTraits"), [this]
    {
        It(TEXT("should derive a family's typed handle through its own traits specialization"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            // The extension point's isolated half: a family supplies FHandle, TTypedHandle and the
            // derivation, and the trait hands back the family's own shape. The sibling case walks the same
            // specialization through the registry of a family the test module owns.
            const FRemTestCustomHandle Erased{Worker, Worker};

            const auto WorkerTyped =
                Rem::TFeatureHandleTraits<FRemTestCustomHandleFamily>::DeriveTypedHandle<
                    URemTestTagEventWorker>(Erased);
            const auto ActorTyped =
                Rem::TFeatureHandleTraits<FRemTestCustomHandleFamily>::DeriveTypedHandle<AActor>(Erased);

            static_assert(std::same_as<std::remove_cvref_t<decltype(WorkerTyped)>,
                                       TRemTestCustomTypedHandle<URemTestTagEventWorker>>,
                "the typed handle has to come from the family's traits specialization");

            TestTrue(TEXT("the erased handle is valid"), Erased.IsValid());
            TestTrue(TEXT("the erased handle points at the worker"),
                Erased.TryGet() == static_cast<IRemTagEventSourceInterface*>(Worker));
            TestTrue(TEXT("the typed handle narrows the erased liveness to the queried type"),
                WorkerTyped.GetObject() == Worker);
            TestTrue(TEXT("the typed handle keeps the worker"),
                WorkerTyped.TryGet() == static_cast<IRemTagEventSourceInterface*>(Worker));
            TestTrue(TEXT("naming a type the erased handle does not carry yields an invalid handle"),
                !ActorTyped.IsValid());
        });

        It(TEXT("should walk a family the test module declares and owns itself"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor        = World->SpawnActor<AActor>();
            auto* FirstWorker  = MakeWorker(Actor);
            auto* SecondWorker = MakeWorker(Actor);

            // the wildcard answers nothing: the walk has to continue past the invalid handle
            const FTestRegistration EmptyWildcard{
                {
                    UObject::StaticClass(),
                    [](const FTestRequest)
                    {
                        return FRemTestCustomHandle{};
                    },
                }
            };

            // the first valid answer, keyed to the worker's base type
            FTestRegistration FirstAnswer{
                {
                    URemTestBareTagEventWorker::StaticClass(),
                    [FirstWorker](const FTestRequest)
                    {
                        return FRemTestCustomHandle{FirstWorker, FirstWorker};
                    },
                }
            };

            // a later valid answer for the derived owner type
            const FTestRegistration SecondAnswer{
                {
                    URemTestTagEventWorker::StaticClass(),
                    [SecondWorker](const FTestRequest)
                    {
                        return FRemTestCustomHandle{SecondWorker, SecondWorker};
                    },
                }
            };

            static_assert(
                std::same_as<std::remove_cvref_t<decltype(FRemTestCustomHandleFamily::Instance.Provide(Actor))>,
                             FRemTestCustomHandle>,
                "the walk has to hand back the family's own handle shape");
            static_assert(std::same_as<std::remove_cvref_t<decltype(FRemTestCustomHandleFamily::Instance
                                           .ProvideTyped<URemTestTagEventWorker>(Actor))>,
                                       TRemTestCustomTypedHandle<URemTestTagEventWorker>>,
                "the typed walk has to go through the family's own traits specialization");

            const auto Unfiltered = FRemTestCustomHandleFamily::Instance.Provide(Actor);
            const auto BaseTyped = FRemTestCustomHandleFamily::Instance.ProvideTyped<URemTestBareTagEventWorker>(Actor);
            const auto DerivedTyped = FRemTestCustomHandleFamily::Instance.ProvideTyped<URemTestTagEventWorker>(Actor);

            TestTrue(TEXT("the unfiltered walk skips the invalid answer and takes the first valid one"),
                Unfiltered.TryGet() == static_cast<IRemTagEventSourceInterface*>(FirstWorker));
            TestTrue(TEXT("the typed walk hands back the worker its own key registered"),
                BaseTyped.GetObject() == FirstWorker);
            TestTrue(TEXT("the typed walk keeps the worker"),
                BaseTyped.TryGet() == static_cast<IRemTagEventSourceInterface*>(FirstWorker));
            TestTrue(TEXT("the owner-type filter skips the base-keyed answer and takes the derived one"),
                DerivedTyped.GetObject() == SecondWorker);
            // the filter is what decides here: only the wildcard entry survives an AActor filter and it answers
            // nothing, while the unfiltered walk above has the base-keyed entry's valid answer to hand back
            TestTrue(TEXT("a filtered query for a type no contribution answers for gets nothing"),
                !FRemTestCustomHandleFamily::Instance.Provide(Actor, AActor::StaticClass()).IsValid());
            TestTrue(TEXT("the typed form of the same query is empty too"),
                !FRemTestCustomHandleFamily::Instance.ProvideTyped<AActor>(Actor).IsValid());

            FirstAnswer.Reset();

            TestTrue(TEXT("the removal leaves the next registered answer"),
                FRemTestCustomHandleFamily::Instance.Provide(Actor).TryGet() ==
                static_cast<IRemTagEventSourceInterface*>(SecondWorker));
            TestTrue(TEXT("the removed key no longer answers its own type"),
                !FRemTestCustomHandleFamily::Instance.ProvideTyped<URemTestBareTagEventWorker>(Actor).IsValid());
        });
    });

    Describe(TEXT("GetTypeName"), [this]
    {
        It(TEXT("should extract the type's name without the enclosing function signature"), [this]
        {
            // The helper strips a compiler-specific prefix and suffix, so the constants have to name
            // the function the way its namespace qualifies it; nothing of the signature may survive.
            // Read at run time: the IDE's constant evaluator mis-models the compiler's signature macro.
            const std::string_view TypeName = Rem::GetTypeName<FRemTestLifetimeBoundStruct>();

            TestTrue(TEXT("the type's name is there"),
                TypeName.find("FRemTestLifetimeBoundStruct") != std::string_view::npos);
            TestTrue(TEXT("no function name"), TypeName.find("GetTypeName") == std::string_view::npos);
            TestTrue(TEXT("no calling convention"), TypeName.find("__cdecl") == std::string_view::npos);
            TestTrue(TEXT("no template brackets"), TypeName.find('<') == std::string_view::npos);
            TestTrue(TEXT("no parameter list"), TypeName.find("(void)") == std::string_view::npos);
        });
    });

#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE

    Describe(TEXT("Rem::GetLifetimeBoundObjectPtr"), [this]
    {
        It(TEXT("should report and yield an invalid handle when the named liveness is not the owner's type"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Owner = World->SpawnActor<AActor>();

            FRemTestLifetimeBoundStruct Struct{};
            Struct.Owner = Owner;

            int32 ReportCount{0};
            const FRemEnsureHandlerScope HandlerScope{[&ReportCount](const FEnsureHandlerArgs&)
                {
                    ++ReportCount;
                    return true; // the count is the assertion: skip the log and the broadcast
                }
            };

            // the struct's owner is an actor, so naming the worker type as the liveness is a mismatch:
            // every handle it would build is invalid
            const auto Handle = Rem::GetLifetimeBoundObjectPtr<IRemGetLifetimeBoundOwnerInterface,
                                                               URemTestTagEventWorker>(Struct);

            TestEqual(TEXT("one development-only report"), ReportCount, 1);
            TestTrue(TEXT("the mismatched handle is invalid"), !Handle.IsValid());
            TestNull(TEXT("no identity"), Handle.GetObject());
            TestNull(TEXT("no pointee"), Handle.TryGet());
        });
    });

    Describe(TEXT("Typed handle promise"), [this]
    {
        It(TEXT("should report when the contribution's worker is not the queried owner type"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor      = World->SpawnActor<AActor>();
            auto* BareWorker = NewObject<URemTestBareTagEventWorker>(Actor);

            // keyed for the identified worker type but handing out a bare worker: the typed query's
            // promise - the named owner type is the worker - does not hold
            const FTagEventSourceRegistration TypedRegistration{
                {
                    URemTestTagEventWorker::StaticClass(),
                    [BareWorker, Actor](const FRequest)
                    {
                        return FTagEventSourceHandle{
                            Actor,
                            Rem::MakeNotNull(static_cast<IRemTagEventSourceInterface*>(BareWorker))
                        };
                    },
                }
            };

            int32 ReportCount{0};
            const FRemEnsureHandlerScope HandlerScope{[&ReportCount](const FEnsureHandlerArgs&)
                {
                    ++ReportCount;
                    return true; // the count is the assertion: skip the log and the broadcast
                }
            };

            const auto TypedSource = Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor);

            TestEqual(TEXT("one development-only report"), ReportCount, 1);
            TestTrue(TEXT("the mismatch leaves an invalid handle"), !TypedSource.IsValid());
        });
    });

#endif // DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE

    Describe(TEXT("Contribution registration"), [this]
    {
        It(TEXT("should skip a contribution whose Provide is an unbound TFunction"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            // a contribution registered without a provider: the walk has to skip it, not call it
            const FTagEventSourceRegistration UnboundRegistration{
                {
                    URemTestTagEventWorker::StaticClass(),
                    FProvide{},
                }
            };
            const FTagEventSourceRegistration ProvidingRegistration{MakeWorkerContribution(Worker)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("the next contribution answers"), Source.IsValid());
            TestTrue(TEXT("it is the next contribution's worker"), Source.GetObject() == Worker);
        });

#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING

        It(TEXT("should report the second contribution for one owner type once and keep the first one"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* First  = MakeWorker(Actor);
            auto* Second = MakeWorker(Actor);

            const FTagEventSourceRegistration FirstRegistration{MakeWorkerContribution(First)};

            // the scope counts the mechanism's duplicate-owner report only, and it is created before the
            // registration that reports: the baseline assertion proves the count comes from this
            // registration and not from an earlier record
            const FRemLogCaptureScope LogScope{LogRemCommon.GetCategoryName(), DuplicateOwnerReportPart};
            TestEqual(TEXT("the scope counts nothing before the registration"), LogScope.CountReports(), 0);

            const FTagEventSourceRegistration SecondRegistration{MakeWorkerContribution(Second)};
            TestEqual(TEXT("the duplicate writes exactly one report"), LogScope.CountReports(), 1);

            // the control: a contribution for a free owner type reports nothing, so the count above is
            // the duplicate's report and not whatever else the scope may see
            const FTagEventSourceRegistration FreeKeyRegistration{
                {
                    URemTestBareTagEventWorker::StaticClass(),
                    [](const FRequest)
                    {
                        return FTagEventSourceHandle{};
                    },
                }
            };
            TestEqual(TEXT("a free owner type adds no report"), LogScope.CountReports(), 1);

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("the first registered contribution still answers"), Source.GetObject() == First);
        });

#endif // REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING

        It(TEXT("should reset idempotently and leave a default-constructed registration inert"), [this]
        {
            // The case is not development-only: the removal and the idempotence it checks exist in every
            // configuration. Only its two report counters need development-only machinery, so each is
            // guarded by the condition its behaviour needs.
            const Rem::Test::FRemTestWorld World;
            auto* Actor    = World->SpawnActor<AActor>();
            auto* Worker   = MakeWorker(Actor);
            auto* Survivor = MakeWorker(Actor);

#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
            int32 EnsureReportCount{0};
            const FRemEnsureHandlerScope HandlerScope{[&EnsureReportCount](const FEnsureHandlerArgs&)
                {
                    ++EnsureReportCount;
                    return true;
                }
            };
#endif

#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING
            const FRemLogCaptureScope LogScope{LogRemCommon.GetCategoryName(), DuplicateOwnerReportPart};
            TestEqual(TEXT("the scope counts nothing before the registrations"), LogScope.CountReports(), 0);

            // The instrument proves itself inside this case: two registrations for one free key make the
            // mechanism report once, so a count of one later is the count of a working filter rather than
            // of a filter that matches nothing. Both contributions answer nothing, so the walk skips them.
            const FTagEventSourceRegistration FirstDuplicate{
                MakeEmptyContribution(ARemTestNullGetterActor::StaticClass())
            };
            const FTagEventSourceRegistration SecondDuplicate{
                MakeEmptyContribution(ARemTestNullGetterActor::StaticClass())
            };
            TestEqual(TEXT("the duplicate pair reports once"), LogScope.CountReports(), 1);
#endif

            // The survivor answers for a test actor type that nothing else in the session registers: the
            // game module's contribution holds /Script/Engine.Actor for the whole session, so that key
            // would report a duplicate inside this scope. The unfiltered walk reaches the survivor only
            // because that earlier contribution declines an actor without an ability system and the walk
            // continues - the case does depend on that.
            const FTagEventSourceRegistration SurvivorRegistration{
                {
                    ARemTestTagEventGetterActor::StaticClass(),
                    [Survivor](const FRequest)
                    {
                        return Survivor->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
                    },
                }
            };

#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING
            TestEqual(TEXT("the survivor's registration adds no report"), LogScope.CountReports(), 1);
#endif

            {
                FTagEventSourceRegistration Resettable{
                    {
                        URemTestTagEventWorker::StaticClass(),
                        [Worker](const FRequest)
                        {
                            return Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
                        },
                    }
                };

                TestTrue(TEXT("the resettable registration answers its own key"),
                    Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor).GetObject() == Worker);
                TestTrue(TEXT("the survivor answers the unfiltered walk"),
                    Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).GetObject() == Survivor);

                Resettable.Reset();
                TestTrue(TEXT("the removed registration's key no longer answers"),
                    !Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor).IsValid());

                Resettable.Reset();
                TestTrue(TEXT("a second reset changes nothing"),
                    !Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor).IsValid());
                TestTrue(TEXT("the second reset left the survivor's entry alone"),
                    Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).GetObject() == Survivor);
            }

            TestTrue(TEXT("the destroyed registration's key still does not answer"),
                !Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor).IsValid());
            TestTrue(TEXT("the destroyed registration removed nothing else"),
                Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).GetObject() == Survivor);

            {
                // a default-constructed registration owns no entry: constructing and destroying one
                // reports nothing and removes nothing
                const FTagEventSourceRegistration Default;
            }

            TestTrue(TEXT("the default-constructed registration removed nothing"),
                Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).GetObject() == Survivor);

#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
            TestEqual(TEXT("no ensure report"), EnsureReportCount, 0);
#endif
#if REM_WITH_DEVELOPMENT_ONLY_CODE && !NO_LOGGING
            TestEqual(TEXT("nothing after the self-check adds a report"), LogScope.CountReports(), 1);
#endif
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
