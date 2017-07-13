#ifndef __STDDEF_H
#define __STDDEF_H

#if !defined(__GET_NULL) && !defined(__GET_SIZE_T)
#define __GET_ALL
#endif

#if defined(__GET_ALL) || defined(__GET_NULL)
#define NULL ((void *) 0)
#undef __GET_NULL
#endif

#if defined(__GET_ALL) || defined(__GET_SIZE_T)
typedef __typeof__(sizeof(int)) size_t;
#undef __GET_SIZE_T
#endif

#ifdef __GET_ALL

/* TODO: __typeof__ is very nice but a GNU extension. should we stay strictly
 * conforming instead? */

typedef __typeof__(NULL - NULL) ptrdiff_t;

typedef int wchar_t;	/* TODO: fix that, its not correct */

#endif
#undef __GET_ALL

#endif