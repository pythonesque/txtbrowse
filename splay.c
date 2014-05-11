#include "splay.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct splay_tree_node
{
    struct splay_tree_node *left;
    struct splay_tree_node *right;
    struct splay_tree_node *parent;
    void const *key;
};

struct splay_tree
{
    cmp_t *compar;
    struct splay_tree_node *root;
};

static void
splay_subtree_destroy(struct splay_tree_node *sp_node)
{
    /*
     * 1. Free the left subtree.
     */
    if (sp_node->left != NULL) {
        splay_subtree_destroy(sp_node->left);
    }

    /*
     * 2. Free the right subtree.
     */
    if (sp_node->right != NULL) {
        splay_subtree_destroy(sp_node->right);
    }

    /*
     * 3. Free the root.
     */
    free(sp_node);
}

/*static void
left_rotate(

static void
splay(struct splay_tree const *sp_tree, struct splay_tree_node const *sp_node)
{
    sp_ro
    sp_node->parent;
}*/

struct splay_tree *
splay_tree_init(cmp_t compar)
{
    struct splay_tree * const sp_tree = malloc(sizeof(struct splay_tree));
    if (sp_tree == NULL) {
        perror("malloc");
        return NULL;
    }

    sp_tree->compar = compar;
    sp_tree->root = NULL;

    return(sp_tree);
}

void
splay_tree_destroy(struct splay_tree *sp_tree)
{
    /*
     * 1. Deallocate the subtree rooted at tree->root
     */
    if (sp_tree->root != NULL) {
        splay_subtree_destroy(sp_tree->root);
    }

    /*
     * 2. Deallocate the tree itself
     */
    free(sp_tree);
}

int
splay_tree_insert(struct splay_tree *sp_tree, void const *key)
{
    /* 1. Insert the node as in a normal binary tree. */
    struct splay_tree_node * parent = NULL;
    struct splay_tree_node ** branch = &sp_tree->root;

    while(*branch != NULL) {
        parent = *branch;
        /* Note: this assumes there was no error in compar... */
        if (sp_tree->compar(key, parent->key) == 1) {
            branch = &parent->right;
        } else {
            branch = &parent->left;
        }
    }
    struct splay_tree_node * const new_node =
      malloc(sizeof(struct splay_tree_node));
    if (new_node == NULL) {
        perror("malloc");
        return -1;
    }
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = parent;
    new_node->key = key;
    *branch = new_node;

    return 0;
}

int
splay_tree_search(struct splay_tree *sp_tree, void const *key)
{
    return -1;
}

int
splay_tree_delete(struct splay_tree *sp_tree, void const *key)
{
    return -1;
}
