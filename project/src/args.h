#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>

typedef void *ArgManager;
typedef void *Arg; 

ArgManager new_arg_manager();
ArgManager add_new_arg(ArgManager argm, char particle, bool mandatory, char *arg_description);
void free_arg_manager(ArgManager argm);

#endif