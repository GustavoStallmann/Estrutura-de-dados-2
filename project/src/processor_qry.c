#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "processor_qry.h"
#include "file.h"
#include "form.h"
#include "list.h"
#include "processor_form.h"
#include "smu_treap.h"
#include "form_selection.h"

#define MAX_LINE_LENGTH 512

static SelectionManager g_selection_manager = NULL; 

static bool process_command(char *line_buffer, char *command_type) {
    if (sscanf(line_buffer, "%9s", command_type) == 0) {
        if (line_buffer[0] != '\n' && line_buffer[0] != '\0') {
            fprintf(stderr, "ERROR: processor_qry unknown line type\n"); 
        } 
        return false;
    }; 

    return true; 
}

static void selr(SmuTreap t, char *line_buffer, List *selections_list) {
    char cmd[10];
    int n;
    double x, y, w, h;
    int parsed = sscanf(line_buffer, "%s %d %lf %lf %lf %lf", cmd, &n, &x, &y, &w, &h);
    if (parsed != 6) {
        fprintf(stderr, "ERROR: processor_qry selr command requires 5 parameters\n");
        return; 
    }

    if (n < 0 || n >= selection_manager_get_region_count()) {
        fprintf(stderr, "ERROR: processor_qry selr command requires a valid selection index (0-%d)\n", selection_manager_get_region_count()-1);
        return; 
    }

    if (selections_list[n] != NULL) {
        list_free(selections_list[n], NULL);
    }
    selections_list[n] = new_list();

    double x2 = x + w;
    double y2 = y + h;

    if (g_selection_manager != NULL) {
        selection_manager_set_region_data(g_selection_manager, n, x, y, w, h);
    }

    getInfosDentroRegiaoSmuT(t, x, y, x2, y2, &is_form_inside_region, selections_list[n]);
}

struct cln_data {
    int max_id;
    SmuTreap tree;
};

static void get_max_info_id(void *value, callback_data call_data) {
    struct cln_data *data = (struct cln_data *) call_data;
    Node node = (Node) value;

    if (data == NULL || node == NULL) {
        fprintf(stderr, "ERROR: processor_qry get_max_info_id requires valid data and node\n");
        return; 
    }
    int id = get_form_id(getTypeInfoSmuT(data->tree, node), getInfoSmuT(data->tree, node));
    if (id > (data->max_id)) {
        (data->max_id) = id;
    }
}

static void cln_helper(void *value, callback_data call_data) {
    struct cln_data *data = (struct cln_data *) call_data;
    Node node = (Node) value;
    if (data->tree == NULL || node == NULL) {
        fprintf(stderr, "ERROR: processor_qry cln command requires a valid treap and node\n");
        return; 
    }

    DescritorTipoInfo form_type = getTypeInfoSmuT(data->tree, node);
    Info form_info = getInfoSmuT(data->tree, node);
    if (form_type == -1 || form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry cln command requires valid form type and info\n");
        return; 
    }
  
    double x, y;
    get_form_coordinates(form_type, form_info, &x, &y);
    // insertSmuT(data->tree, x, y, , form_type, &get_form_bounding_box);
}

static void cln(SmuTreap t, char *line_buffer, List *selections_list) {
    char cmd[10];
    int n;
    double x, y;
    int parsed = sscanf(line_buffer, "%s %d %lf %lf", cmd, &n, &x, &y);
    if (parsed != 4) {
        fprintf(stderr, "ERROR: processor_qry cln command requires 3 parameters\n");
        return; 
    }
    
    if (n < 0 || n >= selection_manager_get_region_count()) {
        fprintf(stderr, "ERROR: processor_qry cln command requires a valid selection index (0-%d)\n", selection_manager_get_region_count()-1);
        return; 
    }
    
    if (selections_list[n] == NULL) {
        fprintf(stderr, "ERROR: processor_qry cln command requires a valid selection (selection %d is empty)\n", n);
        return;
    }
    
    List selection = selections_list[n];
    if (selection == NULL) {
        fprintf(stderr, "ERROR: processor_qry cln command requires a valid selection (selection %d is NULL)\n", n);
        return; 
    }
    
    struct cln_data data = {.max_id = 0, .tree = t};
    list_foreach(selection, &get_max_info_id, &data); // lookup for the maximum id in the selection
    list_foreach(selection, &cln_helper, &data); // clone the forms from the given selection
}

static void qry_execute(FILE *qry_file, SmuTreap smu_treap) {
    assert(qry_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 
    List selections_list[MAX_SELECTION_REGIONS]; 
    
    for (int i = 0; i < MAX_SELECTION_REGIONS; i++) {
        selections_list[i] = NULL;
    }

    while (fgets(line_buffer, sizeof(line_buffer), qry_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue; 

        if (strcmp(command_type, "selr") == 0) {
            selr(smu_treap, line_buffer, selections_list);
        } else if (strcmp(command_type, "cln") == 0) {
            cln(smu_treap, line_buffer, selections_list);
        } else {
            fprintf(stderr, "ERROR: processor_qry unknown command type '%s'\n", command_type);
        }
    }

    for (int i = 0; i < MAX_SELECTION_REGIONS; i++) {
        if (selections_list[i] == NULL) continue; 
        list_free(selections_list[i], NULL);
    }
}

void qry_process(Dir dir, SmuTreap smu_treap, SelectionManager selection_manager) {
    char *file_extension = get_dir_file_extension(dir);
    if (strcmp(file_extension, "qry") != 0) {
        fprintf(stderr, "ERROR: processor_qry requires a .qry file extension\n"); 
        return; 
    }

    FILE *qry_file = file_open_readable(dir);
    if (qry_file == NULL) {
        fprintf(stderr, "ERROR: processor_qry couldn't open the .qry file\n"); 
        return; 
    }

    g_selection_manager = selection_manager;
    if (selection_manager != NULL) {
        for (int i = 0; i < selection_manager_get_region_count(); i++) {
            selection_manager_set_region_active(g_selection_manager, i, false);
        }
    }

    qry_execute(qry_file, smu_treap);
    file_close(qry_file);
    
    g_selection_manager = NULL;
}