#ifndef __LIMITS_H
#define __LIMITS_H

_Static_assert(sizeof(short) == 2, "`short' is not two bytes");
_Static_assert(sizeof(int) == 4, "`int' is not four bytes");
_Static_assert(sizeof(long) == 4, "`long' is not four bytes");
_Static_assert(sizeof(long long) == 8, "`long long' is not eight bytes");

/* TODO: these are just the numerical limits so far. add all other macros! */

#define CHAR_BIT 8
#define WORD_BIT 32

#define UCHAR_MAX 255

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define CHAR_MIN
#define CHAR_MAX

#define SHRT_MIN (-32768)
#define SHRT_MAX 32767
#define USHRT_MAX 65535

#define INT_MIN (-2147483648)
#define INT_MAX 2147483647
#define UINT_MAX (~0U)

/* TODO: add (unsigned) long and (unsigned) long long mins and maxes. */

/* TODO: fix MB_LEN_MAX and SSIZE_MAX, both have wrong values */
#define MB_LEN_MAX 1
#define SSIZE_MAX 32767

#endif