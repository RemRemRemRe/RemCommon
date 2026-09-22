// Copyright Sixze, RemRemRemRe. 2025. All Rights Reserved.

#include "Macro/RemAlsMacros.h"

#include "CoreGlobals.h"
#include "Async/UniqueLock.h"
#include "Async/WordMutex.h"
#include "Containers/StaticArray.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "Misc/CoreDelegates.h"
#include "Templates/Function.h"

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace Rem::AlsEnsure
{
static bool CanExecute(std::atomic<uint8>& bExecuted, const FAlsEnsureInfo& EnsureInfo)
{
    static const auto* EnsureStateConsoleVariable{
        IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureState"))
    };
    check(EnsureStateConsoleVariable != nullptr)

    static const auto* EnsureAlwaysEnabledConsoleVariable{
        IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureAlwaysEnabled"))
    };
    check(EnsureAlwaysEnabledConsoleVariable != nullptr)

    const auto EnsureState{static_cast<uint8>(EnsureStateConsoleVariable->GetInt())};

    if ((bExecuted.load(std::memory_order_relaxed) == EnsureState &&
         (!EnsureInfo.bEnsureAlways || !EnsureAlwaysEnabledConsoleVariable->GetBool())) ||
        !FPlatformMisc::IsEnsureAllowed())
    {
        return false;
    }

    return bExecuted.exchange(EnsureState, std::memory_order_release) != EnsureState || EnsureInfo.bEnsureAlways;
}

static bool ExecuteInternal(const FAlsEnsureInfo& EnsureInfo, const UTF8CHAR* Message)
{
    // The ensure handler and the OnEnsureFailed delegate are TCHAR APIs, so bridge to FString here; everything
    // else in this path works on the UTF8 message directly.
    const FString MessageString{Message
                                    ? Message
                                    : UTF8TEXT("")
    };

    if (UNLIKELY(
        GetEnsureHandler() && GetEnsureHandler()(FEnsureHandlerArgs{.Expression = EnsureInfo.Expression, .Message = *
            MessageString})))
    {
        return false;
    }

    if (FPlatformTime::GetSecondsPerCycle() != 0.0f)
    {
        // The wording and the composition match the engine's ensure message (Misc/AssertionMacros.cpp:
        // "Ensure condition failed: <expr> [File:<file>] [Line: <line>] \n<message>\n"), minus the call stack:
        // generating one is exactly what this lightweight ensure exists to avoid. FUnrealEdMisc::OnEnsureFailed
        // consumes this string, and the engine's log parsers match its prefix, so both keep working. The delegate
        // takes the same text as a TCHAR copy of the UTF8 builder.
        TUtf8StringBuilder<512> EnsureBuilder{
            InPlace,
            "Ensure condition failed: ",
            EnsureInfo.Expression,
            " [File:",
            EnsureInfo.FilePath
                ? EnsureInfo.FilePath
                : "Unknown",
            "] [Line: ",
            EnsureInfo.LineNumber,
            "] \n",
            Message,
            "\n"
        };

        const FString EnsureMessage(EnsureBuilder.ToView());
        FCoreDelegates::OnEnsureFailed.Broadcast(EnsureInfo.Expression, EnsureInfo.FilePath,
            EnsureInfo.LineNumber, *MessageString, *EnsureMessage);

        static const auto* EnsuresAreErrorsConsoleVariable{
            IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsuresAreErrors"))
        };
        check(EnsuresAreErrorsConsoleVariable != nullptr)

#if !NO_LOGGING
        if (EnsuresAreErrorsConsoleVariable->GetBool())
        {
            UE_LOGF(LogOutputDevice, Error, "%s", *EnsureBuilder)
        }
        else
        {
            UE_LOGF(LogOutputDevice, Warning, "%s", *EnsureBuilder)
        }
#endif

        PrintScriptCallstack();
    }

    if (!FPlatformMisc::IsDebuggerPresent())
    {
        FPlatformMisc::PromptForRemoteDebugging(true);
        return false;
    }

    static const auto* EnsureBreakEnabledConsoleVariable{
        IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureBreakEnabled"))
    };
    check(EnsureBreakEnabledConsoleVariable != nullptr)

#if UE_BUILD_SHIPPING
    return EnsureBreakEnabledConsoleVariable->GetBool();
#else
    return !GIgnoreDebugger && EnsureBreakEnabledConsoleVariable->GetBool();
#endif
}

bool UE_COLD UE_DEBUG_SECTION Execute(std::atomic<uint8>& bExecuted, const FAlsEnsureInfo& EnsureInfo)
{
    return CanExecute(bExecuted, EnsureInfo) && ExecuteInternal(EnsureInfo, UTF8TEXT(""));
}

bool UE_COLD UE_DEBUG_SECTION ExecuteFormat(std::atomic<uint8>& bExecuted, const FAlsEnsureInfo& EnsureInfo,
    const UTF8CHAR* Format, ...)
{
    if (!CanExecute(bExecuted, EnsureInfo))
    {
        return false;
    }

    static UE::FWordMutex FormatMutex;
    static constexpr auto MessageSize{std::numeric_limits<uint16>::max()};
    static TStaticArray<UTF8CHAR, MessageSize> Message;

    UE::TUniqueLock Lock{FormatMutex};

    GET_TYPED_VARARGS(UTF8CHAR, Message.GetData(), MessageSize, FormattedMessageSize - 1, Format, Format);

    return ExecuteInternal(EnsureInfo, Message.GetData());
}
}

#endif
