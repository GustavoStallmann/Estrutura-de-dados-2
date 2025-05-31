#ifndef PROCESSOR_DIR 
#define PROCESSOR_DIR

#include <stdbool.h>

typedef void *Dir; 
typedef char *Str; 

Dir new_dir(char *full_dir);
Dir dir_combine_path_and_file(char *path, char *file);
void get_full_dir(Dir d, char *path);
char* get_dir_path(Dir d);
char* get_dir_file_name(Dir d);
char* get_dir_file_extension(Dir d);
bool dir_file_exists(Dir d); 
void dir_free(Dir d); 

#endif