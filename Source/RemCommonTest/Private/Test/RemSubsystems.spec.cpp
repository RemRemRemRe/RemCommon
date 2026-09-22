// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Subsystems/RemAudioEngineSubsystem.inl"
#include "Subsystems/RemEngineSubsystem.inl"
#include "Subsystems/RemGameInstanceSubsystem.inl"
#include "Subsystems/RemLocalPlayerSubsystem.inl"
#include "Subsystems/RemWorldSubsystem.inl"
#include "Test/RemSubsystemTestTypes.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Object/RemObjectStatics.h"
#include "RemTestWorld.h"

#include <type_traits>
#include <utility>

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Common::Private::URemTestAbsentAudioEngineSubsystem;
using Rem::Common::Private::URemTestAbsentEngineSubsystem;
using Rem::Common::Private::URemTestAbsentGameInstanceSubsystem;
using Rem::Common::Private::URemTestAbsentLocalPlayerSubsystem;
using Rem::Common::Private::URemTestAbsentWorldSubsystem;
using Rem::Common::Private::URemTestAudioEngineSubsystem;
using Rem::Common::Private::URemTestEngineSubsystem;
using Rem::Common::Private::URemTestGameInstanceSubsystem;
using Rem::Common::Private::URemTestLocalPlayerSubsystem;
using Rem::Common::Private::URemTestWorldSubsystem;

/**
 * test fixture: an initialised game instance bound to the test world, built the way the engine's
 * FTestWorldWrapper builds its game world. The destructor shuts the game instance down so its subsystem
 * collection deinitializes before the world is torn down.
 */
class FRemTestGameInstance
{
public:
    explicit FRemTestGameInstance(UWorld* InWorld)
        : World(InWorld)
      , GameInstance(NewObject<UGameInstance>(GEngine))
    {
        if (FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(World))
        {
            WorldContext->OwningGameInstance = GameInstance;
        }
        World->SetGameInstance(GameInstance);
        GameInstance->Init();
    }

    ~FRemTestGameInstance()
    {
        GameInstance->Shutdown();
    }

    UE_NONCOPYABLE(FRemTestGameInstance)

    [[nodiscard]] UGameInstance* GetGameInstance() const noexcept { return GameInstance; }

    /** registers a local player; UGameInstance::Shutdown removes it and deinitializes its subsystems */
    ULocalPlayer* AddLocalPlayer() const
    {
        // ULocalPlayer is Within=Engine, so it is created on the engine, like UGameInstance::CreateLocalPlayer does
        auto* LocalPlayer = NewObject<ULocalPlayer>(GEngine);
        GameInstance->AddLocalPlayer(LocalPlayer, FPlatformUserId::CreateFromInternalId(0));
        return LocalPlayer;
    }

private:
    UWorld* World{};
    UGameInstance* GameInstance{};
};
}

