#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define HASH_AVL_OPT 1
#define HASH_TAB_SIZE 524288

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
// #define OPT 1
typedef struct __PHONE_BOOK_DETAIL {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} phonebook_detail;

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    phonebook_detail *detail;
    struct __PHONE_BOOK_ENTRY *lchild;
    struct __PHONE_BOOK_ENTRY *rchild;
} node;

typedef struct __AVL_TREE {
    struct __PHONE_BOOK_ENTRY *root;
} tree;

node *findName(char lastName[], tree *table[]);
void *append(char lastName[], tree *table[]);

//hash function
unsigned int BKDRHash(char *str);
//rotate function
node* rotate_rr(node *n);
node* rotate_ll(node *n);
node* retate_rl(node *n);
node* rotate_lr(node *n);

//avl-tree's building function
tree* create_avl();
node* create_node();
void insert_avl(tree *t, char lastName[]);
int get_node_height(node *n);
int get_balance_factor(node *n);
void balance_tree(tree *t);
node* balance_node(node *n);

void init_tab(tree *table[]);
void drop_tab(tree *table[]);
void drop_tree(tree *t);
void free_node(node *n);

#endif
