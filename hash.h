#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#define MULT 31

typedef struct hte {
    char *key;
    struct hte *next;
} hte;

typedef struct {
    hte **tbl;
    size_t size;
    unsigned count;
} htbl;

extern htbl htbl_init(size_t size);

extern void htbl_destroy(htbl ht);

extern bool htbl_insert(htbl *ht, char *key);

extern bool htbl_find(htbl *ht, char *key);

extern bool htbl_delete(htbl *ht, char *key);

#endif /* !__HASH_TABLE_H */
