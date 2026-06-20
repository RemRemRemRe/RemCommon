// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Object/RemObjectStatics.h"
#include "Object/RemObjectStatics.inl"

#include "TimerManager.h"
#include "Engine/TimerHandle.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Macro/RemAssertionMacros.h"
#include "AudioDeviceHandle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Math/RemMath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemObjectStatics)

UObject* URemObjectStatics::GetObject(const TSoftObjectPtr<>& SoftObjectPtr, UClass*)
{
    return SoftObjectPtr.Get();
}

bool URemObjectStatics::IsBlueprintObject(const UObject* Object)
{
    RemCheckVariable(Object, return false);

    return Object->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint)
           || !Object->GetClass()->HasAnyClassFlags(CLASS_Native);
}

UGameInstance* URemObjectStatics::GetGameInstance(const UObject* Object)
{
    RemCheckVariable(Object, return nullptr);

    return Rem::Object::GetGameInstance(Object);
}

FAudioDeviceHandle URemObjectStatics::GetAudioDevice(const UObject* Object)
{
    RemCheckVariable(Object, return {});

    return Rem::Object::GetAudioDevice(Object);
}

bool URemObjectStatics::IsObjectValidForBlueprint(const UObject* Object)
{
    RemCheckVariable(Object, return false);

    return !Object->HasAnyFlags(RF_BeginDestroyed) && !Object->IsUnreachable();
}

APlayerState* URemObjectStatics::GetPlayerState(const AActor* Actor)
{
    RemEnsureVariable(Actor, return {});

    return Rem::Object::GetPlayerState(Rem::MakeNotNull(Actor));
}

void URemObjectStatics::ShouldNotHappen(const bool bTriggerBreakpointInCpp)
{
    if (bTriggerBreakpointInCpp)
    {
        RemCheckCondition(false);
    }
}

APlayerController* URemObjectStatics::GetFirstLocalPlayerController(const UObject* WorldContextObject)
{
    RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
    return Rem::Object::GetFirstLocalPlayerController(WorldContextObject);
}

ULocalPlayer* URemObjectStatics::GetFirstLocalPlayer(const UObject* WorldContextObject)
{
    RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
    return Rem::Object::GetFirstLocalPlayer(WorldContextObject);
}

APawn* URemObjectStatics::GetFirstLocalPlayerPawn(const UObject* WorldContextObject)
{
    RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
    return Rem::Object::GetFirstLocalPlayerPawn(WorldContextObject);
}

APlayerState* URemObjectStatics::GetFirstLocalPlayerState(const UObject* WorldContextObject)
{
    RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
    return Rem::Object::GetFirstLocalPlayerState(WorldContextObject);
}

APlayerCameraManager* URemObjectStatics::GetFirstLocalPlayerCameraManager(const UObject* WorldContextObject)
{
    RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
    return Rem::Object::GetFirstLocalPlayerCameraManager(WorldContextObject);
}

void URemObjectStatics::ServerViewPreviousPlayer(const UObject* WorldContextObject)
{
    auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return;, REM_NO_LOG_BUT_ENSURE);

    // Dedicated server does not have "local" player controller
    RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

    PlayerController->ServerViewPrevPlayer();
}

void URemObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
    auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
    RemCheckVariable(PlayerController, return;, REM_NO_LOG_BUT_ENSURE);

    // Dedicated server does not have "local" player controller
    RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

    PlayerController->ServerViewNextPlayer();
}

bool URemObjectStatics::SetActorRootComponent(AActor* Actor, USceneComponent* NewRootComponent)
{
    RemCheckVariable(Actor, return false;, REM_NO_LOG_BUT_ENSURE);

    return Actor->SetRootComponent(NewRootComponent);
}


