// Copyright . All Rights Reserved.

#pragma once

// 1.Consider ValidatePointer ? no upper case?
// 2.Why define these macros? @see comment of TIdentity, SFINAE template parameter can not be deduced

#define VALIDATE_POINTER(Pointer) Common::IsValidPtr<decltype(Pointer)>(Pointer)
#define GET_NET_MODE(Object, NetMode) Common::GetNetMode<decltype(Object)>(Object, NetMode)
#define IS_NET_MODE(Object, NetMode) Common::IsNetMode<decltype(Object)>(Object, NetMode)

#if WITH_EDITOR

#define RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld() || !GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#else

#define RETURN_IF_NOT_GAME_WORLD

#endif

namespace Common
{
	// Validator for NONE UObject pointers
	template<typename T>
	bool IsValidPtr(
		typename TEnableIf<
			TAnd<
				TIsPointer<T>,
				TNot<TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UObject>>
			>::Value,
			const T
		>::Type Object)
	{
		return Object != nullptr;
	}

	// Validator for UObject pointers and TObjectPtr
	template<typename T>
	bool IsValidPtr(
		typename TEnableIf<
			TOr<
				TAnd<
					TIsPointer<T>,
					TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UObject>
				>,
				TIsTObjectPtr<T>
			>::Value,
			const T
		>::Type Object)
	{
		return IsValid(Object);
	}

	// Handle nullptr case, parameter name is emitted
	template<typename T>
	bool IsValidPtr(std::nullptr_t)
	{
		return false;
	}

	// GetNetMode for UObjects
	template<typename T>
	ENetMode GetNetMode(
		typename TEnableIf<
			TAnd<
				TIsPointer<T>,
				TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UObject>
			>::Value,
			const T
		>::Type Object)
	{
		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, AActor>::Value)
		{
			const AActor* Actor = Cast<AActor>(Object);
			return Actor->GetNetMode();
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UActorComponent>::Value)
		{
			const UActorComponent* ActorComponent = Cast<UActorComponent>(Object);
			return ActorComponent->GetNetMode();
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UWorld>::Value)
		{
			const UWorld* World = Cast<UWorld>(Object);
			return World->GetNetMode();
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UNetDriver>::Value)
		{
			const UNetDriver* NetDriver = Cast<UNetDriver>(Object);
			return NetDriver->GetNetMode();
		}

		return NM_MAX;
	}


	// GetNetMode for TObjectPtr
	template<typename T>
	ENetMode GetNetMode(
		typename TEnableIf<
			TIsTObjectPtr<T>::Value,
			const T
		>::Type ObjectPtr)
	{
		using ElementType = typename T::ElementType;
		return GetNetMode<ElementType*>(ObjectPtr);
	}

	// IsNetMode for UObjects
	template<typename T>
	bool IsNetMode(
		typename TEnableIf<
			TAnd<
				TIsPointer<T>,
				TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UObject>
			>::Value,
			const T
		>::Type Object, const ENetMode NetMode)
	{
		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, AActor>::Value)
		{
			const AActor* Actor = Cast<AActor>(Object);
			return Actor->IsNetMode(NetMode);
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UActorComponent>::Value)
		{
			const UActorComponent* ActorComponent = Cast<UActorComponent>(Object);
			return ActorComponent->IsNetMode(NetMode);
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UWorld>::Value)
		{
			const UWorld* World = Cast<UWorld>(Object);
			return World->IsNetMode(NetMode);
		}

		if constexpr (TIsDerivedFrom<typename TDecay<std::remove_pointer_t<T>>::Type, UNetDriver>::Value)
		{
			const UNetDriver* NetDriver = Cast<UNetDriver>(Object);
			return NetDriver->GetNetMode() == NetMode;
		}

		return {};
	}

	// IsNetMode for TObjectPtr
	template<typename T>
	bool IsNetMode(
		typename TEnableIf<
			TIsTObjectPtr<T>::Value,
			const T
		>::Type ObjectPtr, const ENetMode NetMode)
	{
		using ElementType = typename T::ElementType;
		return IsNetMode<ElementType*>(ObjectPtr, NetMode);
	}

	COMMON_API bool IsClassDefaultObject(const UObject* Object);

	COMMON_API FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	template<typename T>
	typename TEnableIf<
		TModels<CStaticStructProvider, T>::Value,
		FString
	>::Type ToString(const T& Struct)
	{
		FString HumanReadableMessage;

		T::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, Struct,
			/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
		return HumanReadableMessage;
	}
	
	namespace Impl
	{
		struct CHasGetLocalRole
		{
			template <typename T>
			auto Requires(ENetRole Role, const T* Object) -> decltype(
				Role = Object->GetLocalRole()
			);
		};

		struct CHasGetOwnerRole
		{
			template <typename T>
			auto Requires(ENetRole Role, const T* Object) -> decltype(
				Role = Object->GetOwnerRole()
			);
		};
	}
	
	template<typename T>
	typename TEnableIf<TModels<Impl::CHasGetLocalRole, T>::Value, ENetRole
	>::Type GetNetRole(const T* Object)
	{
		return Object->GetLocalRole();
	}

	template<typename T>
	typename TEnableIf<TModels<Impl::CHasGetOwnerRole, T>::Value, ENetRole
	>::Type GetNetRole(const T* Object)
	{
		return Object->GetOwnerRole();
	}

	template<typename T>
	FString GetNetRoleString(const T* Object)
	{
		if constexpr (TModels<Impl::CHasGetLocalRole, T>::Value || TModels<Impl::CHasGetOwnerRole, T>::Value)
		{
			return StaticEnum<ENetRole>()->GetValueAsString(GetNetRole(Object));
		}
		return {};
	}
	

}
