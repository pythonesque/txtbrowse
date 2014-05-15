#include "hash.h"
#include "heap.h"
#include "sort.h"
#include "splay.h"
#include "window.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_IKEYS 0xF0000 /* number of keys to generate */
#define NUM_SKEYS 0xF0000 /* number of string keys to generate */ 

static int ikeys[NUM_IKEYS]; /* statically initialized to 0 */
static char skeys[NUM_SKEYS][11];

/* To make sure our tree is working for both existence and nonexistence, we
 * generate NUM_IKEYS random keys for our array, but decide whether to insert
 * them into the tree or not based on whether the random variable is even or
 * not.  To avoid feeding all even (all odd) inputs to the splay function, we
 * shift it right one bit before sending it to the sort function (which is
 * obviously far from perfect, but it's better than nothing).  One obvious
 * problem with this approach is that it is possible for two nodes that differ
 * only in the lsb to both be in the array at the same time, so if
 * splay_tree_search returns true for an array element that was expecting false
 * you have to traverse the entire array in order to confirm that it's actually
 * incorrect (but that's okay for this simple implementation). */
#define KEY_GEN(index) (ikeys[(index)] = (intptr_t) rand())
#define KEY_INS(index) (ikeys[(index)] & ((intptr_t) 1))
#define KEY_VAL(index) ((void const *) (ikeys[(index)] >> 1))

/*static int
intcmp(void const *a, void const *b)
{
    intptr_t const i = *((intptr_t *) a);
    intptr_t const j = *((intptr_t *) b);

    if (i < j) {
        return(-1);
    }
    return(i != j);
}

static int
ptrcmp(void const *a, void const *b)
{
    intptr_t const i = (intptr_t) a;
    intptr_t const j = (intptr_t) b;

    if (i < j) {
        return(-1);
    }
    return(i != j);
}*/

static int test_all(void);
static int test_hash_table(void);
//static int test_splay_tree(void);
static int test_sort(void);

int main(void)
{
    return(test_all());
}

int test_all(void)
{
    /* Seed PRNG with current time */
    fprintf(stderr, "test_all: seeding PRNG...\n");
    time_t const seed = time(NULL);
    if (seed < ((time_t) 0))
    {
        perror("time");
        return(-1);
    }
    srand((unsigned int) seed);
    fprintf(stderr, "test_hash_table: starting...\n");
    int const hash_table_exit = test_hash_table();
    fprintf(stderr, "test_hash_table: done, exit code: %d\n", hash_table_exit);
    /*fprintf(stderr, "test_splay_tree: starting...\n");
    int const splay_tree_exit = test_splay_tree();
    fprintf(stderr, "test_splay_tree: done, exit code: %d\n", splay_tree_exit);*/
    fprintf(stderr, "test_sort: starting...\n");
    int const sort_exit = test_sort();
    fprintf(stderr, "test_sort: done, exit code: %d\n", sort_exit);
    int const exit_code = (hash_table_exit < 0 || /*splay_tree_exit < 0 || */sort_exit < 0 ? -1 : 0);
    fprintf(stderr, "test_all: done, exit code: %d\n", exit_code);
    return exit_code;
}

int test_sort(void)
{
    uint32_t i;
    fprintf(stderr, "quicksort: starting...\n");
    for (i = 0; i < NUM_IKEYS; ++i) {
        KEY_GEN(i);
    }

    /*if (quicksort(ikeys, NUM_IKEYS, sizeof(intptr_t), intcmp) < 0) {
        fprintf(stderr, "qsort failed\n");
        return -1;
    }*/
    quicksort(ikeys, NUM_IKEYS);

    for (i = 1; i < NUM_IKEYS; ++i) {
        if (ikeys[i] < ikeys[i-1]) {
            goto error;
        }
    }

    fprintf(stderr, "mergesort: starting...\n");
    for (i = 0; i < NUM_IKEYS; ++i) {
        KEY_GEN(i);
    }

    if (mergesort(ikeys, NUM_IKEYS) < 0) {
        fprintf(stderr, "mergesort failed\n");
        return -1;
    }

    for (i = 1; i < NUM_IKEYS; ++i) {
        if (ikeys[i] < ikeys[i-1]) {
            goto error;
        }
    }
    return 0;
error:

    /*fprintf(stderr,
            "ikeys not sorted: a[%" PRIu32 "] = %" PRIdPTR
            ", a[%" PRIu32 "] = %" PRIdPTR "]\n", i - 1, ikeys[i - 1], i, ikeys[i]);*/
    fprintf(stderr,
            "ikeys not sorted: a[%" PRIu32 "] = %d" 
            ", a[%" PRIu32 "] = %d]\n", i - 1, ikeys[i - 1], i, ikeys[i]);
    return -1;
}

#define ht_assert(foo) do { \
        if ((foo)) { \
            /* fprintf(stderr, "test_hash_table: assertion passed: " #foo "\n"); */ \
        } else { \
            fprintf(stderr, "test_hash_table: assertion failed: " #foo "\n"); \
            goto error; \
        } \
    } while(0);
int test_hash_table(void)
{
    htbl t = htbl_init(NUM_SKEYS);  // 64

    ht_assert(!htbl_find(&t, "hello"));

    ht_assert(htbl_insert(&t, "hello"));

    ht_assert(htbl_find(&t, "hello"));

    ht_assert(!htbl_find(&t, "goodbye"));

    ht_assert(htbl_insert(&t, "goodbye"));

    ht_assert(htbl_find(&t, "goodbye"));

    ht_assert(htbl_delete(&t, "hello"));

    ht_assert(!htbl_find(&t, "hello"));

    ht_assert(htbl_find(&t, "goodbye"));


    for (uint32_t i = 0; i < NUM_SKEYS; ++i) {
        if (sprintf(skeys[i], "%" PRIu32, i) < 0) {
            goto error;
        }
        ht_assert(htbl_insert(&t, skeys[i]));
    }

    for (uint32_t i = 0; i < NUM_SKEYS; ++i) {
        //fprintf(stderr, "%" PRIu32 "\n", i);
        if (!htbl_find(&t, skeys[i])) {
            fprintf(stderr, "Error: %s %" PRIu32 "\n", skeys[i], i);
            goto error;
        }
        ht_assert(htbl_delete(&t, skeys[i]));
        ht_assert(!htbl_find(&t, skeys[i]));
    }

    htbl_destroy(t);
    
    return(0);

error:
    htbl_destroy(t);
    return(-1);
}
#undef ht_assert

// int test_splay_tree(void)
// {
//     struct splay_tree * const sp_tree = splay_tree_init(ptrcmp);
//     if (sp_tree == NULL) {
//         return(-1);
//     }
// 
//     for (size_t i = 0; i < NUM_IKEYS; ++i)
//     {
//         KEY_GEN(i);
//         if (KEY_INS(i)) {
//             /* printf("%" PRIdPTR "\n", (intptr_t) KEY_VAL(i)); */
//             if (splay_tree_insert(sp_tree, KEY_VAL(i)) < 0) {
//                 splay_tree_destroy(sp_tree);
//                 return(-1);
//             }
//         }
//     }
// 
//     splay_tree_destroy(sp_tree);
//     return(0);
// }
