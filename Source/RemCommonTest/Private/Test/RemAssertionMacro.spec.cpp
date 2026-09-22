// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Macro/RemAssertionMacros.h"
#include "Macro/RemEnsureMacro.h"

#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Misc/CoreDelegates.h"
#include "Misc/Paths.h"
#include "String/FormatStringSan.h"

#include "Test/RemTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
using Rem::Common::Private::FRemEnsureHandlerScope;
#endif

/** Collects what FCoreDelegates::OnEnsureFailed broadcasts while the scope is alive, and unbinds on exit. */
class FRemEnsureFailedBroadcastScope
{
public:
    struct FBroadcast
    {
        FString Expression;
        FString FilePath;
        int32 LineNumber{0};
        FString Message;
        FString CombinedErrorMsg;
    };

    FRemEnsureFailedBroadcastScope()
    {
        Handle = FCoreDelegates::OnEnsureFailed.AddLambda(
            [this](const ANSICHAR* Expression, const ANSICHAR* FilePath, const int32 LineNumber,
            const TCHAR* Message, const TCHAR* CombinedErrorMsg)
            {
                Broadcasts.Add(FBroadcast{
                    .Expression = Expression != nullptr
                                      ? Expression
                                      : "",
                    .FilePath = FilePath != nullptr
                                    ? FilePath
                                    : "",
                    .LineNumber = LineNumber,
                    .Message    = Message != nullptr
                                   ? Message
                                   : TEXT(""),
                    .CombinedErrorMsg = CombinedErrorMsg != nullptr
                                            ? CombinedErrorMsg
                                            : TEXT("")
                });
            });
    }

    FRemEnsureFailedBroadcastScope(const FRemEnsureFailedBroadcastScope&)            = delete;
    FRemEnsureFailedBroadcastScope& operator=(const FRemEnsureFailedBroadcastScope&) = delete;

    ~FRemEnsureFailedBroadcastScope()
    {
        FCoreDelegates::OnEnsureFailed.Remove(Handle);
    }

    const TArray<FBroadcast>& GetBroadcasts() const
    {
        return Broadcasts;
    }

private:
    TArray<FBroadcast> Broadcasts;
    FDelegateHandle Handle;
};
}

