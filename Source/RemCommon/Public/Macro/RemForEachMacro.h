// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

// taken from https://www.scs.stanford.edu/~dm/blog/va-opt.html

// #define PARENS ()
//
// #define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
// #define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
// #define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
// #define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
// #define EXPAND1(...) __VA_ARGS__
//
// #define FOR_EACH(macro, ...)                                    \
//   __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
// #define FOR_EACH_HELPER(macro, a1, ...)                         \
//   macro(a1)                                                     \
//   __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
// #define FOR_EACH_AGAIN() FOR_EACH_HELPER
//
// FOR_EACH(F, a, b, c, 1, 2, 3)   // => F(a) F(b) F(c) F(1) F(2) F(3)
//
// ** renamed to avoid naming conflict **

#define REM_PARENS ()

#define REM_EXPAND(...) REM_EXPAND4(REM_EXPAND4(REM_EXPAND4(REM_EXPAND4(__VA_ARGS__))))
#define REM_EXPAND4(...) REM_EXPAND3(REM_EXPAND3(REM_EXPAND3(REM_EXPAND3(__VA_ARGS__))))
#define REM_EXPAND3(...) REM_EXPAND2(REM_EXPAND2(REM_EXPAND2(REM_EXPAND2(__VA_ARGS__))))
#define REM_EXPAND2(...) REM_EXPAND1(REM_EXPAND1(REM_EXPAND1(REM_EXPAND1(__VA_ARGS__))))
#define REM_EXPAND1(...) __VA_ARGS__

#define REM_FOR_EACH(macro, ...)                                    \
  __VA_OPT__(REM_EXPAND(REM_FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define REM_FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                         \
  __VA_OPT__(REM_FOR_EACH_AGAIN REM_PARENS (macro, __VA_ARGS__))
#define REM_FOR_EACH_AGAIN() REM_FOR_EACH_HELPER
