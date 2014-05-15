#include "hash.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*typedef struct hte {
    char *key;
    struct hte *next;
} hte;

typedef struct {
    hte **tbl;
    size_t size;
} htbl;*/

htbl
htbl_init(size_t size)
{
    htbl ht;
    assert (size != 0);
    ht.tbl = calloc(size, sizeof(hte *));
    assert (ht.tbl != NULL);
    ht.size = size;
    return ht;
}

void
htbl_destroy(htbl ht)
{
    for (size_t i = 0; i < ht.size; ++i) {
        hte *e = ht.tbl[i];
        while (e != NULL) {
            hte *next = e->next;
            free(e);
            e = next;
        }
    }
    free(ht.tbl);
}

#define MULT 31

static size_t
hash(char *key, size_t size)
{
    size_t h = 0;
    for (; *key; ++key) {
        h = h * MULT + *key;
    }
    return(h % size);
}

bool
htbl_insert(htbl *ht, char *key)
{
    size_t i = hash(key, ht->size);
    hte *e = ht->tbl[i];
    while (e != NULL) {
        if (!strcmp(key, e->key)) {
            return false;
        }
        e = e->next;
    }
    e = malloc(sizeof(hte));
    if (e == NULL) {
        return false;
    }
    e->key = key;
    e->next = ht->tbl[i];
    ht->tbl[i] = e;
    return true;
}

bool
htbl_find(htbl *ht, char *key)
{
    size_t i = hash(key, ht->size);
    hte *e = ht->tbl[i];
    while (e != NULL) {
        if (!strcmp(key, e->key)) {
            return true;
        }
        e = e->next;
    }
    return false;
}

bool
htbl_delete(htbl *ht, char *key)
{
    size_t i = hash(key, ht->size);
    hte **prevnext = &ht->tbl[i];
    hte *e;
    while ((e = *prevnext) != NULL) {
        if (!strcmp(key, e->key)) {
            *prevnext = e->next;
            free(e);
            return true;
        }
        prevnext = &e->next;
    }
    return false;
}
