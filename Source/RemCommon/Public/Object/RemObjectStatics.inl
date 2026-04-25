// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemAssertionMacros.h"
#include "RemNotNull.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Enum/RemHelperEnum.h"
#include "Enum/RemHelperEnumAlias.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "RemConcepts.h"

namespace Rem::Object
{

template <CUGameInstance T = UGameInstance>
T* GetGameInstance(const TNotNull<const UObject*> Object)
{
    auto* World = Object->GetWorld();
    RemCheckVariable(World, return nullptr);

    return World->GetGameInstance<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetGameInstance)

template <CAGameStateBase T = AGameStateBase>
T* GetGameState(const TNotNull<const UObject*> Object)
{
    auto* World = Object->GetWorld();
    RemCheckVariable(World, return nullptr);

    return World->GetGameState<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetGameState)


template <CAPlayerController T = APlayerController>
T* GetControllerFromPawnOwner(const TNotNull<const UActorComponent*> Component)
{
    auto* Pawn = Component->GetOwner<APawn>();
    RemEnsureVariable(Pawn, return {});

    return Pawn->GetController<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetPlayerStateFromPawnOwner)

template <CAPlayerState TPlayerState = APlayerState,
    Enum::ERecursive ShouldRecursive = Enum::ERecursive{Enum::EYesOrNo::No},
    CAActor TActorOfNotNull = AActor>
TPlayerState* GetPlayerState(const TNotNull<TActorOfNotNull*> Actor)
{
    TPlayerState* Result{nullptr};

    if constexpr (std::derived_from<TActorOfNotNull, APawn> || std::derived_from<TActorOfNotNull, APlayerController>)
    {
        Result = Actor->template GetPlayerState<TPlayerState>();
    }
    else if constexpr (std::derived_from<TActorOfNotNull, APlayerState>)
    {
        Result = Cast<TPlayerState>(Actor);
    }
    else if constexpr (std::derived_from<TActorOfNotNull, AActor>)
    {
        if (const auto* Pawn = Cast<APawn>(Actor))
        {
            Result = Pawn->template GetPlayerState<TPlayerState>();
        }
        else if (const auto* PlayerController = Cast<APlayerController>(Actor))
        {
            Result = PlayerController->template GetPlayerState<TPlayerState>();
        }
        else if (const TPlayerState* PlayerState = Cast<TPlayerState>(Actor))
        {
            Result = const_cast<TPlayerState*>(PlayerState);
        }
    }
    else
    {
        static_assert(always_false<TActorOfNotNull>::value, "type is not supported");
    }

    if constexpr (ShouldRecursive == Enum::EYesOrNo::Yes)
    {
        if (!Result)
        {
            // try to find player state from owner chain
            if (const auto* Owner = Actor->GetOwner())
            {
                return GetPlayerState<AActor, TPlayerState, ShouldRecursive>(*Owner);
            }
        }
    }

    return Result;
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetPlayerState)

template <CAPlayerState TPlayerState = APlayerState,
    CAActor TActorOfNotNull = AActor>
TPlayerState* GetPlayerStateRecursive(const TNotNull<TActorOfNotNull*> Actor)
{
    return GetPlayerState<TActorOfNotNull, TPlayerState, Enum::ERecursive{Enum::EYesOrNo::Yes}>(Actor);
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetPlayerStateRecursive)

template <CAPlayerController T = APlayerController>
T* GetFirstLocalPlayerController(const TNotNull<const UObject*> WorldContextObject)
{
    // ReSharper disable once CommentTypo
    // https://wizardcell.com/unreal/multiplayer-tips-and-tricks/#2-beware-of-getplayerxxx0-static-functions

    const auto* GameInstance = GetGameInstance(WorldContextObject);
    RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return Cast<T>(GameInstance->GetFirstLocalPlayerController());
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalPlayerController)

template <CAHUD T = AHUD>
T* GetFirstLocalHUD(const TNotNull<const UObject*> WorldContextObject)
{
    const auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return PlayerController->GetHUD<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalHUD)

template <CULocalPlayer T = ULocalPlayer>
T* GetFirstLocalPlayer(const TNotNull<const UObject*> WorldContextObject)
{
    const auto* GameInstance = GetGameInstance(WorldContextObject);
    RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return Cast<T>(GameInstance->GetFirstGamePlayer());
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalPlayer)

template <CAPawn T = APawn>
T* GetFirstLocalPlayerPawn(const TNotNull<const UObject*> WorldContextObject)
{
    auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return PlayerController->GetPawn<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalPlayerPawn)

template <CAPlayerState T = APlayerState>
T* GetFirstLocalPlayerState(const TNotNull<const UObject*> WorldContextObject)
{
    auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return PlayerController->GetPlayerState<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalPlayerState)

template <CAPlayerCameraManager T = APlayerCameraManager>
T* GetFirstLocalPlayerCameraManager(const TNotNull<const UObject*> WorldContextObject)
{
    auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

    return Cast<T>(PlayerController->PlayerCameraManager);
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetFirstLocalPlayerCameraManager)

template <CUObject T>
[[nodiscard]] uint32 GetHashForObjects(const TConstArrayView<T*> Objects)
{
    uint32 HashResult{GetTypeHash(Objects.Num())};

    for (auto* Object : Objects)
    {
        HashResult = HashCombineFast(HashResult, GetTypeHash(FWeakObjectPtr{Object}));
    }

    return HashResult;
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetHashForObjects)

template <CUMovementComponent T = UMovementComponent, CAActor TActorOfNotNull = AActor>
T* FindMovementComponent(const TNotNull<TActorOfNotNull*> Actor)
{
    if (auto* Pawn = Cast<APawn>(Actor))
    {
        return Cast<T>(Pawn->GetMovementComponent());
    }

    return Actor->template FindComponentByClass<T>();
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(FindMovementComponent)

template <CAActor TActor = AActor,
    Enum::ECallFinishSpawn CallFinishSpawn = Enum::ECallFinishSpawn{Enum::EYesOrNo::Yes}>
[[nodiscard]] TActor* SpawnActor(const TNotNull<UWorld*> World, const TSubclassOf<TActor> Class,
    const FTransform& SpawnTransform,
    const ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::Undefined,
    AActor* OwnerActor                                               = nullptr,
    APawn* Instigator                                                = nullptr,
    const ESpawnActorScaleMethod ScaleMethod                         = ESpawnActorScaleMethod::MultiplyWithRoot
)
{
    auto* Actor = World->SpawnActorDeferred<TActor>(Class, SpawnTransform, OwnerActor, Instigator,
        CollisionHandlingMethod, ScaleMethod);

    if constexpr (CallFinishSpawn == Enum::EYesOrNo::Yes)
    {
        RemCheckVariable(Actor, return nullptr;);
        Actor->FinishSpawning(SpawnTransform, false, nullptr, ScaleMethod);
    }

    return Actor;
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(SpawnActor)

template <CAActor TActor = AActor,
    Enum::ECallFinishSpawn CallFinishSpawn = Enum::ECallFinishSpawn{Enum::EYesOrNo::Yes}>
[[nodiscard]] TActor* SpawnActor(const TNotNull<AActor*> OwnerActor, const TSubclassOf<TActor> Class,
    const FTransform& SpawnTransform,
    const ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::Undefined,
    APawn* Instigator                                                = nullptr,
    const ESpawnActorScaleMethod ScaleMethod                         = ESpawnActorScaleMethod::MultiplyWithRoot
)
{
    auto* World = OwnerActor->GetWorld();
    RemCheckVariable(World, return nullptr;);

    return Object::SpawnActor<TActor, CallFinishSpawn>(World, Class, SpawnTransform, CollisionHandlingMethod,
        OwnerActor, Instigator, ScaleMethod);
}

template <CUObject TObject>
[[nodiscard]] TArray<TObject*>& ObjectPtrDecay(TArray<TObjectPtr<TObject>>& Array)
{
    auto& RawObjectArrayMutable = const_cast<TArray<TObject*>&>(::ObjectPtrDecay(Array));
    return RawObjectArrayMutable;
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(ObjectPtrDecay)

template <CUObject TObject>
[[nodiscard]] TArrayView<TObject*> MakeArrayView(TArray<TObjectPtr<TObject>>& Array)
{
    return Object::ObjectPtrDecay(Array);
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(MakeArrayView)

template <CUObject TReturnType,
    typename T>
    requires !CNotNull<T>
decltype(auto) GetOwner(const T& Object)
{
    using FReturnTypeOfGetOwner = decltype(Object.GetOwner());
    if constexpr (CHasGet<FReturnTypeOfGetOwner>)
    {
        return ::Cast<TReturnType>(Object.GetOwner().Get());
    }
    else
    {
        return ::Cast<TReturnType>(Object.GetOwner());
    }
}

template <CUObject TReturnType = UObject,
    typename TObjectOfNotNull = UObject>
decltype(auto) GetOwner(const TNotNull<TObjectOfNotNull*>& Object)
{
    return GetOwner<TReturnType>(*Object);
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetOwner)

template <CAActor TReturnType = AActor,
    CUObject TObjectOfNotNull = UObject>
auto GetActorFromObject(const TNotNull<TObjectOfNotNull*> Object)
{
    auto* Actor = Cast<TReturnType>(Object);
    if (!Actor)
    {
        if (auto* Component = Cast<UActorComponent>(Object))
        {
            Actor = Cast<TReturnType>(Component->GetOwner());
        }
    }

    return Actor;
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(GetActorFromObject)
}
