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

/*
 * "Eu" passo a ser filho direito do meu filho da esquerda, 
 * e tomo os filhos da direita dele como meus filhos da esquerda. 
 * (viro direito do da esquerda e pego os filhos)
*/
static TreapNode* rotate_right(TreapNode *node) {
    if (node == NULL || node->left == NULL) return node; 
    TreapNode *left_node = node->left; 
    TreapNode *left_node_right = left_node->right; 

    left_node->right = node;
    node->left = left_node_right;
    return left_node; 
}

/* 
 * "Eu" passo a ser filho esquerdo do meu filho da direita, 
 * e tomo os filhos da esquerda dele como meus filhos da direita. 
 * (viro esquerdo do da direita e pego os filhos)
*/
static TreapNode* rotate_left(TreapNode *node) {
    if (node == NULL || node->right == NULL) return node;  
    TreapNode *right_node = node->right; 
    TreapNode *right_node_left = right_node->left; 

    right_node->left = node; 
    node->right = right_node_left; 
    return right_node;
}

void* new_treap_node(TreapNodeValue value) {
    TreapNode* node = (TreapNode*) malloc(sizeof(TreapNode));
    if (node == NULL) alloc_error( ); 

    node->key = value;
    node->left = NULL; 
    node->right = NULL;
    node->priority = rand( ) % 100;
    return node;
}

void* treap_insert(void *treapNode, TreapNodeValue value) {
    TreapNode *root = (TreapNode *) treapNode; 
    
    if (root == NULL) return new_treap_node(value);

    if (root->key < value) {
        root->left = treap_insert(root->left, value);

        if (root->left->priority < root->priority) {
            root = rotate_right(root);
        }
    } else if (root->key > value) {
        root->right = treap_insert(root->right, value);

        if (root->right->priority < root->priority) {
            root = rotate_left(root);
        }
    }

    return root;
}

void treap_print(TreapNode *root) {
    if (root == NULL) return;

    treap_print(root->left);

    printf("%d ", root->key);

    treap_print(root->right);
}
