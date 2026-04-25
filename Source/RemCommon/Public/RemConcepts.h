// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Engine/EngineBaseTypes.h"

class UGameInstance;
class UCurveLinearColor;
class UCurveVector;
class UCurveFloat;
class UCurveBase;
class AHUD;
class UCameraModifier;
class UPoseAsset;
class UAnimStreamable;
class UAnimComposite;
class UAimOffsetBlendSpace;
class UBlendSpace;
class UAnimMontage;
class UAnimSequence;
class UAnimSequenceBase;
class UAnimationAsset;
class UMediaTexture;
struct FGameplayTag;
class FSoftObjectProperty;
class FObjectPropertyBase;
class FProperty;
class UMaterialInterface;
class UDataAsset;
class UPrimaryDataAsset;
class APlayerCameraManager;
class APlayerController;
class AController;
class APawn;
class APlayerState;
class UActorComponent;
class AActor;
class UObject;
class UWorld;
struct FSoftObjectPath;
class UClass;
class UScriptStruct;
enum ENetRole : int;
//enum ENetMode;
class FString;
class AAIController;
class ISlateTextureAtlasInterface;
class ULocalPlayer;
class UMovementComponent;
class AGameStateBase;
class AGameModeBase;
class UAnimInstance;
class UStreamableRenderAsset;
struct FTableRowBase;
template <typename T>
struct TIsUEnumClass;
class FDelegateHandle;

