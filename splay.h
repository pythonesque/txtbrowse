#ifndef SPLAY_H
#define SPLAY_H

typedef int cmp_t(void const *, void const *);

extern struct splay_tree *splay_tree_init(cmp_t compar);

extern void splay_tree_destroy(struct splay_tree *sp_tree);

extern int splay_tree_insert(struct splay_tree *sp_tree, void const *key);

extern int splay_tree_search(struct splay_tree *sp_tree, void const *key);

extern int splay_tree_delete(struct splay_tree *sp_tree, void const *key);

#endif /* !SPLAY_H */
