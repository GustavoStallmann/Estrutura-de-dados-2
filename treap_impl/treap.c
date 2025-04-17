#include "treap.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct TreapNode {
    TreapNodeValue key;
    float priority;
    struct TreapNode *left, *right;
} TreapNode;

static void alloc_error( ) {
    printf("ERROR: memória insuficiente.");
    exit(1);
}

static TreapNode* rotate_right(TreapNode *node) {
    if (node == NULL || node->left == NULL) return node; 
    TreapNode *left_node = node->left; 
    TreapNode *left_node_right = left_node->right; 

    left_node->right = node;
    node->left = left_node_right;
    return left_node; 
}

static TreapNode* rotate_left(TreapNode *node) {
    if (node == NULL || node->right == NULL) return node;  
    TreapNode *right_node = node->right; 
    TreapNode *right_node_left = right_node->left; 

    right_node->right = node; 
    node->right = right_node_left; 
    return right_node;
}

TreapNode* new_node(TreapNodeValue value) {
    TreapNode* node = (TreapNode*) malloc(sizeof(TreapNode));
    if (node == NULL) alloc_error( ); 

    node->key = value;
    node->left = NULL; 
    node->right = NULL;
    node->priority = rand( ) % 100;
    return node;
}

// TODO: fix and understand the insertion logic
TreapNode* treap_insert(TreapNode *root, TreapNodeValue value) {
    if (root == NULL) return new_node(value);

    if (root->key < value) {
        root->left = treap_insert(root->left, value);

        if (root->left->priority < root->priority) {
            root = rotate_right(root);
        }
    } else if (root->key > value) {
        root->right = treap_insert(root->right, value);

        if (root->right->priority < root->priority) {
            root = rotate_right(root);
        }
    }

    return root;
}

void treap_print(TreapNode *root) {
    if (root == NULL) return;

    // Print left subtree
    treap_print(root->left);

    // Print current node
    printf("%d ", root->key);

    // Print right subtree
    treap_print(root->right);
}
