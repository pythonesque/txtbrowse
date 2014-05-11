#include "window.h"
#include "splay.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_KEYS 0xF0000 /* number of keys to generate */

static intptr_t keys[NUM_KEYS]; /* statically initialized to 0 */

/* To make sure our tree is working for both existence and nonexistence, we
 * generate NUM_KEYS random keys for our array, but decide whether to insert
 * them into the tree or not based on whether the random variable is even or
 * not.  To avoid feeding all even (all odd) inputs to the splay function, we
 * shift it right one bit before sending it to the sort function (which is
 * obviously far from perfect, but it's better than nothing).  One obvious
 * problem with this approach is that it is possible for two nodes that differ
 * only in the lsb to both be in the array at the same time, so if
 * splay_tree_search returns true for an array element that was expecting false
 * you have to traverse the entire array in order to confirm that it's actually
 * incorrect (but that's okay for this simple implementation). */
#define KEY_GEN(index) (keys[(index)] = (intptr_t) rand())
#define KEY_INS(index) (keys[(index)] & ((intptr_t) 1))
#define KEY_VAL(index) ((void const *) (keys[(index)] >> 1))

static int
intcmp(void const *a, void const *b)
{
    intptr_t const i = (intptr_t) a;
    intptr_t const j = (intptr_t) b;

    if (i < j) {
        return(-1);
    }
    return(i != j);
}

int main(void)
{
    struct splay_tree * const sp_tree = splay_tree_init(intcmp);
    if (sp_tree == NULL) {
        exit(-1);
    }

    /* Seed PRNG with current time */
    time_t const seed = time(NULL);
    if (seed < ((time_t) 0))
    {
        perror("time");
        splay_tree_destroy(sp_tree);
        exit(-1);
    }
    srand((unsigned int) seed);

    for (size_t i = 0; i < NUM_KEYS; ++i)
    {
        KEY_GEN(i);
        if (KEY_INS(i)) {
            printf("%" PRIdPTR "\n", (intptr_t) KEY_VAL(i));
            if (splay_tree_insert(sp_tree, KEY_VAL(i)) < 0) {
                splay_tree_destroy(sp_tree);
                exit(-1);
            }
        }
    }

    splay_tree_destroy(sp_tree);
    exit(0);
}
