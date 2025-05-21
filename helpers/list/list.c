#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct st_linode_st {
    ListValue value; 
    struct st_linode_st *next; 
} ListNode;

typedef struct st_list {
    ListNode *head; 
    int size; 
} List;

static void alloc_error() {
    perror("Erro: memória insuficiente para alocação da lista");
    exit(1);     
}
 
List* new_list() {
    List* new_list = (List *) malloc(sizeof(List));
    if (new_list == NULL) alloc_error( ); 

    new_list->head = NULL; 

    return new_list; 
}

static void aux_insert(ListNode *node, ListValue value) {
    ListNode* new_node = (ListNode*) malloc(sizeof(ListNode));
    if (new_node == NULL) alloc_error( );

    return new_node; 
}

List* list_insert(List *list, ListValue value) {
    aux_insert(list->head, value); 
    return list;
}

int main(int argc, char const *argv[])
{
    
    return 0;
}
