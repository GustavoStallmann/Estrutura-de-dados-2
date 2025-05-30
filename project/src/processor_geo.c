#include <stdio.h>
#include <string.h>
#include "file.h"
#include "processor_form.h"

#define MAX_LINE_LENGTH 512 

static List geo_execute(FILE *geo_file) {
    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 

    List form_list = new_list();
    if (form_list == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't create a new list"); 
        return NULL; 
    }

    while (fgets(line_buffer, sizeof(line_buffer), geo_file) != NULL) {
        if (sscanf(line_buffer, "%9s", command_type) == 0) {
            if (line_buffer[0] != '\n' && line_buffer[0] != '\0') { // lines's empty
                fprintf(stderr, "ERROR: processor_geo unknown line type: %s", line_buffer); 
            }
            continue; 
        }; 

        FormInfo form = process_form(command_type, line_buffer); 
        if (form == NULL) {
            fprintf(stderr, "ERROR: processor_geo couldn't process the form: %s", line_buffer); 
            continue; 
        }

        if (list_insert(form_list, form) == NULL) {
            fprintf(stderr, "ERROR: processor_geo couldn't add the form to the list: %s", line_buffer); 
            free_form_info(form);
            continue; 
        }	
    }

    return form_list;
}

List geo_process(Dir dir) {
    char *file_extension = get_file_extension(dir);
    if (strcmp(file_extension, "geo") != 0) {
        printf("file ext: [%s]\n", file_extension);
        fprintf(stderr, "ERROR: processor_geo requires a .geo file extension"); 
        return NULL; 
    }

    FILE *geo_file = file_open_readable(dir);
    if (geo_file == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't open the geo file"); 
        return NULL; 
    }

    List form_list = geo_execute(geo_file);     
    file_close(geo_file);
    return form_list; 
}