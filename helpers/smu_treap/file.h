#ifndef FILE_H
#define FILE_H
#include <stdio.h>

/*
 * Create a new svg file on the desired path
 * @param path the output path for the file
 * @return FILE* the svg file reference
*/
FILE* file_open(char *path, char *file_name); 


/*
 * Close the opened svg file
 * @param file the svg file
*/
void file_close(FILE *file); 

#endif