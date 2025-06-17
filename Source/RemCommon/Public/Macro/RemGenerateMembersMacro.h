// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#pragma region Helpers

#define REM_DEFINE_GETTERS_RETURN_TYPE(Type, NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	Type Get##NamePredicate##NameSuffix() ConstKeyword \
	{ \
		return ReturnValue; \
	}

#define REM_DEFINE_GETTERS_RETURN_AUTO_REFERENCE(NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_TYPE(auto&&, NamePredicate, NameSuffix, ConstKeyword, ReturnValue)

#define REM_DEFINE_GETTERS_RETURN_AUTO(NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_TYPE(auto, NamePredicate, NameSuffix, ConstKeyword, ReturnValue)

#define REM_DEFINE_TEMPLATE_GETTERS_RETURN_TYPE(YourConcept, Type, NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	template<YourConcept T> \
	Type Get##NamePredicate##NameSuffix() ConstKeyword \
	{ \
		return ReturnValue; \
	}

#define REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO_REFERENCE(YourConcept, NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_TYPE(YourConcept, auto&&, NamePredicate, NameSuffix, ConstKeyword, ReturnValue)

#define REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO(YourConcept, NamePredicate, NameSuffix, ConstKeyword, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_TYPE(YourConcept, auto, NamePredicate, NameSuffix, ConstKeyword, ReturnValue)

#pragma endregion Helpers

#pragma region Getter


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define getters that return pointer and reference, OR one of them (with const and none-const version)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Use this macro to help define getter --- "return reference", const and no cost
 * @param NamePredicate getter's meaningful name part
 * @param NameSuffix suffix of the getter's name
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_GETTERS_RETURN_REFERENCE(NamePredicate, NameSuffix, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_AUTO_REFERENCE(NamePredicate, NameSuffix, const, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_AUTO_REFERENCE(NamePredicate, NameSuffix, /*no const*/, ReturnValue)

/**
 * @brief Use this macro to help define getter --- "return value", const only
 * @param NamePredicate getter's meaningful name part
 * @param NameSuffix suffix of the getter's name
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_GETTERS_RETURN_VALUE(NamePredicate, NameSuffix, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_AUTO(NamePredicate, NameSuffix, const, ReturnValue)

#define REM_DEFINE_GETTERS_RETURN_REFERENCE_SIMPLE(NamePredicate) \
	REM_DEFINE_GETTERS_RETURN_REFERENCE(NamePredicate, /*no suffix*/, NamePredicate)

#define REM_DEFINE_GETTERS_RETURN_VALUE_SIMPLE(NamePredicate) \
	REM_DEFINE_GETTERS_RETURN_VALUE(NamePredicate, /*no suffix*/, NamePredicate)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define getters that return pointer and reference, OR one of them (const version only)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Use this macro to help define getter --- "return reference", const and no cost
 * @param NamePredicate getter's meaningful name part
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE(NamePredicate, ReturnValue) \
	REM_DEFINE_GETTERS_RETURN_AUTO_REFERENCE(NamePredicate, /*no suffix*/, const, ReturnValue)

#define REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_SIMPLE(NamePredicate) \
	REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE(NamePredicate, NamePredicate)

#pragma endregion Getter



#pragma region Getter template

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// Define TEMPLATE version of getters that return pointer and reference, OR one of them
///////////// providing variant that could use to customize the ReturnStatement
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Use this macro to help define getter --- "return reference", const and no cost
 * @param YourConcept required concept for the template getter
 * @param NamePredicate getter's meaningful name part
 * @param NameSuffix suffix of the getter's name
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_TEMPLATE_GETTER_RETURN_REFERENCE(YourConcept, NamePredicate, NameSuffix, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO_REFERENCE(YourConcept, NamePredicate, NameSuffix, const, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO_REFERENCE(YourConcept, NamePredicate, NameSuffix, /*no const*/, ReturnValue)

/**
 * @brief Use this macro to help define getter --- "return value", const only
 * @param YourConcept required concept for the template getter
 * @param NamePredicate getter's meaningful name part
 * @param NameSuffix suffix of the getter's name
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(YourConcept, NamePredicate, NameSuffix, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO(YourConcept, NamePredicate, NameSuffix, const, ReturnValue) \

/**
 * @brief Use this macro to help define getters --- "return reference", const only
 * @param YourConcept required concept for the template getter
 * @param NamePredicate getter's meaningful name part
 * @param NameSuffix suffix of the getter's name
 * @param ReturnValue statement without the "return" keyword
 */
#define REM_DEFINE_CONST_ONLY_TEMPLATE_GETTER_RETURN_REFERENCE(YourConcept, NamePredicate, NameSuffix, ReturnValue) \
	REM_DEFINE_TEMPLATE_GETTERS_RETURN_AUTO_REFERENCE(YourConcept, NamePredicate, NameSuffix, const, ReturnValue)

#pragma endregion Getter template



#pragma region Rule Of Five

#define REM_DEFINE_THE_RULE_OF_FIVE(Type)															\
	Type(const Type&) = default;																	\
	Type(Type&&) noexcept = default;																\
	Type& operator=(const Type&) = default;															\
	Type& operator=(Type&&) noexcept = default;														\
	~Type() noexcept = default;

#pragma endregion Rule Of Five

#define REM_DEFINE_GET_SCRIPT_STRUCT_INTERFACE \
	virtual UScriptStruct& GetScriptStruct() const override \
	{ \
		return *StaticStruct(); \
	}
