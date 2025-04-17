#ifndef TREAP_H
#define TREAP_H

typedef int TreapNodeValue;
typedef struct TreapNode TreapNode;

/* 
 * Create a new node for the treap
 * @param `value` the value of the new node 
 * @return the created node
 */
TreapNode* new_node(TreapNodeValue value);

/* 
 * Insert a node within the given root
 * @param `value` the value 
 * @return the root node
 */
TreapNode* treap_insert(TreapNode *root, TreapNodeValue value);

/* 
 * Prints the treap values
 * @param root the treap root
 * @return void
 */
void treap_print(TreapNode *root);

#endif