#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "smu_treap.h"

typedef struct {
    struct Node *root;
    int hitCount;
    double promotionRate, epsilon; 
} SmuTreap_st;

typedef struct nd {
    double x, y; 
    double priority; 
    DescritorTipoInfo formType; 
    Info form; 
    struct nd *left, *right; 
    struct {
        double x, y, w, h; 
    } bb; 
} Node_st; 

static void alloc_error() { 
    fprintf(stderr, "ERRO: memória insuficiente para alocar smu_treap");
    exit(1); 
}

static double get_random_priority( ) {
    return rand() / (RAND_MAX + 1.0);
}

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon) {
    if (hitCount < 1) {
        fprintf(stderr, "smu_treap: hitCount cannot be lower than 1"); 
        return NULL; 
    }

    if (promotionRate < 0) {
        fprintf(stderr, "smu_treap: promotionRate cannot be lower than 0");
        return NULL; 
    }

    SmuTreap_st *new_node = (SmuTreap_st *) malloc(sizeof(SmuTreap_st));
    if (new_node == NULL) {
        alloc_error();
        return NULL;
    }

    new_node->root = NULL;
    new_node->hitCount = hitCount; 
    new_node->promotionRate = promotionRate; 
    new_node->epsilon = epsilon; 

    return new_node; 
}

static Node newSmuTreapNode(Info form, DescritorTipoInfo formType, double x, double y, double priority) {
    Node_st* new_node = malloc(sizeof(Node_st)); 
    if (new_node == NULL) {
        alloc_error( ); 
        return NULL; 
    }

    new_node->left = NULL; 
    new_node->right = NULL; 
    new_node->formType = formType; 
    new_node->form = form; 
    new_node->x = x; 
    new_node->y = y; 
    new_node->priority = priority; 

    return new_node; 
}

static Node rotate_left(Node *nd) {
    Node_st* node = (Node_st *) nd; 
    if (node == NULL || node->right == NULL) return node;  

    Node_st *right_node = node->right; 
    Node_st *right_node_left = right_node->left; 

    right_node->left = node; 
    node->right = right_node_left; 
    return right_node;
}


static Node rotate_right(Node *nd) {
    Node_st* node = (Node_st *) nd; 
    if (node == NULL || node->left == NULL) return node; 

    Node_st *left_node = node->left; 
    Node_st *left_node_right = left_node->right; 

    left_node->right = node;
    node->left = left_node_right;
    return left_node; 
}


static Node insertSmuTHelper(Node *r, Info form, DescritorTipoInfo formType, double x, double y, double priority) {
    Node_st *root = (Node_st*) r; 
    if (root == NULL) return newSmuTreapNode((Node) form, formType, x, y, priority); 

    if (root->x < x) {
        root->left = insertSmuTHelper((Node) root->left, form, formType, x, y, priority); 

        if (root->left->priority < root->priority) {
            Node root = (Node) root; 
            root = rotate_right(root);
        }
    } else if (root->x > x) {
        root->right = insertSmuTHelper((Node) root->right, form, formType, x, y, priority); 
        
        if (root->right->priority < root->priority) {
            root = rotate_left((Node) root); 
        }
    }

    return (Node) root; 
}

static void print_tree(Node nd) {
    if (nd == NULL) return; 

    Node_st* node = (Node_st *) nd; 

    printf("(form -> %d; priority -> %lf)\n", node->formType, node->priority); 
    
    print_tree(node->left); 
    print_tree(node->right); 
}

Node insertSmuT(SmuTreap t, double x, double y, Info form, DescritorTipoInfo formType, FCalculaBoundingBox fCalcBb) {
    assert(t); 

    SmuTreap_st *tree = (SmuTreap_st *) t; 
    Node nd = insertSmuTHelper((Node) tree->root, form, formType, x, y, get_random_priority()); 
    if (tree->root == NULL) {
        tree->root = nd; // New node as root
    }

    Node_st* inserted_node = (Node_st *) nd; 
    fCalcBb(formType, inserted_node->form, &inserted_node->bb.x, &inserted_node->bb.y, &inserted_node->bb.w, &inserted_node->bb.h); 

    print_tree(tree->root);
    return nd; 
}

Info getInfoSmuT(SmuTreap t, Node n) {
    Node_st *node = (Node_st *) n;
    return node->form; 
}

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h) {
    assert(t);
    assert(n);

    Node_st *node = (Node_st *) n; 
    *x = node->bb.x; 
    *y = node->bb.y;
    *w = node->bb.w;
    *h = node->bb.h;

    return node->form;
}