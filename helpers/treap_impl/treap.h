#ifndef TREAP_H
#define TREAP_H

typedef int TreapNodeValue;
extern const void *TreapNode;

/* 
 * Create a new node for the treap
 * @param `value` the value of the new node 
 * @return the created node
 */
void* new_treap_node(TreapNodeValue value);

/* 
 * Insert a node within the given root
 * @param `value` the value 
 * @return the root node
 */
void* treap_insert(void *treapNode, TreapNodeValue value);

/* 
 * Prints the treap values
 * @param root the treap root
 * @return void
 */
void treap_print(void *treapNode);

#endif