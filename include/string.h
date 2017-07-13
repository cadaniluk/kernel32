#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

#define __GET_LOCALE_T
#include <locale.h>

static inline void *memchr(const void *s, int c, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		if ((const unsigned char *) s[i] == (unsigned char ) c)
			return (const unsigned char *) s + i;

	return NULL;
}

static inline int memcmp(const void *s1, const void *s2, size_t n)
{
	for (size_t i = 0; i < n; ++i) {
		int diff =
		(const unsigned char *) s1[i] - (const unsigned char *) s2[i];

		if (diff != 0)
			return diff;
	}

	return 0;
}

static inline void *memcpy(void *dest, const void *src, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		(unsigned char *) dest[i] = (const unsigned char *) src[i];
	
	return dest;
}

static inline void *memmove(void *dest, const void *src, size_t n)
{
	/* TODO: add implementation */
	
	return dest;
}

static inline void *memset(void *s, int c, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		(unsigned char *) s[i] = (unsigned char) c;

	return s;
}

/* TODO: add the other functions */

#endif