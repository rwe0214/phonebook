#include <stdlib.h>
#include <string.h>
#include "phonebook_hash_avl_opt.h"

/* TODO: FILL YOUR OWN IMPLEMENTATION HERE! */
node *findName(char lastName[], tree *table[])
{
    unsigned int index = BKDRHash(lastName);
    if(table[index]->root) {
        node *n = table[index]->root;
        while(n) {
            if(strcmp(lastName, n->lastName)==0)
                return n;
            else if(strcmp(lastName, n->lastName)>0)
                n = n->rchild;
            else
                n = n->lchild;
        }
    }
    /* TODO: implement */
    return NULL;
}

void *append(char lastName[], tree *table[])
{
    /* TODO: implement */
    unsigned int index = BKDRHash(lastName);
    if (table[index] == NULL) {
        table[index] = create_avl();
        insert_avl(table[index], lastName);
    } else {
        insert_avl(table[index], lastName);
    }
}

unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & (HASH_TAB_SIZE-1));
}

node* rotate_rr(node *n)
{
    /*
       n(a)
         \                  b
          b       -->      / \
           \            n(a)  c
            c
    */
    node *a = n;
    node *b = n->rchild;

    a->rchild = b->lchild;
    b->lchild = a;

    return b;
}

node* rotate_ll(node *n)
{
    /*
            n(a)
           /               b
          b       -->     / \
         /               c  n(a)
        c
    */
    node *a = n;
    node *b = n->lchild;

    a->lchild = b->rchild;
    b->rchild = a;

    return b;
}

node* rotate_rl(node *n)
{
    /*
       n(a)
         \                  c
          b       -->      / \
         /              n(a)  c
        c
    */
    node *a = n;
    node *b = n->rchild;
    node *c = b->lchild;

    a->rchild = c->lchild;
    b->lchild = c->rchild;
    c->lchild = a;
    c->rchild = b;

    return c;
}

node* rotate_lr(node *n)
{
    /*
            n(a)
           /               c
          b       -->     / \
           \             b  n(a)
            c
    */
    node *a = n;
    node *b = n->lchild;
    node *c = b->rchild;

    a->lchild = c->rchild;
    b->rchild = c->lchild;
    c->lchild = b;
    c->rchild = a;

    return c;
}

tree* create_avl()
{
    tree *t = malloc(sizeof(tree));
    t->root = NULL;
    return t;
}

node* create_node()
{
    node *n = malloc(sizeof(node));
    n->lchild = NULL;
    n->rchild = NULL;
    return n;
}

void insert_avl(tree *t, char lastName[])
{
    node *new;
    node *last;
    node *next;
    //tree is empty
    if(t->root == NULL) {
        new = create_node();
        strcpy(new->lastName, lastName);
        t->root = new;
    } else {
        next = t->root;
        while(next) {
            //AVL is a BST
            last = next;
            if(strcmp(lastName, next->lastName)>0)
                next = next->rchild;
            else
                next = next->lchild;
        }
        new = create_node();
        strcpy(new->lastName, lastName);

        if(strcmp(lastName, last->lastName))
            last->rchild = new;
        else
            last->lchild = new;

        // TODO balance from root
        balance_tree(t);
    }
}

int get_node_height(node *n)
{
    int left = 0, right = 0;
    if(n->lchild)
        left = get_node_height(n->lchild);
    if(n->rchild)
        right = get_node_height(n->rchild);
    if(left > right)
        return left + 1;
    else
        return right + 1;

}

int get_balance_factor(node *n)
{
    int bf = 0;
    if(n->lchild)
        bf += get_node_height(n->lchild);
    if(n->rchild)
        bf -= get_node_height(n->rchild);
    return bf;
}

void balance_tree(tree *t)
{
    node *b_root = NULL;
    b_root = balance_node(t->root);
    if(b_root != t->root)
        t->root = b_root;
}

node *balance_node(node *n)
{
    node *b_root = NULL;

    //balanced by botton-up
    if(n->lchild)
        n->lchild = balance_node(n->lchild);
    if(n->rchild)
        n->rchild = balance_node(n->rchild);

    int bf = get_balance_factor(n);
    if(bf >= 2) {
        if(get_balance_factor(n->lchild) <= -1)
            //LR rotate
            b_root = rotate_lr(n);
        else
            //LL rotate
            b_root = rotate_ll(n);
    } else if(bf <= -2) {
        if(get_balance_factor(n->rchild) >= 1)
            //RL rotate
            b_root = rotate_rl(n);
        else
            //RR rotate
            b_root = rotate_rr(n);
    } else {
        b_root = n;
    }

    return b_root;
}

void init_tab(tree *table[])
{
    for(int i=0; i<HASH_TAB_SIZE; i++) {
        table[i] = NULL;
    }
}

void drop_tab(tree *table[])
{
    for(int i=0; i<HASH_TAB_SIZE; i++)
        if(table[i] != NULL)
            drop_tree(table[i]);
}
void drop_tree(tree *t)
{
    free_node(t->root);
}

void free_node(node *n)
{
    if(n->lchild)
        free_node(n->lchild);
    if(n->rchild)
        free_node(n->rchild);
    free(n);
}