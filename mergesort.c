#include "sort.h"

#include <stdlib.h>
#include <string.h>
 
static void
merge(int *a, size_t m, size_t nitems, int *b)
{
    int *i0 = a;
    int *i1 = a + m;
    int *im = a + m;
    int *iend = a + nitems;
    for (size_t j = 0; j < nitems; ++j) {
        if (i0 < im && (i1 >= iend || *i0 <= *i1)) {
            b[j] = *(i0++);
        } else {
            b[j] = *(i1++);
        }
    }
}

static void
msort_internal(int *a, size_t nitems, int *b)
{
    if (nitems < 2) {
        return;
    }
    size_t m = nitems / 2;
    msort_internal(a, m, b);
    msort_internal(a + m, nitems - m, b + m);
    merge(a, m, nitems, b);
    memmove(a, b, nitems * sizeof(int));
}

int
mergesort(int *array, size_t nitems)
{
    if (!nitems) {
        return 0;
    }
    int *narray = malloc(nitems * sizeof(int));
    if (narray == NULL) {
        return -1;
    }
    msort_internal(array, nitems, narray);
    free(narray);
    return(0);
}
