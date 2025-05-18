

#pragma once

// Variadic macro utilities for FOR_EACH-style expansion
// Source: https://www.scs.stanford.edu/~dm/blog/va-opt.html
// 
// This header enables macros like FOR_EACH and argument unpacking using __VA_OPT__,
// supporting tuples like (Name, "String") in macro calls.
//
// Example usage:
//     #define PRINT_NAME(pair) std::cout << GET_SECOND(pair) << std::endl;
//     FOR_EACH(PRINT_NAME, (Red, "Red"), (Green, "Green"))
//
// Note: These macros rely on C++20 support for __VA_OPT__, and MSVC requires /Zc:preprocessor

#define PARENS ()

#define STRINGIFY(X) #X

#define IDENTITY(X) X
 

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__




#define FOR_EACH(macro, ...)                                                            \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))

#define FOR_EACH_HELPER(macro, a1, ...)                                                 \
  macro(a1)                                                                             \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))

#define FOR_EACH_AGAIN() FOR_EACH_HELPER




#define FOR_EACH_SEP_COMMA(macro, ...)                                                  \
  __VA_OPT__(EXPAND(FOR_EACH_SEP_COMMA_HELPER(macro, __VA_ARGS__)))

#define FOR_EACH_SEP_COMMA_HELPER(macro, a1, ...)                                       \
  macro(a1)                                                                             \
  __VA_OPT__(,)__VA_OPT__(FOR_EACH_SEP_COMMA_AGAIN PARENS (macro, __VA_ARGS__))

#define FOR_EACH_SEP_COMMA_AGAIN() FOR_EACH_SEP_COMMA_HELPER


#define UNPACK(...) __VA_ARGS__
#define UNPACK_TUPLE(TUPLE_) UNPACK TUPLE_


// 2-element tuple (a, b, c)
#define TUPLE_FIRST(a, b) a
#define TUPLE_SECOND(a, b) b

#define TUPLE_GET_FIRST(pair) TUPLE_FIRST pair
#define TUPLE_GET_SECOND(pair) TUPLE_SECOND pair



// 3-element tuple (a, b, c)
#define TUPLE_FIRST_3(a, b, c) a
#define TUPLE_SECOND_3(a, b, c) b
#define TUPLE_THIRD_3(a, b, c) c

#define TUPLE_GET_FIRST_3(t) TUPLE_FIRST_3 t
#define TUPLE_GET_SECOND_3(t) TUPLE_SECOND_3 t
#define TUPLE_GET_THIRD_3(t) TUPLE_THIRD_3 t



// 4-element tuple (a, b, c, d)
#define TUPLE_FIRST_4(a, b, c, d) a
#define TUPLE_SECOND_4(a, b, c, d) b
#define TUPLE_THIRD_4(a, b, c, d) c
#define TUPLE_FOURTH_4(a, b, c, d) d

#define TUPLE_GET_FIRST_4(t) TUPLE_FIRST_4 t
#define TUPLE_GET_SECOND_4(t) TUPLE_SECOND_4 t
#define TUPLE_GET_THIRD_4(t) TUPLE_THIRD_4 t
#define TUPLE_GET_FOURTH_4(t) TUPLE_FOURTH_4 t


#define EXPAND_TUPLE_4(t) TUPLE_GET_FIRST_4(t), TUPLE_GET_SECOND_4(t), TUPLE_GET_THIRD_4(t), TUPLE_GET_FOURTH_4(t)

// 5-element tuple (a, b, c, d, e)
#define TUPLE_FIRST_5(a, b, c, d, e) a
#define TUPLE_SECOND_5(a, b, c, d, e) b
#define TUPLE_THIRD_5(a, b, c, d, e) c
#define TUPLE_FOURTH_5(a, b, c, d, e) d
#define TUPLE_FIFTH_5(a, b, c, d, e) e

#define TUPLE_GET_FIRST_5(t) TUPLE_FIRST_5 t
#define TUPLE_GET_SECOND_5(t) TUPLE_SECOND_5 t
#define TUPLE_GET_THIRD_5(t) TUPLE_THIRD_5 t
#define TUPLE_GET_FOURTH_5(t) TUPLE_FOURTH_5 t
#define TUPLE_GET_FIFTH_5(t) TUPLE_FIFTH_5 t


#define EXPAND_TUPLE_5(t)      \
    TUPLE_GET_FIRST_5(t),      \
    TUPLE_GET_SECOND_5(t),     \
    TUPLE_GET_THIRD_5(t),      \
    TUPLE_GET_FOURTH_5(t),     \
    TUPLE_GET_FIFTH_5(t)
