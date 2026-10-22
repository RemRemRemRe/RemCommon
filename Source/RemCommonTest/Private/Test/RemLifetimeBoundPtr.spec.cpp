// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Interface/RemStringInterface.h"
#include "Templates/RemLifetimeBoundPtr.h"

#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "Templates/SharedPointer.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakInterfacePtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "RemInterfacesTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

#include <concepts>

namespace
{
using Rem::Common::Private::URemTestStringObject;

/** Plain (non-UObject) interface a test payload derives from. */
struct FRemTestLifetimePayloadInterface
{
    virtual ~FRemTestLifetimePayloadInterface() = default;

    [[nodiscard]] virtual int32 GetNumber() const = 0;
};

/** Plain (non-UObject) payload, driven by a TSharedPtr liveness. */
struct FRemTestLifetimePayload : FRemTestLifetimePayloadInterface
{
    int32 Number{};

    [[nodiscard]] virtual int32 GetNumber() const override
    {
        return Number;
    }
};

/** Unrelated plain struct, used as the pointee of a general-storage handle. */
struct FRemTestForeignPointee
{
};

/** Validity-only liveness that answers validity and nothing else: it has no payload, so it cannot drive a compact storage. */
struct FRemTestValidityOnly
{
    bool bValid{};

    [[nodiscard]] bool IsValid() const
    {
        return bValid;
    }
};

using FRemTestCompactObjectPtr = Rem::TLifetimeBoundPtr<TWeakObjectPtr<URemTestStringObject>, IRemStringInterface>;
using FRemTestErasedObjectPtr  = Rem::TLifetimeBoundPtr<TWeakObjectPtr<UObject>, IRemStringInterface>;
using FRemTestCompactSharedPtr = Rem::TLifetimeBoundPtr<
    TSharedPtr<FRemTestLifetimePayload>, FRemTestLifetimePayloadInterface>;
using FRemTestGeneralSharedPtr = Rem::TLifetimeBoundPtr<
    TSharedPtr<FRemTestLifetimePayload>, const FRemTestForeignPointee>;
using FRemTestWeakSharedPtr = Rem::TLifetimeBoundPtr<
    TWeakPtr<FRemTestLifetimePayload>, const FRemTestLifetimePayloadInterface>;
using FRemTestSelfPointeePtr = Rem::TLifetimeBoundPtr<TWeakObjectPtr<URemTestStringObject>, URemTestStringObject>;

using FRemTestValidityOnlyPtr = Rem::TLifetimeBoundPtr<FRemTestValidityOnly, const FRemTestForeignPointee>;
}

