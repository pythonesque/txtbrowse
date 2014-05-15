#include "sort.h"

static void
swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

static void
qsort_internal(int *l, int *u)
{
    if (u <= l) {
        return;
    }
    int *m = l;
    for (int *p = l + 1; p <= u; ++p) {
        if (*p < *l) {
            swap(++m, p);
        }
    }
    swap(l, m);
    qsort_internal(l, m - 1);
    qsort_internal(m + 1, u);
}

void
quicksort(int *array, size_t nitems)
{
    qsort_internal(array, array + nitems - 1);
}