DEFINE_SPEC(FRemAssertionMacroTest, "Rem.Common.Assertion",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemAssertionMacroTest::Define()
{
#if DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
    // Expected-error bookkeeping of the cases below (FAutomationTestBase::IsExpectedMessage and
    // FAutomationExpectedMessage::Matches, Misc/AutomationTest.cpp):
    // - One failing report leaves TWO records in an editor run: the LogOutputDevice line, and the
    //   FMessageLog("EnsureFailed") mirror that FUnrealEdMisc::OnEnsureFailed posts when bShowEnsuresInMessageLog is
    //   set (MessageLogListingModel::AddMessageInternal mirrors a listing entry with the listing's own category).
    //   A game build has only the first, so the count is declared as occurrences 0, the framework's "one or more,
    //   no upper limit" form (-1 means silently ignored, a positive number means exactly that many).
    // - The expected-message list belongs to the whole spec test and Matches() stops at the first matching entry, so
    //   a record can credit at most one entry. Each case therefore declares exactly one pattern, carrying its own
    //   condition token, and no pattern can match another case's record whatever order the cases run in.
    Describe(TEXT("RemEnsureCondition"), [this]
    {
        It(TEXT(
                "should report a failed condition through the lightweight path once and still run the handling statement"),
            [this]
            {
                // This case's record: the guard's synthesized condition followed by the engine's file marker.
                AddExpectedErrorPlain(TEXT("bCase1Condition [File:"), EAutomationExpectedErrorFlags::Contains, 0);

                const FRemEnsureFailedBroadcastScope BroadcastScope;

                int32 HandlerCallCount{0};
                const FRemEnsureHandlerScope HandlerScope{[&HandlerCallCount](const FEnsureHandlerArgs&)
                    {
                        ++HandlerCallCount;
                        return false; // continue to the log and the delegate, so the case covers the whole report
                    }
                };

                const bool bCase1Condition{false};
                int32 HandledCallCount{0};
                RemEnsureCondition(bCase1Condition, ++HandledCallCount;);

                TestEqual(TEXT("the handling statement ran once"), HandledCallCount, 1);
                TestEqual(TEXT("the ensure handler was called once"), HandlerCallCount, 1);

                const auto& Broadcasts = BroadcastScope.GetBroadcasts();
                TestEqual(TEXT("the delegate was broadcast once"), Broadcasts.Num(), 1);
                if (Broadcasts.Num() == 1)
                {
                    const auto& Broadcast = Broadcasts[0];
                    TestTrue(TEXT("the report has no caller message"), Broadcast.Message.IsEmpty());
                    // The combined message is what FUnrealEdMisc::OnEnsureFailed consumes, and the absent call stack is
                    // what keeps this case tied to the lightweight path: the engine's ensureAlways broadcasts one.
                    TestTrue(TEXT("the combined message starts with the engine prefix"),
                        Broadcast.CombinedErrorMsg.StartsWith(TEXT("Ensure condition failed: ")));
                    TestFalse(TEXT("the lightweight path adds no call stack"),
                        Broadcast.CombinedErrorMsg.Contains(TEXT("Stack: ")));
                }
            });

        It(TEXT("should run the handling statement of a REM_NO_ASSERTION form without reporting"), [this]
        {
            int32 HandlerCallCount{0};
            const FRemEnsureHandlerScope HandlerScope{[&HandlerCallCount](const FEnsureHandlerArgs&)
                {
                    ++HandlerCallCount;
                    return true;
                }
            };

            int32 HandledCallCount{0};
            RemEnsureCondition(REM_NO_ASSERTION, false, ++HandledCallCount;);

            TestEqual(TEXT("the handling statement ran once"), HandledCallCount, 1);
            TestEqual(TEXT("nothing was reported to the ensure handler"), HandlerCallCount, 0);
        });
    });

    Describe(TEXT("REM_ENSURE and REM_ENSURE_ALWAYS"), [this]
    {
        It(TEXT("should report every REM_ENSURE_ALWAYS failure and only the first failure of a REM_ENSURE call site"), [
            this]
        {
            int32 HandlerCallCount{0};
            const FRemEnsureHandlerScope HandlerScope{[&HandlerCallCount](const FEnsureHandlerArgs&)
                {
                    ++HandlerCallCount;
                    return true; // stop after the handler: this case needs the report count, not the log
                }
            };

            const auto ReportAlways = [] { REM_ENSURE_ALWAYS(false); };
            ReportAlways();
            ReportAlways();

            TestEqual(TEXT("REM_ENSURE_ALWAYS reports every failure"), HandlerCallCount, 2);

            // Both calls share this call site (file and line), so the once-per-session state is consumed by the first.
            const auto ReportOnce = [] { REM_ENSURE(false); };
            ReportOnce();
            ReportOnce();

            TestEqual(TEXT("REM_ENSURE reports only the first failure of a call site"), HandlerCallCount, 3);
        });
    });

    Describe(TEXT("REM_ENSURE_MESSAGE family"), [this]
    {
        It(TEXT("should format a UTF8 message and hand it to the ensure handler"), [this]
        {
            FString HandlerMessage;
            int32 HandlerCallCount{0};
            const FRemEnsureHandlerScope HandlerScope{
                [&HandlerMessage, &HandlerCallCount](const FEnsureHandlerArgs& Args)
                {
                    ++HandlerCallCount;
                    HandlerMessage = Args.Message;
                    return true;
                }
            };

            REM_ENSURE_MESSAGE(false, UTF8TEXT("id=%d"), 7);

            TestEqual(TEXT("the ensure handler was called once"), HandlerCallCount, 1);
            TestEqual(TEXT("the formatted message"), HandlerMessage, TEXT("id=7"));
        });

        It(TEXT("should round-trip a non-ASCII UTF8 argument back to the same text"), [this]
        {
            // Universal character names keep the spec text ASCII; U+89D2 U+8272 are two CJK characters.
            const FString SourceText{TEXT("\u89D2\u8272")};
            const FString ExpectedMessage = FString::Printf(TEXT("name=%s"), *SourceText);

            FString HandlerMessage;
            int32 HandlerCallCount{0};
            const FRemEnsureHandlerScope HandlerScope{
                [&HandlerMessage, &HandlerCallCount](const FEnsureHandlerArgs& Args)
                {
                    ++HandlerCallCount;
                    HandlerMessage = Args.Message;
                    return true;
                }
            };

            const bool bCase5Condition{false};
            REM_ENSURE_ALWAYS_MESSAGE(bCase5Condition, UTF8TEXT("name=%s"), StringCast<UTF8CHAR>(*SourceText).Get());

            TestEqual(TEXT("the ensure handler was called once"), HandlerCallCount, 1);
            TestEqualSensitive(TEXT("the decoded message"), HandlerMessage, ExpectedMessage);
        });
    });

    Describe(TEXT("FCoreDelegates::OnEnsureFailed"), [this]
    {
        It(TEXT("should broadcast the caller message with the expression, file and line in the combined error message"),
            [this]
            {
                // This case's record: the engine prefix with the caller's expression.
                AddExpectedErrorPlain(TEXT("Ensure condition failed: bCase6Condition"),
                    EAutomationExpectedErrorFlags::Contains, 0);

                const FRemEnsureFailedBroadcastScope BroadcastScope;

                const FRemEnsureHandlerScope HandlerScope{[](const FEnsureHandlerArgs&)
                    {
                        return false; // only the regular path broadcasts, so let it run
                    }
                };

                const bool bCase6Condition{false};
                REM_ENSURE_ALWAYS_MESSAGE(bCase6Condition, UTF8TEXT("probe=%d"), 7);

                const auto& Broadcasts = BroadcastScope.GetBroadcasts();
                TestEqual(TEXT("the delegate was broadcast once"), Broadcasts.Num(), 1);
                if (Broadcasts.Num() != 1)
                {
                    return;
                }

                const auto& Broadcast = Broadcasts[0];
                TestEqual(TEXT("the expression"), Broadcast.Expression, TEXT("bCase6Condition"));
                TestEqual(TEXT("the file"), FPaths::GetCleanFilename(Broadcast.FilePath),
                    TEXT("RemAssertionMacro.spec.cpp"));
                TestTrue(TEXT("the line number is set"), Broadcast.LineNumber > 0);
                TestEqual(TEXT("the caller message"), Broadcast.Message, TEXT("probe=7"));

                TestTrue(TEXT("the combined message carries the expression"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("bCase6Condition")));
                TestTrue(TEXT("the combined message carries the file"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("[File:")));
                TestTrue(TEXT("the combined message carries the line"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("[Line: ")));
                TestTrue(TEXT("the combined message carries the message"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("probe=7")));
                // The engine's layout, on the very string both records carry: the message sits on the line after the file
                // and line marker. The log-text expectation cannot check two properties of one record, so this replaces
                // the dropped second pattern.
                TestTrue(TEXT("the combined message carries the message after the file and line marker"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("] \nprobe=7")));
                TestFalse(TEXT("the lightweight path produces no call stack"),
                    Broadcast.CombinedErrorMsg.Contains(TEXT("Stack: ")));
            });
    });

    Describe(TEXT("UTF8 format validation"), [this]
    {
        It(TEXT("should reject a TCHAR pointer under %s in a UTF8 format at compile time"), [this]
        {
            using EFormatStatus = UE::Core::Private::FormatStringSan::EFormatStringSanStatus;

            // UE_CHECK_FORMAT_STRING_ERR is the compile-time status probe behind UE_VALIDATE_FORMAT_STRING, which is
            // the check that turns a TCHAR argument under a UTF8 %s into a compile error at the call sites.
            static_assert(UE_CHECK_FORMAT_STRING_ERR(EFormatStatus::Ok, UTF8TEXT("id=%d"), 7));
            static_assert(UE_CHECK_FORMAT_STRING_ERR(EFormatStatus::SNeedsNarrowCharPtrArgButGotWide,
                UTF8TEXT("name=%s"), static_cast<const TCHAR*>(nullptr)));

            TestTrue(TEXT("the UTF8 format checks hold at compile time"), true);
        });
    });
#endif // DO_ENSURE && REM_USE_LIGHT_WEIGHT_ENSURE
}

#endif // WITH_DEV_AUTOMATION_TESTS
