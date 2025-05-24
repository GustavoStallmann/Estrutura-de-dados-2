#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "args.h"
#include "list.h"

typedef struct {
    char particle; 
    char *arg_description; 
    char *value; 
    bool mandatory; 
} Arg_st; 

typedef struct {
    List list; 
} ArgManager_st; 

static void alloc_error() {
    fprintf(stderr, "Error: insufficient memory for args allocation");
    exit(EXIT_FAILURE);     
}

static bool compare_list_search(ListValue value, void *target) {
    Arg_st *arg = (Arg_st *) value; 
    char *search_particle = (char *) target; 

    return arg->particle == *search_particle;
}

ArgManager new_arg_manager() {
    ArgManager_st *arg_manager = (ArgManager_st *) malloc(sizeof(ArgManager_st));
    if (arg_manager == NULL) alloc_error(); 

    arg_manager->list = new_list(); 

    return (ArgManager) arg_manager;
}

ArgManager add_new_arg(ArgManager argm, char particle, bool mandatory, char *arg_description) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm; 
    if (arg_manager == NULL) return NULL; 

    Arg_st *new_arg = (Arg_st *) malloc(sizeof(Arg_st));
    if (new_arg == NULL) alloc_error(); 
    new_arg->particle = particle;
    new_arg->mandatory = mandatory;
    new_arg->arg_description = arg_description;
    new_arg->value = NULL; 

    list_insert(arg_manager->list, (ListValue) new_arg); 

    return argm; 
}

char* get_arg_value(ArgManager argm, char particle) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm; 
    if (arg_manager == NULL) return NULL; 

    Arg_st *arg = (Arg_st *) list_search(arg_manager->list, (ListValue) &particle, &compare_list_search);
    if (arg == NULL) return NULL; 

    return arg->value; 
}

static void free_arg(ListValue a) {
    Arg_st *arg = (Arg_st *) a; 

    free(arg->arg_description); 
    free(arg->value); 
    free(arg); 
}

void free_arg_manager(ArgManager argm) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm; 
    if (arg_manager == NULL) return; 

    list_free(arg_manager->list, &free_arg); 
}