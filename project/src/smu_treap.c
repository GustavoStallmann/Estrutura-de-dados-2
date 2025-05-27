#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "smu_treap.h"

typedef struct {
    struct Node *root;
    int hitCount, maxPriority;
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

//REMOVE LATER
static void print_tree(Node nd) {
    if (nd == NULL) return; 

    Node_st* node = (Node_st *) nd; 

    printf("(form -> %d; priority -> %lf)\n", node->formType, node->priority); 
    
    print_tree(node->left); 
    print_tree(node->right); 
}

static void alloc_error() { 
    fprintf(stderr, "Error: insufficient memory for smu_treap allocation");
    exit(EXIT_FAILURE); 
}

static inline int get_random_priority(int min, int max) {
    return rand() % (max - min + 1) + min;
}

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon, int prio) {
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
    new_node->maxPriority = prio; 

    return new_node; 
}

static Node newSmuTreapNode(Info form, DescritorTipoInfo formType, double x, double y, double priority) {
    Node_st* new_node = calloc(1, sizeof(Node_st)); 
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
static void joinBoundingBoxes(double *x1, double *y1, double *w1, double *h1, double *x2, double *y2, double *w2, double *h2) {

}

static Node rotate_left(Node nd) {
    Node_st* node = (Node_st *) nd; 
    if (node == NULL || node->right == NULL) return node;  

    Node_st *right_node = node->right; 
    Node_st *right_node_left = right_node->left; 

    right_node->left = node; 
    node->right = right_node_left; 
    return (Node) right_node;
}

static Node rotate_right(Node nd) {
    Node_st* node = (Node_st *) nd; 
    if (node == NULL || node->left == NULL) return node; 

    Node_st *left_node = node->left; 
    Node_st *left_node_right = left_node->right; 

    left_node->right = node;
    node->left = left_node_right;
    return (Node) left_node; 
}

static Node insertSmuTHelper(Node *r, Node i) {
    Node_st *root = (Node_st*) r; 
    Node_st *insertion_node = (Node_st *) i; 

    if (root == NULL) return insertion_node; 

    if (insertion_node->x <= root->x) { // left insertion
        root->left = insertSmuTHelper((Node) root->left, insertion_node); 

        if (root->left != NULL && ((Node_st*)root->left)->priority < root->priority) {
            return rotate_right((Node) root);
        }
    } else { // right insertion
        root->right = insertSmuTHelper((Node) root->right, insertion_node); 
        
        if (root->right != NULL && ((Node_st*)root->right)->priority < root->priority) {
            return rotate_left((Node) root); 
        }
    }

    return (Node) root; 
}

//TODO: CALC BB AS A LEAF THEN FIX IF ROTATION OCCURS
Node insertSmuT(SmuTreap t, double x, double y, Info form, DescritorTipoInfo formType, FCalculaBoundingBox fCalcBb) {
    assert(t); 
    
    SmuTreap_st *tree = (SmuTreap_st *) t; 
    Node_st *new_node = (Node_st *) newSmuTreapNode((Node) form, formType, x, y, get_random_priority(0, tree->maxPriority)); 
    fCalcBb(formType, new_node->form, &new_node->bb.x, &new_node->bb.y, &new_node->bb.w, &new_node->bb.h); 

    Node nd = insertSmuTHelper((Node) tree->root, new_node); 
    
    print_tree(tree->root);
    return nd; 
}

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h) {
    assert(n);

    Node_st *node = (Node_st *) n; 
    *x = node->bb.x; 
    *y = node->bb.y;
    *w = node->bb.w;
    *h = node->bb.h;

    return node->form;
}


// static void fixBoundingBoxSmuT(Node_st *node, Node_st *parent, FCalculaBoundingBox fCalcBb) {
//     if (node == NULL) return; 

//     fixBoundingBoxSmuT(node->left, node, fCalcBb); 
//     fixBoundingBoxSmuT(node->right, node, fCalcBb);

//     fCalcBb(node->formType, node->form, &node->bb.x, &node->bb.y, &node->bb.w, &node->bb.h);

//     // getBoundingBoxSmuT(NULL, (Node) node, &node->bb.x, &node->bb.y, &node->bb.w, &node->bb.h); 
// }
static void visitaProfundidadeSmuT_aux(SmuTreap t, Node nd, FvisitaNo f, void *aux) {
    if (nd == NULL) return; 

    Node_st *node = (Node_st *) nd; 
    
    f(t, nd, node->form, node->x, node->y, aux);
    visitaProfundidadeSmuT_aux(t, ((Node_st *) nd)->left, f, aux); 
    visitaProfundidadeSmuT_aux(t, ((Node_st *) nd)->right, f, aux); 
}

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void *aux) {
    assert(t); 

    SmuTreap_st *tree = (SmuTreap_st *) t; 
    visitaProfundidadeSmuT_aux(t, tree->root, f, aux);
}

static Node fixHeapProperty_aux(Node nd) {
    if (nd == NULL) return NULL;
    
    Node_st *node = (Node_st *) nd;
    
    node->left = fixHeapProperty_aux(node->left);
    node->right = fixHeapProperty_aux(node->right);
    
    if (node->left && ((Node_st*)node->left)->priority > node->priority) {
        return rotate_right(nd);
    }
    
    if (node->right && ((Node_st*)node->right)->priority > node->priority) {
        return rotate_left(nd);
    }
    
    return nd;
}

void promoteNodeSmuT(SmuTreap t, Node n, double promotionRate) {
    assert(t); 
    assert(n);

    SmuTreap_st *tree = (SmuTreap_st *) t; 
    Node_st *node = (Node_st *) n; 

    double new_priority = promotionRate * node->priority; 
    if (tree->maxPriority < new_priority) {
        new_priority = tree->maxPriority; 
    }

    node->priority = new_priority; 
    tree->root = fixHeapProperty_aux(tree->root);     
}
/*
 * Aumenta a prioridade do no' n pelo fator promotionRate.
 */

Info getInfoSmuT(SmuTreap t, Node n) {
    Node_st *node = (Node_st *) n;
    return node->form; 
}