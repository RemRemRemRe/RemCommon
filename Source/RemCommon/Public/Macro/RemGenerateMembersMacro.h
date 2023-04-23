// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define getters that return pointer and reference, OR one of them (with const and none-const version)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief Use this macro to help define two version of Getters --- "return pointer" or "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate, Get##NamePredicate##Ref
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_GETTERS_RETURN_POINTER_AND_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	REM_DEFINE_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	\
	REM_DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*suffix*/Ref, MemberAsPointer)

/**
 * @brief Use this macro to help define one version of Getters --- "return pointer"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	\
	ReturnType* Get##NamePredicate() \
	{ \
		using ThisType = std::remove_pointer_t<decltype(this)>; \
		return const_cast<ReturnType*>(const_cast<const ThisType*>(this)->Get##NamePredicate()); \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, NameSuffix, MemberAsPointer) \
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, NameSuffix, MemberAsPointer) \
	\
	ReturnType& Get##NamePredicate##NameSuffix() \
	{ \
		using ThisType = std::remove_pointer_t<decltype(this)>; \
		return const_cast<ReturnType&>(const_cast<const ThisType*>(this)->Get##NamePredicate##NameSuffix()); \
	}

/**
 * @brief Use this macro to help define one version of Getters --- "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_GETTERS_RETURN_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	REM_DEFINE_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*no suffix*/, MemberAsPointer)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define getters that return pointer and reference, OR one of them (const version only)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief Use this macro to help define two version of Getters --- "return pointer" or "return reference"
 * @param NamePredicate getter's unique name part : Get##NamePredicate, Get##NamePredicate##Ref
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER_AND_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
	\
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*suffix*/Ref, MemberAsPointer)

/**
 * @brief Use this macro to help define one version of Getters --- "return pointer"
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ReturnType getter's return type (raw, decayed)
 * @param MemberAsPointer it has to be a pointer : if the member is a value type then pass "&member" else "member"
 */
#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_POINTER(NamePredicate, ReturnType, MemberAsPointer) \
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
#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, NameSuffix, MemberAsPointer) \
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
#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE(NamePredicate, ReturnType, MemberAsPointer) \
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, ReturnType, /*no suffix*/, MemberAsPointer)

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER_AND_REFERENCE(NamePredicate, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	\
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, /*suffix*/Ref, ValidationStatement) \

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE(NamePredicate, T*, ValidationStatement) \

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, NameSuffix, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE(NamePredicate##NameSuffix, T&, ValidationStatement) \

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE(NamePredicate, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, /*no suffix*/, ValidationStatement)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define TEMPLATE version of getters that return pointer and reference, OR one of them
///////////// providing variant that could use to customize the ReturnStatement
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_TYPED_GETTER_RETURN_POINTER_AND_REFERENCE(NamePredicate, ValidationStatement) \
	REM_DEFINE_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	\
	REM_DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, /*suffix*/Ref, ValidationStatement) \



#define REM_DEFINE_TYPED_GETTER_RETURN_TYPE(NamePredicate, ReturnType, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE(NamePredicate, ReturnType, ValidationStatement) \
	\
	REM_INTERNAL_DEFINE_TYPED_GETTER_RETURN_STATEMENT(/*none const*/, NamePredicate, ValidationStatement, \
	return const_cast<ReturnType>(const_cast<const ThisType*>(this)->Get##NamePredicate<T>()))

#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE(NamePredicate, ReturnType, ValidationStatement) \
	REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE_STATEMENT(NamePredicate, ValidationStatement, \
	return static_cast<const ReturnType>(Get##NamePredicate()))


#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_TYPE_STATEMENT(NamePredicate, ValidationStatement, ReturnStatement) \
	REM_INTERNAL_DEFINE_TYPED_GETTER_RETURN_STATEMENT(const, NamePredicate, ValidationStatement, ReturnStatement)

#define REM_DEFINE_CONST_ONLY_TYPED_GETTER_RETURN_NONE_CONST_TYPE_STATEMENT(NamePredicate, ValidationStatement, ReturnStatement) \
	REM_INTERNAL_DEFINE_TYPED_GETTER_RETURN_STATEMENT(const, NamePredicate, ValidationStatement, ReturnStatement)

#define REM_INTERNAL_DEFINE_TYPED_GETTER_RETURN_STATEMENT(GetterConstness, NamePredicate, ValidationStatement, ReturnStatement) \
	template<typename T> \
	decltype(auto) Get##NamePredicate() GetterConstness \
	{ \
		using ThisType = std::remove_pointer_t<decltype(this)>; \
		{ \
			ValidationStatement; \
		} \
		ReturnStatement; \
	}


/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_TYPED_GETTER_RETURN_POINTER(NamePredicate, ValidationStatement) \
	REM_DEFINE_TYPED_GETTER_RETURN_TYPE(NamePredicate, T*, ValidationStatement)

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param NameSuffix suffix of the getter's name, so the final name : Get##NamePredicate##NameSuffix
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define REM_DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, NameSuffix, ValidationStatement) \
	REM_DEFINE_TYPED_GETTER_RETURN_TYPE(NamePredicate##NameSuffix, T&, ValidationStatement)

/**
 * @brief Define the template version of these getters (they should be defined first)
 * @param NamePredicate getter's unique name part : Get##NamePredicate
 * @param ValidationStatement statements that could use to validate the incoming type T
 */
#define DEFINE_TYPED_GETTER_RETURN_REFERENCE(NamePredicate, ValidationStatement) \
	REM_DEFINE_TYPED_GETTER_RETURN_REFERENCE_WITH_SUFFIX(NamePredicate, /*no suffix*/, ValidationStatement)


#define DEFINE_THE_RULE_OF_FIVE(Type)																\
	Type(const Type&) = default;               														\
	Type(Type&&) noexcept = default;           														\
	Type& operator=(const Type&) = default;    														\
	Type& operator=(Type&&) noexcept = default;														\
	~Type() noexcept = default;
