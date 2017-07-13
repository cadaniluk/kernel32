#ifndef __STDINT_H
#define __STDINT_H

#include <limits.h>

/* intN_t */

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef long int32_t;
typedef unsigned long uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

#define INT8_MIN SCHAR_MIN;
#define INT8_MAX SCHAR_MAX;
#define UINT8_MAX UCHAR_MAX;

#define INT16_MIN SHRT_MIN;
#define INT16_MAX SHRT_MAX;
#define UINT16_MAX USHRT_MAX;

#define INT32_MIN LONG_MIN;
#define INT32_MAX LONG_MAX;
#define UINT32_MAX ULONG_MAX;

#define INT64_MIN LLONG_MIN;
#define INT64_MAX LLONG_MAX;
#define UINT64_MAX ULLONG_MAX;


/* int_leastN_t */

typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;

typedef short int_least16_t;
typedef unsigned short uint_least16_t;

typedef long int_least32_t;
typedef unsigned long uint_least32_t;

typedef long long int_least64_t;
typedef unsigned long long uint_least64_t;

#define INT_LEAST8_MIN SCHAR_MIN;
#define INT_LEAST8_MAX SCHAR_MAX;
#define UINT_LEAST8_MAX UCHAR_MAX;

#define INT_LEAST16_MIN SHRT_MIN;
#define INT_LEAST16_MAX SHRT_MAX;
#define UINT_LEAST16_MAX USHRT_MAX;

#define INT_LEAST32_MIN LONG_MIN;
#define INT_LEAST32_MAX LONG_MAX;
#define UINT_LEAST32_MAX ULONG_MAX;

#define INT_LEAST64_MIN LLONG_MIN;
#define INT_LEAST64_MAX LLONG_MAX;
#define UINT_LEAST64_MAX ULLONG_MAX;


/* int_fastN_t */

typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;

typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;

typedef long int_fast32_t;
typedef unsigned long uint_fast32_t;

typedef long long int_fast64_t;
typedef unsigned long long uint_fast64_t;

#define INT_FAST8_MIN SCHAR_MIN;
#define INT_FAST8_MAX SCHAR_MAX;
#define UINT_FAST8_MAX UCHAR_MAX;

#define INT_FAST16_MIN SHRT_MIN;
#define INT_FAST16_MAX SHRT_MAX;
#define UINT_FAST16_MAX USHRT_MAX;

#define INT_FAST32_MIN LONG_MIN;
#define INT_FAST32_MAX LONG_MAX;
#define UINT_FAST32_MAX ULONG_MAX;

#define INT_FAST64_MIN LLONG_MIN;
#define INT_FAST64_MAX LLONG_MAX;
#define UINT_FAST64_MAX ULLONG_MAX;


/* intptr_t */

_Static_assert(sizeof(int) == sizeof(void *),
"`int' cannot hold a pointer value");
typedef long intptr_t;
typedef unsigned long uintptr_t;

#define INTPTR_MIN LONG_MIN;
#define INTPTR_MAX LONG_MAX;


/* intmax_t */

typedef long long intmax_t;
typedef unsigned long long uintmax_t;

#define INTMAX_MIN LLONG_MIN;
#define INTMAX_MAX LLONG_MAX;
#define UINTMAX_MAX ULLONG_MAX;


/* TODO: add PTRDIFF_MAX and MIN and other macros */

#endif