namespace Rem::Object
{

const FName RootBone{TEXTVIEW("root")};

FAudioDeviceHandle GetAudioDevice(const TNotNull<const UObject*> Object)
{
    const auto* World = Object->GetWorld();
    RemCheckVariable(World, return {});

    return World->GetAudioDevice();
}

void ForeachObjectInArray(const TNotNull<const FArrayProperty*> ArrayProperty,
    const TNotNull<const UObject*> InContainer,
    const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)>& Predicate)
{
    FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner);
    RemCheckVariable(ObjectProperty, return;);

    FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

    for (auto DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
    {
        uint8* PropertyValueAddress = Helper.GetRawPtr(DynamicIndex);

        Predicate(PropertyValueAddress, DynamicIndex);
    }
}

bool IsImplementedInBlueprint(const UFunction* Function)
{
    return Function && REM_ENSURE(Function->GetOuter())
           && Function->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
}

bool CheckPropertyChainByNames(const FEditPropertyChain& PropertyChain, const TArray<FName>& PropertyNamePath,
    const bool bShouldHaveNextNode)
{
    const auto* MemberNode = PropertyChain.GetActiveMemberNode();
    for (auto& Name : PropertyNamePath)
    {
        if (!MemberNode)
        {
            // different property path
            return false;
        }

        if (const auto* Value = MemberNode->GetValue();
            !Value || Value->GetFName() != Name)
        {
            // different property path
            return false;
        }

        MemberNode = MemberNode->GetNextNode();
    }

    if ((!bShouldHaveNextNode && MemberNode)
        || (bShouldHaveNextNode && !MemberNode))
    {
        return false;
    }

    return true;
}

FTimerHandle SetTimerForThisTick(const TNotNull<const UWorld*> World, const FTimerDelegate& Delegate)
{
    return World->GetTimerManager().SetTimerForNextTick([=]
    {
        Delegate.Execute();
    });
}

FTimerHandle SetTimerForThisTick(const TNotNull<const UObject*> WorldContextObject,
    const FTimerDelegate& Delegate)
{
    const auto* World = WorldContextObject->GetWorld();
    RemCheckVariable(World, return {});

    // MakeNotNull - fix ambiguous call
    return SetTimerForThisTick(MakeNotNull(World), Delegate);
}


FTimerHandle SetTimerForNextTick(const TNotNull<const UWorld*> World, const FTimerDelegate& Delegate)
{
    FTimerHandle Handle;
    World->GetTimerManager().SetTimer(Handle, [=]
    {
        Delegate.Execute();
    }, UE_KINDA_SMALL_NUMBER, {});

    return Handle;
}

FTimerHandle SetTimerForNextTick(const TNotNull<const UObject*> WorldContextObject,
    const FTimerDelegate& Delegate)
{
    const auto* World = WorldContextObject->GetWorld();
    RemCheckVariable(World, return {});

    // MakeNotNull - fix ambiguous call
    return SetTimerForNextTick(MakeNotNull(World), Delegate);
}

FVector GetActorFeetLocation(const TNotNull<const AActor*> Actor)
{
    const UNavMovementComponent* MovementComponent;

    if (auto* Pawn = Cast<APawn>(Actor))
    {
        MovementComponent = Pawn->GetMovementComponent();
        if (MovementComponent)
        {
            return MovementComponent->GetActorFeetLocation();
        }
    }

    MovementComponent = FindMovementComponent<UNavMovementComponent>(Actor);
    if (MovementComponent)
    {
        return MovementComponent->GetActorFeetLocation();
    }

    RemCheckCondition(Actor->GetRootComponent(), return FVector::ZeroVector, REM_NO_LOG_BUT_ENSURE);

    return Actor->GetRootComponent()->GetComponentTransform().TransformPosition(
        FVector{0.0f, 0.0f, Actor->GetRootComponent()->Bounds.BoxExtent.Z});
}

FTransform GetRootBoneTransform(const TNotNull<const ACharacter*> Character)
{
    auto* Mesh = Character->GetMesh();
    RemEnsureVariable(Mesh, return FTransform::Identity, REM_NO_LOG_BUT_ENSURE);

    return Mesh->GetSocketTransform(RootBone);
}

