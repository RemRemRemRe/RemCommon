// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#pragma region Helper Macro Definition

// @see https://stackoverflow.com/a/56038661 and related posts it referred
//  detailed explanation : https://stackoverflow.com/a/28074198

// This part you put in some library header:
#define REM_FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define REM_FUNC_COMPOSER(argsWithParentheses) REM_FUNC_CHOOSER argsWithParentheses
#define REM_CHOOSE_FROM_ARG_COUNT(F, ...) REM_FUNC_COMPOSER((__VA_ARGS__, \
F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,\
F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, ))
#define REM_NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
#define REM_MACRO_CHOOSER(FUNC, ...) REM_CHOOSE_FROM_ARG_COUNT(FUNC, REM_NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define REM_MULTI_MACRO(FUNC, ...) REM_MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)

#pragma endregion Helper Macro Definition

//zero unnecessary check
