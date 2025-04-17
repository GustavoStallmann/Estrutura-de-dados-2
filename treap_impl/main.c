#include <stdio.h>
#include "treap.h"

int main( ) {
    TreapNode *root = new_node(10);
    treap_insert(root, 20);
    treap_insert(root, 30);
    treap_insert(root, 40);

    treap_print(root);
    return 0; 
}