FVector GetRootBoneLocation(const TNotNull<const ACharacter*> Character)
{
    return GetRootBoneTransform(Character).GetLocation();
}

TOptional<FVector2f> GetPositionToMouseScreenSpace(const TNotNull<const APlayerController*> PlayerController,
    const FVector2f& ScreenPosition)
{
    FVector2f MousePosition;

    const auto bSuccess = PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
    RemEnsureCondition(bSuccess, return {}, REM_NO_LOG_OR_ASSERTION);

    return FVector2f{MousePosition.X - ScreenPosition.X, MousePosition.Y - ScreenPosition.Y};
}

FVector2f GetViewportSize(const TNotNull<const APlayerController*> PlayerController)
{
    FIntVector2 ViewportSize;
    PlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

    return {static_cast<float>(ViewportSize.X), static_cast<float>(ViewportSize.Y)};
}

FVector2f GetViewportCenter(const TNotNull<const APlayerController*> PlayerController)
{
    return GetViewportSize(PlayerController) / 2.0f;
}

TOptional<FVector2f> GetScreenCenterToMouse(const TNotNull<const APlayerController*> PlayerController)
{
    return GetPositionToMouseScreenSpace(PlayerController, GetViewportCenter(PlayerController));
}

TOptional<FVector> GetScreenCenterToMouseWorldSpace(
    const TNotNull<const APlayerController*> PlayerController)
{
    FVector MouseWorldLocation, MouseWorldDirection;
    auto bSuccess = PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
    RemEnsureCondition(bSuccess, return {}, REM_NO_LOG_OR_ASSERTION);

    const auto ViewportCenter = GetViewportCenter(PlayerController);

    FVector ViewportCenterWorldLocation, ViewportCenterWorldDirection;
    bSuccess = PlayerController->DeprojectScreenPositionToWorld(ViewportCenter.X, ViewportCenter.Y,
        ViewportCenterWorldLocation, ViewportCenterWorldDirection);

    RemEnsureCondition(bSuccess, return {}, REM_NO_LOG_OR_ASSERTION);


    return MouseWorldLocation - ViewportCenterWorldLocation;
}

TOptional<FVector> GetPositionToMouseWorldSpace(const TNotNull<const APlayerController*> PlayerController,
    const FVector& WorldLocation, const TOptional<double>& CustomPlaneZ)
{
    FVector MouseWorldLocation, MouseWorldDirection;
    const auto bSuccess = PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
    RemEnsureCondition(bSuccess, return {}, REM_NO_LOG_OR_ASSERTION);

    const auto TargetPlaneZ{CustomPlaneZ.Get(WorldLocation.Z)};

    // the formular : TargetPlaneZ = MouseWorldLocation.Z + ScalerOfZAxis * MouseWorldDirection.Z
    const auto ScalerOfZAxis = (TargetPlaneZ - MouseWorldLocation.Z) / MouseWorldDirection.Z;
    RemEnsureCondition(ScalerOfZAxis > 0.0f, return {});

    const auto MouseWorldPositionOnPlane = MouseWorldLocation + MouseWorldDirection * ScalerOfZAxis;
    return MouseWorldPositionOnPlane - WorldLocation;
}

FVector2f ScreenToWorld(const TNotNull<const APlayerController*> PlayerController, const FVector2f ScreenDirection)
{
    const auto ForwardDirection{Math::AngleToDirectionXY(PlayerController->GetControlRotation().Yaw)};
    const auto RightDirection{Math::PerpendicularCounterClockwiseXY(ForwardDirection)};

    // y is inverted, and x,y are swapped
    return FVector2f{ForwardDirection * -ScreenDirection.Y + RightDirection * ScreenDirection.X};
}

UObject* GetMovementBase(const FBasedMovementInfo& BasedMovement)
{
    return BasedMovement.MovementBaseInterfaceData.GetMovementBaseObject();
}

}
