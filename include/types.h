#ifndef TYPES_H
#define TYPES_H

#if __ASSEMBLY__ != 1

/* TODO: make posix-compatible and dont abort compilation when some sizeof
 * doesnt match but resume with another type instead. */

typedef signed char int8_t;
typedef unsigned char uint8_t;

_Static_assert(sizeof(short) == 2, "`sizeof(short) != 2'; `int16_t' cannot be "
"defined");

typedef short int16_t;
typedef unsigned short uint16_t;

_Static_assert(sizeof(long) == 4, "`sizeof(long) != 4'; `int32_t' cannot be "
"defined");

typedef long int32_t;
typedef unsigned long uint32_t;

_Static_assert(sizeof(long long) == 8, "`sizeof(long long)' != 8; `int32_t' "
"cannot be defined");

typedef long long int64_t;
typedef unsigned long long uint64_t;

#endif /* __ASSEMBLY__ != 1 */

#endif /* TYPES_H */
