#include <assert.h>
#include <stdio.h>
#include "file.h"
#include <string.h>

FILE* file_open_writable(char *file_path) {
    assert(file_path); 

    FILE *file = fopen(file_path, "w"); 
    assert(file); 

    if (file == NULL) {
        fprintf(stderr, "file: couldn't open the file\n"); 
        return NULL; 
    }

    return file; 
}

FILE* file_open_readable(char *path, char *file_name) {
    assert(path); 
    assert(file_name); 

    char* file_dir = strcat(path, file_name);
    FILE *file = fopen(file_dir, "r"); 

    if (file == NULL) {
        fprintf(stderr, "file: couldn't open the file\n"); 
        return NULL; 
    }

    return file; 
}

void file_close(FILE *file) {
    assert(file);
    if (file == NULL) return; 

    fclose(file);
}