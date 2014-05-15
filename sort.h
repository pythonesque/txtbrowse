#ifndef __SORT_H
#define __SORT_H

#include <stddef.h>

typedef int sort_func_t(void *array, size_t nitems, size_t size, int (*compar)(void const *, void const *));

extern void quicksort(int *array, size_t nitems);//, size_t size, int (*compar)(void const *, void const *));

extern int mergesort(int *array, size_t nitems);

#endif /* SORT_H */
