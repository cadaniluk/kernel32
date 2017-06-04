#ifndef MALLOC_H
#define MALLOC_H

#include <basetypes.h>

/*
 * Allocates a certain number of pages and returns a pointer to that contiguous
 * memory area. The pages need not be physically consecutive, though.
 */
void *vmalloc(size_t pages);

/* TODO: add something like a pmalloc function that allocates physically
 * contiguous pages.
 */

#endif