DEFINE_SPEC(FRemSubsystemsTest, "Rem.Common.Subsystems",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemSubsystemsTest::Define()
{
    Describe(TEXT("Engine subsystem accessors"), [this]
    {
        It(TEXT("should instantiate the TryGet engine subsystem accessor"), [this]
        {
            // taking the address in an evaluated context is what instantiates the template body; a
            // static_assert or decltype alone would leave it uninstantiated
            constexpr auto Accessor = &Rem::Subsystem::TryGetEngineSubsystem<URemTestEngineSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<decltype(Rem::Subsystem::TryGetEngineSubsystem<URemTestEngineSubsystem>()),
                                         URemTestEngineSubsystem*>);
        });

        It(TEXT("should instantiate the Get engine subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::GetEngineSubsystem<URemTestEngineSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<decltype(Rem::Subsystem::GetEngineSubsystem<URemTestEngineSubsystem>()),
                                         Rem::TNotNull<URemTestEngineSubsystem*>>);
        });

        It(TEXT("should resolve the registered engine subsystem by TryGet"), [this]
        {
            const auto* Registered = GEngine->GetEngineSubsystem<URemTestEngineSubsystem>();
            TestNotNull(TEXT("engine collection has the test subsystem"), Registered);

            const auto* Resolved = Rem::Subsystem::TryGetEngineSubsystem<URemTestEngineSubsystem>();

            TestTrue(TEXT("resolved instance is the registered one"), Resolved == Registered);
        });

        It(TEXT("should return nullptr when the engine subsystem is not created"), [this]
        {
            // the collection skipped the type because ShouldCreateSubsystem returned false, so this is a miss
            // with a resolvable context: the nullable half must answer nullptr without reporting anything
            TestNull(TEXT("absent engine subsystem"),
                Rem::Subsystem::TryGetEngineSubsystem<URemTestAbsentEngineSubsystem>());
        });

        It(TEXT("should resolve the registered engine subsystem by Get"), [this]
        {
            const auto* Registered = GEngine->GetEngineSubsystem<URemTestEngineSubsystem>();
            TestNotNull(TEXT("engine collection has the test subsystem"), Registered);
            if (Registered == nullptr)
            {
                return;
            }

            const Rem::TNotNull<URemTestEngineSubsystem*> Resolved =
                Rem::Subsystem::GetEngineSubsystem<URemTestEngineSubsystem>();

            // &* recovers the raw pointer: TestNotNull cannot see through the wrapper
            TestTrue(TEXT("resolved instance is the registered one"), &*Resolved == Registered);
        });
    });

    Describe(TEXT("World subsystem accessors"), [this]
    {
        It(TEXT("should instantiate the TryGet world subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::TryGetWorldSubsystem<URemTestWorldSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::TryGetWorldSubsystem<URemTestWorldSubsystem>(std::declval<UObject*>())),
                URemTestWorldSubsystem*>);
        });

        It(TEXT("should instantiate the Get world subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::GetWorldSubsystem<URemTestWorldSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::GetWorldSubsystem<URemTestWorldSubsystem>(std::declval<UObject*>())),
                Rem::TNotNull<URemTestWorldSubsystem*>>);
        });

        It(TEXT("should resolve the registered world subsystem by TryGet"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            const auto* Registered = World->GetSubsystem<URemTestWorldSubsystem>();
            TestNotNull(TEXT("world collection has the test subsystem"), Registered);

            const auto* Resolved = Rem::Subsystem::TryGetWorldSubsystem<URemTestWorldSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), Resolved == Registered);
        });

        It(TEXT("should return nullptr when the world subsystem is not created"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            TestNull(TEXT("absent world subsystem"),
                Rem::Subsystem::TryGetWorldSubsystem<URemTestAbsentWorldSubsystem>(World));
        });

        It(TEXT("should resolve the registered world subsystem by Get"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            const auto* Registered = World->GetSubsystem<URemTestWorldSubsystem>();
            TestNotNull(TEXT("world collection has the test subsystem"), Registered);
            if (Registered == nullptr)
            {
                return;
            }

            const Rem::TNotNull<URemTestWorldSubsystem*> Resolved =
                Rem::Subsystem::GetWorldSubsystem<URemTestWorldSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), &*Resolved == Registered);
        });
    });

    Describe(TEXT("Game instance subsystem accessors"), [this]
    {
        It(TEXT("should instantiate the TryGet game-instance subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::TryGetGameInstanceSubsystem<URemTestGameInstanceSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::TryGetGameInstanceSubsystem<URemTestGameInstanceSubsystem>(
                    std::declval<UObject*>())),
                URemTestGameInstanceSubsystem*>);
        });

        It(TEXT("should instantiate the Get game-instance subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::GetGameInstanceSubsystem<URemTestGameInstanceSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::GetGameInstanceSubsystem<URemTestGameInstanceSubsystem>(
                    std::declval<UObject*>())),
                Rem::TNotNull<URemTestGameInstanceSubsystem*>>);
        });

        It(TEXT("should resolve the registered game-instance subsystem by TryGet"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};

            const auto* Registered = Fixture.GetGameInstance()->GetSubsystem<URemTestGameInstanceSubsystem>();
            TestNotNull(TEXT("game instance collection has the test subsystem"), Registered);

            const auto* Resolved = Rem::Subsystem::TryGetGameInstanceSubsystem<URemTestGameInstanceSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), Resolved == Registered);
        });

        It(TEXT("should return nullptr when the game-instance subsystem is not created"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};

            // the test world's game instance exists, so the context resolves; only the subsystem type is absent
            TestNull(TEXT("absent game-instance subsystem"),
                Rem::Subsystem::TryGetGameInstanceSubsystem<URemTestAbsentGameInstanceSubsystem>(World));
        });

        It(TEXT("should resolve the registered game-instance subsystem by Get"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};

            const auto* Registered = Fixture.GetGameInstance()->GetSubsystem<URemTestGameInstanceSubsystem>();
            TestNotNull(TEXT("game instance collection has the test subsystem"), Registered);
            if (Registered == nullptr)
            {
                return;
            }

            const Rem::TNotNull<URemTestGameInstanceSubsystem*> Resolved =
                Rem::Subsystem::GetGameInstanceSubsystem<URemTestGameInstanceSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), &*Resolved == Registered);
        });
    });

    Describe(TEXT("Local player subsystem accessors"), [this]
    {
        It(TEXT("should instantiate the TryGet local-player subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::TryGetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::TryGetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>(
                    std::declval<UObject*>())),
                URemTestLocalPlayerSubsystem*>);
        });

        It(TEXT("should instantiate the Get local-player subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::GetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::GetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>(
                    std::declval<UObject*>())),
                Rem::TNotNull<URemTestLocalPlayerSubsystem*>>);
        });

        It(TEXT("should resolve the registered local-player subsystem by TryGet"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};
            const auto* LocalPlayer = Fixture.AddLocalPlayer();
            TestNotNull(TEXT("local player registered"), LocalPlayer);

            const auto* Registered = LocalPlayer->GetSubsystem<URemTestLocalPlayerSubsystem>();
            TestNotNull(TEXT("local player collection has the test subsystem"), Registered);

            const auto* Resolved = Rem::Subsystem::TryGetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), Resolved == Registered);
        });

        It(TEXT("should return nullptr when the local-player subsystem is not created"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};
            const auto* LocalPlayer = Fixture.AddLocalPlayer();
            TestNotNull(TEXT("local player registered"), LocalPlayer);

            // the local player exists, so the context resolves; only the subsystem type is absent
            TestNull(TEXT("absent local-player subsystem"),
                Rem::Subsystem::TryGetLocalPlayerSubsystem<URemTestAbsentLocalPlayerSubsystem>(World));
        });

        It(TEXT("should resolve the registered local-player subsystem by Get"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            const FRemTestGameInstance Fixture{World};
            const auto* LocalPlayer = Fixture.AddLocalPlayer();
            TestNotNull(TEXT("local player registered"), LocalPlayer);

            const auto* Registered = LocalPlayer->GetSubsystem<URemTestLocalPlayerSubsystem>();
            TestNotNull(TEXT("local player collection has the test subsystem"), Registered);
            if (Registered == nullptr)
            {
                return;
            }

            const Rem::TNotNull<URemTestLocalPlayerSubsystem*> Resolved =
                Rem::Subsystem::GetLocalPlayerSubsystem<URemTestLocalPlayerSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), &*Resolved == Registered);
        });
    });

    Describe(TEXT("Audio engine subsystem accessors"), [this]
    {
        It(TEXT("should instantiate the TryGet audio-engine subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::TryGetAudioEngineSubsystem<URemTestAudioEngineSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::TryGetAudioEngineSubsystem<URemTestAudioEngineSubsystem>(
                    std::declval<UObject*>())),
                URemTestAudioEngineSubsystem*>);
        });

        It(TEXT("should instantiate the Get audio-engine subsystem accessor"), [this]
        {
            constexpr auto Accessor = &Rem::Subsystem::GetAudioEngineSubsystem<URemTestAudioEngineSubsystem>;

            TestTrue(TEXT("accessor instantiated"), Accessor != nullptr);
            static_assert(std::is_same_v<
                decltype(Rem::Subsystem::GetAudioEngineSubsystem<URemTestAudioEngineSubsystem>(
                    std::declval<UObject*>())),
                Rem::TNotNull<URemTestAudioEngineSubsystem*>>);
        });

        It(TEXT("should resolve the registered audio-engine subsystem by TryGet"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            // asserted first so a -nosound run fails loudly instead of using an invalid handle
            if (!TestTrue(TEXT("audio device available"), Rem::Object::GetAudioDevice(World).IsValid()))
            {
                return;
            }

            const FAudioDeviceHandle AudioDevice = Rem::Object::GetAudioDevice(World);
            const auto* Registered               = AudioDevice->GetSubsystem<URemTestAudioEngineSubsystem>();
            TestNotNull(TEXT("audio device collection has the test subsystem"), Registered);

            const auto* Resolved = Rem::Subsystem::TryGetAudioEngineSubsystem<URemTestAudioEngineSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), Resolved == Registered);
        });

        It(TEXT("should return nullptr when the audio-engine subsystem is not created"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            if (!TestTrue(TEXT("audio device available"), Rem::Object::GetAudioDevice(World).IsValid()))
            {
                return;
            }

            TestNull(TEXT("absent audio-engine subsystem"),
                Rem::Subsystem::TryGetAudioEngineSubsystem<URemTestAbsentAudioEngineSubsystem>(World));
        });

        It(TEXT("should resolve the registered audio-engine subsystem by Get"), [this]
        {
            const Rem::Test::FRemTestWorld World;

            if (!TestTrue(TEXT("audio device available"), Rem::Object::GetAudioDevice(World).IsValid()))
            {
                return;
            }

            const FAudioDeviceHandle AudioDevice = Rem::Object::GetAudioDevice(World);
            const auto* Registered               = AudioDevice->GetSubsystem<URemTestAudioEngineSubsystem>();
            TestNotNull(TEXT("audio device collection has the test subsystem"), Registered);
            if (Registered == nullptr)
            {
                return;
            }

            const Rem::TNotNull<URemTestAudioEngineSubsystem*> Resolved =
                Rem::Subsystem::GetAudioEngineSubsystem<URemTestAudioEngineSubsystem>(World);

            TestTrue(TEXT("resolved instance is the registered one"), &*Resolved == Registered);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