namespace Rem
{
template <class T>
concept CHasGetWorld = requires(UWorld* World, const T Object)
{
    World = Object.GetWorld();
};

template <class T>
concept CHasGetLocalRole = requires(ENetRole Role, const T Object)
{
    Role = Object.GetLocalRole();
};

template <class T>
concept CHasGetOwnerRole = requires(ENetRole Role, const T Object)
{
    Role = Object.GetOwnerRole();
};

template <class T>
concept CHasGetNetMode = requires(ENetMode Mode, const T Object)
{
    Mode = Object.GetNetMode();
};

template <class T>
concept CHasGetOwner = requires(const T Object)
{
    Object.GetOwner();
};

template <class T>
concept CHasGet = requires(const T Object)
{
    Object.Get();
};

template <class T>
concept CHasTag = requires(FGameplayTag& Tag, const T Object)
{
    Tag = T::Tag;
};

template <class T>
concept CHasGetTag = requires(FGameplayTag& Tag, const T Object)
{
    Tag = Object.GetTag();
};

template <class T>
concept CHasIsNetMode = requires(ENetMode Mode, const T Object, bool Result)
{
    Result = Object.IsNetMode(Mode);
};

template <class T>
concept CHasToCompactString = requires(FString& String, const T Object)
{
    String = Object.ToCompactString();
};

template <class T>
concept CHasToStringSimple = requires(FString& String, const T Object)
{
    String = Object.ToStringSimple();
};

template <class T>
concept CHasToSimpleString = requires(FString& String, const T Object)
{
    String = Object.ToSimpleString();
};

template <class T>
concept CHasToString = requires(FString& String, const T Object)
{
    String = Object.ToString();
};

template <class T>
concept CCanLexToString = requires(FString& String, const T Object)
{
    String = LexToString(Object);
};

template <class T>
concept CHasStaticStruct = requires(const UScriptStruct* ScriptStruct)
{
    ScriptStruct = T::StaticStruct();
};

template <class T>
concept CHasStaticClass = requires(UClass*& StructRef)
{
    StructRef = T::StaticClass();
};

template <class T>
concept CHasGetName = requires(FString& String, const T Object)
{
    String = Object.GetName();
};

template <class T>
concept CHasGetFullName = requires(FString& String, const T Object)
{
    String = Object.GetFullName();
};

template <class T>
concept CUEnum = std::is_enum_v<T> && requires(UEnum* Enum)
{
    Enum = StaticEnum<T>();
};

template <class T>
concept CStringable =
    std::is_same_v<bool, std::remove_cvref_t<T>>
    || CHasToCompactString<T>
    || CHasToStringSimple<T>
    || CHasToSimpleString<T>
    || CHasToString<T>
    || CHasGetName<T>
    || std::is_same_v<FDelegateHandle, std::remove_cvref_t<T>>
    || CUEnum<T>
    || TIsUEnumClass<T>::Value == 1
    || CHasStaticStruct<T>
    || CCanLexToString<T>
    ;

template <class T>
concept CHasIsValid = requires(bool Result, const T Object)
{
    Result = Object.IsValid();
};

template <class T>
concept CHasToSoftObjectPath = requires(FSoftObjectPath& SoftPath, bool bNull, const T Object)
{
    SoftPath = Object.ToSoftObjectPath();
};

template <class T>
concept CUWorld = std::derived_from<T, UWorld>;

template <class T>
concept CUGameInstance = std::derived_from<T, UGameInstance>;

template <class T>
concept CUObject = std::derived_from<T, UObject>;

template <class T>
concept CUClass = std::derived_from<T, UClass>;

template <class T>
concept CAHUD = std::derived_from<T, AHUD>;

template <class T>
concept CAActor = std::derived_from<T, AActor>;

template <class T>
concept CUActorComponent = std::derived_from<T, UActorComponent>;

template <class T>
concept CUMovementComponent = std::derived_from<T, UMovementComponent>;

template <class T>
concept CAPlayerState = std::derived_from<T, APlayerState>;

template <class T>
concept CAPawn = std::derived_from<T, APawn>;

template <class T>
concept CAController = std::derived_from<T, AController>;

template <class T>
concept CAPlayerController = std::derived_from<T, APlayerController>;

template <class T>
concept CAPlayerCameraManager = std::derived_from<T, APlayerCameraManager>;

template <class T>
concept CULocalPlayer = std::derived_from<T, ULocalPlayer>;

template <class T>
concept CAAIController = std::derived_from<T, AAIController>;

template <class T>
concept CAGameStateBase = std::derived_from<T, AGameStateBase>;

template <class T>
concept CAGameModeBase = std::derived_from<T, AGameModeBase>;

template <class T>
concept CUDataAsset = std::derived_from<T, UDataAsset>;

template <class T>
concept CUCurveBase = std::derived_from<T, UCurveBase>;

template <class T>
concept CUCurveFloat = std::derived_from<T, UCurveFloat>;

template <class T>
concept CUCurveVector = std::derived_from<T, UCurveVector>;

template <class T>
concept CUCurveLinearColor = std::derived_from<T, UCurveLinearColor>;

template <class T>
concept CUPrimaryDataAsset = std::derived_from<T, UPrimaryDataAsset>;

template <class T>
concept CUAnimInstance = std::derived_from<T, UAnimInstance>;

template <class T>
concept CUMaterialInterface = std::derived_from<T, UMaterialInterface>;

template <class T>
concept CUStreamableRenderAsset = std::derived_from<T, UStreamableRenderAsset>;

template <class T>
concept CUMediaTexture = std::derived_from<T, UMediaTexture>;

template <class T>
concept CISlateTextureAtlasInterface = std::derived_from<T, ISlateTextureAtlasInterface>;

template <class T>
concept CImage = !CUMediaTexture<T> && (CUMaterialInterface<T> || CISlateTextureAtlasInterface<T>);

template <class T>
concept CFProperty = std::derived_from<T, FProperty>;

template <class T>
concept CFObjectPropertyBase = std::derived_from<T, FObjectPropertyBase>;

template <class T>
concept CFSoftObjectProperty = std::derived_from<T, FSoftObjectProperty>;

template <class T>
concept CFTableRowBase = std::derived_from<T, FTableRowBase>;

template <class T>
concept CUAnimationAsset = std::derived_from<T, UAnimationAsset>;

template <class T>
concept CUAnimSequenceBase = std::derived_from<T, UAnimSequenceBase>;

template <class T>
concept CUAnimSequence = std::derived_from<T, UAnimSequence>;

template <class T>
concept CUAnimMontage = std::derived_from<T, UAnimMontage>;

template <class T>
concept is_blend_space = std::derived_from<T, UBlendSpace>;

template <class T>
concept CUAimOffsetBlendSpace = std::derived_from<T, UAimOffsetBlendSpace>;

template <class T>
concept CUAnimComposite = std::derived_from<T, UAnimComposite>;

template <class T>
concept CUAnimStreamable = std::derived_from<T, UAnimStreamable>;

template <class T>
concept CUPoseAsset = std::derived_from<T, UPoseAsset>;

template <class T>
concept CUCameraModifier = std::derived_from<T, UCameraModifier>;
}