DEFINE_SPEC(FRemLifetimeBoundPtrTest, "Rem.Common.LifetimeBoundPtr",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemLifetimeBoundPtrTest::Define()
{
    Describe(TEXT("Storage"), [this]
    {
        It(TEXT("should derive the pointer of a pointee that is a base of the liveness payload"), [this]
        {
            static_assert(FRemTestCompactObjectPtr::bCompactStorage);
            static_assert(FRemTestCompactSharedPtr::bCompactStorage);

            static_assert(!FRemTestErasedObjectPtr::bCompactStorage);
            static_assert(!FRemTestGeneralSharedPtr::bCompactStorage);
            static_assert(!FRemTestWeakSharedPtr::bCompactStorage);
            static_assert(!FRemTestValidityOnlyPtr::bCompactStorage);

            static_assert(FRemTestSelfPointeePtr::bCompactStorage);

            TestTrue(TEXT("compact object pointer"), FRemTestCompactObjectPtr::bCompactStorage);
            TestFalse(TEXT("erased object pointer"), FRemTestErasedObjectPtr::bCompactStorage);
        });

        It(TEXT("should store only the liveness when compact and the liveness plus the pointer otherwise"), [this]
        {
            static_assert(sizeof(FRemTestCompactObjectPtr) == sizeof(TWeakObjectPtr<URemTestStringObject>));
            static_assert(sizeof(FRemTestCompactSharedPtr) == sizeof(TSharedPtr<FRemTestLifetimePayload>));
            static_assert(sizeof(FRemTestSelfPointeePtr) == sizeof(TWeakObjectPtr<URemTestStringObject>));

            static_assert(
                sizeof(FRemTestErasedObjectPtr) == sizeof(TWeakObjectPtr<UObject>) + sizeof(IRemStringInterface*));
            static_assert(
                sizeof(FRemTestGeneralSharedPtr) == sizeof(TSharedPtr<FRemTestLifetimePayload>) + sizeof(
                    FRemTestForeignPointee*));

            TestEqual(TEXT("compact object pointer"), sizeof(FRemTestCompactObjectPtr),
                sizeof(TWeakObjectPtr<URemTestStringObject>));
            TestEqual(TEXT("erased object pointer"), sizeof(FRemTestErasedObjectPtr),
                sizeof(TWeakObjectPtr<UObject>) + sizeof(IRemStringInterface*));
        });

        It(TEXT("should report the payload of the liveness"), [this]
        {
            static_assert(std::same_as<Rem::TLifetimePayload<TWeakObjectPtr<UObject>>, UObject>);
            static_assert(
                std::same_as<Rem::TLifetimePayload<TSharedPtr<FRemTestLifetimePayload>>, FRemTestLifetimePayload>);
            static_assert(std::same_as<Rem::TLifetimePayloadPointer<TWeakObjectPtr<UObject>>, UObject*>);
            static_assert(Rem::CHasLifetimePayload<TWeakObjectPtr<UObject>>);
            static_assert(!Rem::CHasLifetimePayload<FRemTestValidityOnly>);

            URemTestStringObject* Object = NewObject<URemTestStringObject>();
            const TWeakObjectPtr<URemTestStringObject> Weak{Object};

            TestTrue(TEXT("payload"), std::same_as<Rem::TLifetimePayload<TWeakObjectPtr<UObject>>, UObject>);
            TestTrue(TEXT("payload pointer"), Rem::GetLifetimePayloadPointer(Weak) == Object);
        });
    });

    Describe(TEXT("Default"), [this]
    {
        It(TEXT("should be invalid, unbound and equal to another unbound handle"), [this]
        {
            const FRemTestErasedObjectPtr Handle{};
            const FRemTestCompactObjectPtr CompactHandle{};

            TestFalse(TEXT("erased invalid"), Handle.IsValid());
            TestFalse(TEXT("compact invalid"), CompactHandle.IsValid());
            TestTrue(TEXT("erased has no pointer"), Handle.TryGet() == nullptr);
            TestTrue(TEXT("erased has no object"), Handle.GetObject() == nullptr);
            TestTrue(TEXT("compact has no pointer"), CompactHandle.TryGet() == nullptr);
            TestTrue(TEXT("compact has no object"), CompactHandle.GetObject() == nullptr);
            TestTrue(TEXT("unbound handles are equal"), Handle == FRemTestErasedObjectPtr{});
            TestFalse(TEXT("unbound handles are not distinct"), Handle != FRemTestErasedObjectPtr{});
        });

        It(TEXT("should stay invalid after a reset"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            FRemTestErasedObjectPtr Handle{TWeakObjectPtr<UObject>{Object},
                Rem::MakeNotNull(Cast<IRemStringInterface>(Object))
            };
            TestTrue(TEXT("bound"), Handle.IsValid());

            Handle.Reset();

            TestFalse(TEXT("invalid after reset"), Handle.IsValid());
            TestTrue(TEXT("no pointer after reset"), Handle.TryGet() == nullptr);
            TestTrue(TEXT("no object after reset"), Handle.GetObject() == nullptr);
        });
    });

    Describe(TEXT("Compact storage"), [this]
    {
        It(TEXT("should derive the interface pointer from the tracked object by a compile-time offset"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const FRemTestCompactObjectPtr Handle{TWeakObjectPtr<URemTestStringObject>{Object}};

            const auto* Expected = Cast<IRemStringInterface>(Object);
            const auto* Static   = static_cast<IRemStringInterface*>(Object);

            TestTrue(TEXT("valid"), Handle.IsValid());
            TestTrue(TEXT("object"), Handle.GetObject() == Object);
            TestTrue(TEXT("liveness"), Handle.GetLiveness() == TWeakObjectPtr<URemTestStringObject>{Object});
            TestTrue(TEXT("derived pointer"), Handle.TryGet() == Expected);
            TestTrue(TEXT("static_cast matches the engine interface lookup"), Static == Expected);
            TestTrue(TEXT("the interface subobject is not at offset zero"),
                static_cast<const void*>(Object) != static_cast<const void*>(Expected));
            TestTrue(TEXT("checked pointer"), static_cast<IRemStringInterface*>(Handle.Get()) == Expected);
            TestEqual(TEXT("member access"), Handle->ToString(), FString{TEXT("RemTestStringObject")});
            TestTrue(TEXT("matches TWeakInterfacePtr"),
                Handle.TryGet() == TWeakInterfacePtr<IRemStringInterface>{Object}.Get());
        });

        It(TEXT("should invalidate when the tracked object dies, without touching the derived pointer"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const FRemTestCompactObjectPtr Handle{TWeakObjectPtr<URemTestStringObject>{Object}};
            TestTrue(TEXT("valid before destroy"), Handle.IsValid());

            Object->MarkAsGarbage();
            CollectGarbage(RF_NoFlags, true);

            TestFalse(TEXT("invalid after destroy"), Handle.IsValid());
            TestTrue(TEXT("no pointer after destroy"), Handle.TryGet() == nullptr);
            TestTrue(TEXT("no object after destroy"), Handle.GetObject() == nullptr);
        });

        It(TEXT("should treat the liveness payload itself as the pointee"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const FRemTestSelfPointeePtr Handle{TWeakObjectPtr<URemTestStringObject>{Object}};

            TestTrue(TEXT("valid"), Handle.IsValid());
            TestTrue(TEXT("same pointer"), Handle.TryGet() == Object);
            TestTrue(TEXT("same object"), Handle.GetObject() == Object);
        });
    });

    Describe(TEXT("General storage"), [this]
    {
        It(TEXT("should expose the stored pointer while the tracked object lives"), [this]
        {
            URemTestStringObject* Object   = NewObject<URemTestStringObject>();
            IRemStringInterface* Interface = Cast<IRemStringInterface>(Object);

            const FRemTestErasedObjectPtr Handle{TWeakObjectPtr<UObject>{Object}, Rem::MakeNotNull(Interface)};

            TestTrue(TEXT("valid"), Handle.IsValid());
            TestTrue(TEXT("object"), Handle.GetObject() == Object);
            TestTrue(TEXT("stored pointer"), Handle.TryGet() == Interface);
            TestTrue(TEXT("checked pointer"), static_cast<IRemStringInterface*>(Handle.Get()) == Interface);
            TestEqual(TEXT("member access"), Handle->ToString(), FString{TEXT("RemTestStringObject")});
        });

        It(TEXT("should erase the liveness type and keep the identity"), [this]
        {
            static_assert(std::convertible_to<TWeakObjectPtr<URemTestStringObject>, TWeakObjectPtr<UObject>>);
            static_assert(!std::same_as<TWeakObjectPtr<URemTestStringObject>, TWeakObjectPtr<UObject>>);

            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const FRemTestCompactObjectPtr Compact{TWeakObjectPtr<URemTestStringObject>{Object}};
            const FRemTestErasedObjectPtr Erased{Compact};

            TestFalse(TEXT("the erased handle stores its pointer"), FRemTestErasedObjectPtr::bCompactStorage);
            TestTrue(TEXT("valid"), Erased.IsValid());
            TestTrue(TEXT("same pointer"), Erased.TryGet() == Compact.Get());
            TestTrue(TEXT("same object"), Erased.GetObject() == Compact.GetObject());
        });

        It(TEXT("should carry an unbound compact handle over as unbound"), [this]
        {
            const FRemTestCompactObjectPtr Compact{};
            const FRemTestErasedObjectPtr Erased{Compact};

            TestFalse(TEXT("invalid"), Erased.IsValid());
            TestTrue(TEXT("no pointer"), Erased.TryGet() == nullptr);
            TestTrue(TEXT("no object"), Erased.GetObject() == nullptr);
        });

        It(TEXT("should stay bound to a dead liveness"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const FRemTestErasedObjectPtr Handle{TWeakObjectPtr<UObject>{Object},
                Rem::MakeNotNull(Cast<IRemStringInterface>(Object))
            };

            Object->MarkAsGarbage();
            CollectGarbage(RF_NoFlags, true);

            TestFalse(TEXT("invalid after destroy"), Handle.IsValid());
            TestTrue(TEXT("no pointer after destroy"), Handle.TryGet() == nullptr);
            TestTrue(TEXT("no object after destroy"), Handle.GetObject() == nullptr);
        });
    });

    Describe(TEXT("Non-UObject liveness"), [this]
    {
        It(TEXT("should derive an interface from a shared payload by a compile-time offset"), [this]
        {
            const TSharedPtr<FRemTestLifetimePayload> Shared = MakeShared<FRemTestLifetimePayload>();
            Shared->Number                                   = 7;

            const FRemTestCompactSharedPtr Handle{Shared};

            TestTrue(TEXT("valid"), Handle.IsValid());
            TestTrue(TEXT("object"), Handle.GetObject() == Shared.Get());
            TestTrue(TEXT("derived pointer"),
                Handle.TryGet() == static_cast<FRemTestLifetimePayloadInterface*>(Shared.Get()));
            TestEqual(TEXT("member access"), Handle->GetNumber(), 7);
        });

        It(TEXT("should keep a shared payload alive, because the handle holds a shared reference"), [this]
        {
            TSharedPtr<FRemTestLifetimePayload> Shared = MakeShared<FRemTestLifetimePayload>();
            FRemTestLifetimePayload* Payload           = Shared.Get();

            FRemTestCompactSharedPtr Handle{Shared};

            Shared.Reset();

            TestTrue(TEXT("a shared liveness is a strong reference"), Handle.IsValid());
            TestTrue(TEXT("same payload"), Handle.TryGet() == static_cast<FRemTestLifetimePayloadInterface*>(Payload));

            Handle.Reset();

            TestFalse(TEXT("invalid after reset"), Handle.IsValid());
            TestTrue(TEXT("no pointer after reset"), Handle.TryGet() == nullptr);
        });

        It(TEXT("should invalidate a weak liveness when the last shared owner is released"), [this]
        {
            TSharedPtr<FRemTestLifetimePayload> Shared        = MakeShared<FRemTestLifetimePayload>();
            const FRemTestLifetimePayloadInterface* Interface = Shared.Get();

            const FRemTestWeakSharedPtr Handle{TWeakPtr<FRemTestLifetimePayload>{Shared}, Rem::MakeNotNull(Interface)};
            TestTrue(TEXT("valid before release"), Handle.IsValid());

            Shared.Reset();

            TestFalse(TEXT("invalid after release"), Handle.IsValid());
            TestTrue(TEXT("no pointer after release"), Handle.TryGet() == nullptr);
        });

        It(TEXT("should store a pointer to an unrelated object"), [this]
        {
            const TSharedPtr<FRemTestLifetimePayload> Shared = MakeShared<FRemTestLifetimePayload>();
            constexpr FRemTestForeignPointee Foreign{};

            const FRemTestGeneralSharedPtr Handle{Shared, Rem::MakeNotNull(&Foreign)};

            TestTrue(TEXT("valid"), Handle.IsValid());
            TestTrue(TEXT("object"), Handle.GetObject() == Shared.Get());
            TestTrue(TEXT("stored pointer"), Handle.TryGet() == &Foreign);
        });
    });

    Describe(TEXT("Validity-only liveness"), [this]
    {
        It(TEXT("should judge validity through Rem::IsValid and keep the stored pointer"), [this]
        {
            constexpr FRemTestForeignPointee Foreign{};

            const FRemTestValidityOnlyPtr ValidHandle{FRemTestValidityOnly{.bValid = true}, Rem::MakeNotNull(&Foreign)};
            const FRemTestValidityOnlyPtr InvalidHandle{FRemTestValidityOnly{.bValid = false},
                Rem::MakeNotNull(&Foreign)
            };

            TestTrue(TEXT("valid"), ValidHandle.IsValid());
            TestTrue(TEXT("stored pointer"), ValidHandle.TryGet() == &Foreign);
            TestFalse(TEXT("invalid"), InvalidHandle.IsValid());
            TestTrue(TEXT("invalid keeps no pointer"), InvalidHandle.TryGet() == nullptr);
        });

        It(TEXT("should be reachable through Rem::IsValid and Rem::CHasIsValid"), [this]
        {
            const TSharedPtr<FRemTestLifetimePayload> Shared = MakeShared<FRemTestLifetimePayload>();
            const FRemTestCompactSharedPtr Handle{Shared};

            static_assert(Rem::CHasIsValid<FRemTestCompactSharedPtr>);

            TestTrue(TEXT("valid"), Rem::IsValid(Handle));

            const FRemTestCompactSharedPtr Unbound{};

            TestFalse(TEXT("invalid"), Rem::IsValid(Unbound));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
