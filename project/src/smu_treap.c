#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "smu_treap.h"
#include "form.h"

typedef struct {
    struct Node *root;
    int hitCount, maxPriority;
    double promotionRate, epsilon; 
} SmuTreap_st;

typedef struct {
    double x, y, w, h; 
} BoundingBox; 

typedef struct nd {
    double x, y; 
    double priority; 
    DescritorTipoInfo formType; 
    Info form; 
    BoundingBox form_bb; 
    BoundingBox sub_bb; 
    struct nd *left, *right; 
} Node_st; 

//REMOVE LATER
static void print_tree_aux(Node nd, int depth, char* prefix, int is_last) {
    if (nd == NULL) {
        return;
    }
    
    Node_st* node = (Node_st *) nd;
    
    // Print current node with tree structure
    printf("%s%s", prefix, is_last ? "└── " : "├── ");
    printf("Node[%p]: form=%d, prio=%.2f, pos=(%.2f,%.2f)\n", 
           (void*)node, node->formType, node->priority, node->x, node->y);
    
    // Print bounding box info with indentation
    printf("%s%s    BB: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", 
           prefix, is_last ? "    " : "│   ",
           node->sub_bb.x, node->sub_bb.y, node->sub_bb.w, node->sub_bb.h);
    
    // Print children info
    printf("%s%s    Children: L=%s, R=%s\n", 
           prefix, is_last ? "    " : "│   ",
           node->left ? "Yes" : "NULL", 
           node->right ? "Yes" : "NULL");
    
    // Prepare prefix for children
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");
    
    // Print children
    if (node->left || node->right) {
        if (node->left) {
            print_tree_aux(node->left, depth + 1, new_prefix, node->right == NULL);
        }
        if (node->right) {
            print_tree_aux(node->right, depth + 1, new_prefix, 1);
        }
    }
}

static void print_tree(Node nd) {
    if (nd == NULL) {
        printf("Tree is empty (NULL)\n");
        return;
    }
    
    printf("\n=== SMU TREAP STRUCTURE ===\n");
    print_tree_aux(nd, 0, "", 1);
    printf("=========================\n\n");
}

static void alloc_error() { 
    fprintf(stderr, "(ERROR) smu_treap: insufficient memory for smu_treap allocation");
    exit(EXIT_FAILURE); 
}

static inline int get_random_priority(int min, int max) {
    return rand() % (max - min + 1) + min;
}

SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon, int prio) {
    if (hitCount < 1) {
        fprintf(stderr, "(ERROR) smu_treap: hitCount cannot be lower than 1"); 
        return NULL; 
    }

    if (promotionRate < 0) {
        fprintf(stderr, "(ERROR) smu_treap: promotionRate cannot be lower than 0");
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

static void joinBoundingBoxes(BoundingBox *first_bb, BoundingBox *second_bb, double *x, double *y, double *w, double *h) {
    if (first_bb == NULL || second_bb == NULL) return;
    
    double min_x = first_bb->x < second_bb->x ? first_bb->x : second_bb->x;
    double min_y = first_bb->y < second_bb->y ? first_bb->y : second_bb->y;
    
    double max_x = (first_bb->x + first_bb->w) > (second_bb->x + second_bb->w) ? 
                   (first_bb->x + first_bb->w) : (second_bb->x + second_bb->w);
    double max_y = (first_bb->y + first_bb->h) > (second_bb->y + second_bb->h) ? 
                   (first_bb->y + first_bb->h) : (second_bb->y + second_bb->h);
    
    *x = min_x;
    *y = min_y;
    *w = max_x - min_x;
    *h = max_y - min_y;
}

static Node rotate_left(Node nd) {
    Node_st *node = (Node_st *) nd; 
    if (node == NULL || node->right == NULL) return node;  

    Node_st *right_node = node->right; 
    Node_st *right_node_left = right_node->left; 

    right_node->left = node; 
    node->right = right_node_left; 

    // Recalcula a bounding box do nó não promovido (agora filho esquerdo)
    // Inicia com a bounding box da forma do próprio nó
    node->sub_bb = node->form_bb;
    if (node->left != NULL)
        joinBoundingBoxes(&node->sub_bb, &node->left->sub_bb, &node->sub_bb.x, &node->sub_bb.y, &node->sub_bb.w, &node->sub_bb.h);
    if (node->right != NULL)
        joinBoundingBoxes(&node->sub_bb, &node->right->sub_bb, &node->sub_bb.x, &node->sub_bb.y, &node->sub_bb.w, &node->sub_bb.h);

    // Recalcula a bounding box do nó promovido (agora raiz)
    // Inicia com a bounding box da forma do próprio nó
    right_node->sub_bb = right_node->form_bb;
    if (right_node->left != NULL)
        joinBoundingBoxes(&right_node->sub_bb, &right_node->left->sub_bb, &right_node->sub_bb.x, &right_node->sub_bb.y, &right_node->sub_bb.w, &right_node->sub_bb.h);
    if (right_node->right != NULL)
        joinBoundingBoxes(&right_node->sub_bb, &right_node->right->sub_bb, &right_node->sub_bb.x, &right_node->sub_bb.y, &right_node->sub_bb.w, &right_node->sub_bb.h);

    return (Node) right_node;
}

static Node rotate_right(Node nd) {
    Node_st* node = (Node_st *) nd; 
    if (node == NULL || node->left == NULL) return node; 

    Node_st *left_node = node->left; 
    Node_st *left_node_right = left_node->right; 

    // Primeiro faz a rotação física
    left_node->right = node;
    node->left = left_node_right;

    // Recalcula BB do node (agora filho direito)
    // Inicia com a bounding box da forma do próprio nó
    node->sub_bb = node->form_bb;
    if (node->left != NULL) 
        joinBoundingBoxes(&node->sub_bb, &node->left->sub_bb, &node->sub_bb.x, &node->sub_bb.y, &node->sub_bb.w, &node->sub_bb.h);
    if (node->right != NULL) 
        joinBoundingBoxes(&node->sub_bb, &node->right->sub_bb, &node->sub_bb.x, &node->sub_bb.y, &node->sub_bb.w, &node->sub_bb.h);

    // Recalcula BB do left_node (agora raiz)
    // Inicia com a bounding box da forma do próprio nó
    left_node->sub_bb = left_node->form_bb;
    if (left_node->left != NULL) 
        joinBoundingBoxes(&left_node->sub_bb, &left_node->left->sub_bb, &left_node->sub_bb.x, &left_node->sub_bb.y, &left_node->sub_bb.w, &left_node->sub_bb.h);
    if (left_node->right != NULL) 
        joinBoundingBoxes(&left_node->sub_bb, &left_node->right->sub_bb, &left_node->sub_bb.x, &left_node->sub_bb.y, &left_node->sub_bb.w, &left_node->sub_bb.h);

    return (Node) left_node; 
}

static Node insertSmuT_aux(Node r, Node i) {
    Node_st *root = (Node_st*) r; 
    Node_st *insertion_node = (Node_st *) i; 

    if (root == NULL) return insertion_node; 

    if (insertion_node->x <= root->x) { // left insertion
        root->left = insertSmuT_aux((Node) root->left, insertion_node); 

        if (root->left != NULL && ((Node_st*)root->left)->priority > root->priority) {
            return rotate_right((Node) root);
        }
    } else { // right insertion
        root->right = insertSmuT_aux((Node) root->right, insertion_node); 
        
        if (root->right != NULL && ((Node_st*)root->right)->priority > root->priority) {
            return rotate_left((Node) root); 
        }
    }

    // Recalcula a bounding box do nó atual incluindo seus filhos
    // Inicia com a bounding box da forma do próprio nó
    root->sub_bb = root->form_bb;
    
    if (root->left != NULL) {
        joinBoundingBoxes(&root->sub_bb, &root->left->sub_bb, &root->sub_bb.x, &root->sub_bb.y, &root->sub_bb.w, &root->sub_bb.h);
    }
    if (root->right != NULL) {
        joinBoundingBoxes(&root->sub_bb, &root->right->sub_bb, &root->sub_bb.x, &root->sub_bb.y, &root->sub_bb.w, &root->sub_bb.h);
    }
    return (Node) root;
}

Node insertSmuT(SmuTreap t, double x, double y, Info form, DescritorTipoInfo formType, FCalculaBoundingBox fCalcBb) {
    assert(t); 
    
    SmuTreap_st *tree = (SmuTreap_st *) t; 
    Node_st *new_node = (Node_st *) newSmuTreapNode((Node) form, formType, x, y, get_random_priority(0, tree->maxPriority)); 
    fCalcBb(formType, new_node->form, &new_node->form_bb.x, &new_node->form_bb.y, &new_node->form_bb.w, &new_node->form_bb.h); 
    new_node->sub_bb = new_node->form_bb; 

    tree->root = insertSmuT_aux((Node) tree->root, new_node); 
    
    print_tree(tree->root);
    return tree->root; 
} 

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h) {
    assert(n);

    Node_st *node = (Node_st *) n; 
    *x = node->sub_bb.x; 
    *y = node->sub_bb.y;
    *w = node->sub_bb.w;
    *h = node->sub_bb.h;

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

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Node n) {
    (void)t; // ignore arg
    assert(n);

    Node_st *node = (Node_st *) n; 
    return node->formType; 
}

Info getInfoSmuT(SmuTreap t, Node n) {
    (void)t; // ignore arg
    assert(n);

    Node_st *node = (Node_st *) n; 
    return node->form; 
}

static void killSmuTreap_aux(SmuTreap t, Node n, Info i, double x, double y, void *aux) {
    (void)x; // unused parameter
    (void)y; // unused parameter
    (void)aux; // unused parameter
    
    DescritorTipoInfo formType = getTypeInfoSmuT(t, n);
    free_form(formType, i);
}

static void killSmuTreap_nodes_aux(Node nd) {
    if (nd == NULL) return;
    
    Node_st *node = (Node_st *) nd;
    
    killSmuTreap_nodes_aux(node->left);
    killSmuTreap_nodes_aux(node->right);
    free(node);
}

void killSmuTreap(SmuTreap t) {
    assert(t); 
    SmuTreap_st *tree = (SmuTreap_st *) t;
    
    visitaProfundidadeSmuT(t, &killSmuTreap_aux, NULL); //kill the forms
    killSmuTreap_nodes_aux(tree->root); //kill nodes
    free(t); //kill treap
}

// static Node fixHeapProperty_aux(Node nd) {
//     if (nd == NULL) return NULL;
    
//     Node_st *node = (Node_st *) nd;
    
//     node->left = fixHeapProperty_aux(node->left);
//     node->right = fixHeapProperty_aux(node->right);
    
//     if (node->left && ((Node_st*)node->left)->priority > node->priority) {
//         return rotate_right(nd);
//     }
    
//     if (node->right && ((Node_st*)node->right)->priority > node->priority) {
//         return rotate_left(nd);
//     }
    
//     return nd;
// }

// void promoteNodeSmuT(SmuTreap t, Node n, double promotionRate) {
//     assert(t); 
//     assert(n);

//     SmuTreap_st *tree = (SmuTreap_st *) t; 
//     Node_st *node = (Node_st *) n; 

//     double new_priority = promotionRate * node->priority; 
//     if (tree->maxPriority < new_priority) {
//         new_priority = tree->maxPriority; 
//     }

//     node->priority = new_priority; 
//     tree->root = fixHeapProperty_aux(tree->root);     
// }
/*
 * Aumenta a prioridade do no' n pelo fator promotionRate.
 */