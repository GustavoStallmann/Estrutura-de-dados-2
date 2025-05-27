#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>

typedef enum {
    DIR, 
    STR, 
    DOUBLE,
    INT
} ArgType; 

typedef void *ArgManager;
typedef void *Arg; 


ArgManager new_arg_manager();
ArgManager add_new_arg(ArgManager argm, char *particle, bool mandatory, char *arg_description, ArgType argType, void *defaultValue);
ArgType get_arg_value_by_particle(ArgManager argm, char *particle, void *value) ; 
bool verify_args(ArgManager argm, int argc, char *argv[]);
void free_arg_manager(ArgManager argm);

#endif