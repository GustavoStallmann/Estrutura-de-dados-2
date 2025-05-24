#ifndef FILE_H
#define FILE_H
#include <stdio.h>

/*
 * Open or create a new writable file on the desired path
 * @param path the output path for the file
 * @return FILE* the svg file reference
*/
FILE* file_open_writable(char *file_path); 


/*
 * Close the opened svg file
 * @param file the svg file
*/
void file_close(FILE *file); 

#endif