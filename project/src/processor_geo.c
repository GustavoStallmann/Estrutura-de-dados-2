#include <stdio.h>
#include <string.h>
#include "file.h"
#include "processor_dir.h"

#define MAX_LINE_LENGTH 512 

static void geo_execute(FILE *geo_file) {
    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 
    while (fgets(line_buffer, sizeof(line_buffer), geo_file) != NULL) {
        if (sscanf(line_buffer, "%9s", command_type) != 0) {
            if (line_buffer[0] != '\n' && line_buffer[0] != '\0') { // lines's empty
                fprintf(stderr, "ERROR: processor_geo unknown line type: %s", line_buffer); 
            }
            continue; 
        }; 

        printf("%s", command_type);
    }
}

void geo_process(Dir dir) {
    char *file_extension = get_file_extension(dir);
    if (!strcmp(file_extension, "geo")) {
        fprintf(stderr, "ERROR: processor_geo requires a .geo file extension"); 
        return; 
    }

    FILE *geo_file = file_open_readable(dir);
    if (geo_file == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't open the geo file"); 
        return; 
    }

    geo_execute(geo_file);     
}