// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * @brief Use this macro to help define two version of Getters --- "return pointer" or "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate, Get##NamePredicate##Ref
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_GETTERS_RETURN_POINTER_AND_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	DEFINE_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	\
	DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*suffix*/Ref, MemberAsPointer)

/**
 * @brief Use this macro to help define one version of Getters --- "return pointer"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	const ReturnType* Get##NamePredicate() const \
	{ \
		return MemberAsPointer; \
	} \
	\
	ReturnType* Get##NamePredicate() \
	{ \
		return const_cast<ReturnType*>(const_cast<const Type*>(this)->Get##NamePredicate()); \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, NameSuffix, MemberAsPointer) \
	const ReturnType& Get##NamePredicate##NameSuffix() const \
	{ \
		return *MemberAsPointer; \
	} \
	\
	ReturnType& Get##NamePredicate##NameSuffix() \
	{ \
		return const_cast<ReturnType&>(const_cast<const Type*>(this)->Get##NamePredicate##NameSuffix()); \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_GETTERS_RETURN_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*no suffix*/, MemberAsPointer)

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_TYPED_GETTER_RETURN_POINTER_AND_REFERENCE(NamePredicate, ValidationStatement) \
	DEFINE_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	\
	DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, /*suffix*/Ref) \
	
/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	template<typename T> \
	const T* Get##NamePredicate() const \
	{ \
		{ \
			ValidationStatement \
		} \
		return static_cast<const T*>(Get##NamePredicate()); \
	} \
	\
	template<typename T> \
	T* Get##NamePredicate() \
	{ \
		return const_cast<T*>(const_cast<const Type*>(this)->Get##NamePredicate<T>()); \
	}

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 */
#define DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, NameSuffix) \
	template<typename T> \
	const T& Get##NamePredicate##NameSuffix() const \
	{ \
		{ \
			ValidationStatement \
		} \
		return static_cast<const T&>(Get##NamePredicate##NameSuffix()); \
	} \
	\
	template<typename T> \
	T& Get##NamePredicate##NameSuffix() \
	{ \
		return const_cast<T&>(const_cast<const Type*>(this)->Get##NamePredicate##NameSuffix<T>()); \
	}

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_TYPED_GETTER_RETURN_REFERENCE(NamePredicate, ValidationStatement) \
	DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, /*no suffix*/)

/**
 * @brief Use this macro to help define two version of Getters --- "return pointer" or "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate, Get##NamePredicate##Ref
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER_AND_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	\
	DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*suffix*/Ref, MemberAsPointer)

/**
 * @brief Use this macro to help define one version of Getters --- "return pointer"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	const ReturnType* Get##NamePredicate() const \
	{ \
		return MemberAsPointer; \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, NameSuffix, MemberAsPointer) \
	const ReturnType& Get##NamePredicate##NameSuffix() const \
	{ \
		return *MemberAsPointer; \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*no suffix*/, MemberAsPointer)

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER_AND_REFERENCE(NamePredicate, ValidationStatement) \
	DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	\
	DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, /*suffix*/Ref) \

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	template<typename T> \
	const T* Get##NamePredicate() const \
	{ \
		{ \
			ValidationStatement \
		} \
		return static_cast<const T*>(Get##NamePredicate()); \
	}

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 */
#define DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, NameSuffix) \
	template<typename T> \
	const T& Get##NamePredicate##NameSuffix() const \
	{ \
		{ \
			ValidationStatement \
		} \
		return static_cast<const T&>(Get##NamePredicate##NameSuffix()); \
	}

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE(NamePredicate, ValidationStatement) \
	DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ValidationStatement, /*no suffix*/)


#define DEFINE_THE_RULE_OF_FIVE(Type)																\
	Type(const Type&) = default;               														\
	Type(Type&&) noexcept = default;           														\
	Type& operator=(const Type&) = default;    														\
	Type& operator=(Type&&) noexcept = default;														\
	~Type() noexcept = default;