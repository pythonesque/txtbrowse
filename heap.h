#ifndef __HEAP_H
#define __HEAP_H

#include <assert.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t count;
} heap;

heap
heap_init(void) {
    heap h;
    h.data = (int *) malloc(sizeof(int)) - 1;
    assert ((h.data + 1) != NULL);
    h.size = 1;
    h.count = 0;
    return h;
}

void
heap_destroy(heap h) {
    free(h.data);
}

bool
heap_push(heap *h, int key) {
    if (h->count == h->size) {
        int *ndata = realloc(h->data + 1, (h->size << 1) * sizeof(int));
        if (ndata == NULL) {
            return false;
        }
        h->data = ndata - 1;
        h->size <<= 1;
    }
    size_t index = ++h->count;
    while (index > 1) {
        size_t parent = index >> 1;
        if (key <= h->data[parent]) {
            break;
        }
        h->data[index] = h->data[parent];
        index = parent;
    }
    h->data[index] = key;
    return true;
}

bool
heap_pop(heap *h) {
    if (!h->count) {
        return false;
    }
    int max = h->data[h->count];
    if (h->count - 1 < h->size >> 2) {
        int *ndata = realloc(h->data + 1, (h->size >> 1) * sizeof(int));
        if (ndata == NULL) {
            return false;
        }
        h->data = ndata - 1;
        h->size >>= 1;
    }
    --h->count;
    size_t index = 1;
    size_t swap = 0;
    for (;;) {
        size_t left = (index << 1);
        if (left > h->count) {
            break;
        }
        size_t right = left + 1;
        if (right < h->count && h->data[left] <= h->data[right]) {
            swap = right;
        } else {
            swap = left;
        }
        if (h->data[swap] <= max) {
            break;
        }
        h->data[index] = h->data[swap];
        index = swap;
    }
    h->data[index] = h->data[swap];
    return true;
}

#endif /* !HEAP_H */
