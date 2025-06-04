#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "processor_qry.h"
#include "file.h"
#include "form.h"
#include "form_state.h"
#include "form_style.h"
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
    int n;
    double x, y, w, h;
    int parsed = sscanf(line_buffer, "%*s %d %lf %lf %lf %lf", &n, &x, &y, &w, &h);
    if (parsed != 5) {
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
        bool region_exists = false;
        int region_count = selection_manager_get_region_count();
        
        for (int i = 0; i < region_count; i++) {
            if (!selection_manager_is_region_active(g_selection_manager, i)) continue;

            double existing_x, existing_y, existing_w, existing_h;
            selection_manager_get_region_data(g_selection_manager, i, &existing_x, &existing_y, &existing_w, &existing_h);
            
            if (existing_x == x && existing_y == y && existing_w == w && existing_h == h) {
                region_exists = true;
                break;
            }
        }
        
        if (!region_exists) {
            selection_manager_add_region(g_selection_manager, x, y, w, h);
        }
    }

    getInfosDentroRegiaoSmuT(t, x, y, x2, y2, &is_form_inside_region, selections_list[n]);
}

static void seli(SmuTreap t, char *line_buffer, List *selections_list) {
    int n; 
    double x, y; 
    int parsed = sscanf(line_buffer, "%*s %d %lf %lf", &n, &x, &y);
    if (parsed != 3) {
        fprintf(stderr, "ERROR: processor_qry seli command requires 3 parameters\n");
        return; 
    }

    if (n < 0 || n >= selection_manager_get_region_count()) {
        fprintf(stderr, "ERROR: processor_qry seli command requires a valid selection index (0-%d)\n", selection_manager_get_region_count()-1);
        return; 
    }

    Node found_node = getNodeSmuT(t, x, y); 
    if (found_node == NULL) {
        fprintf(stderr, "ERROR: processor_qry seli command did not find a form at the given coordinates (%lf/%lf)\n", x, y);
        return; 
    }

    if (selections_list[n] != NULL) {
        list_free(selections_list[n], NULL);
    }
    selections_list[n] = new_list();

    if (g_selection_manager != NULL) {
        selection_manager_set_region_data(g_selection_manager, n, x, y, 1, 1);
    }

    list_insert(selections_list[n], found_node);
}

struct cln_data {
    int max_id;
    SmuTreap tree;
    double target_x;
    double target_y;
};

static bool find_node_by_id(SmuTreap t, Node n, Info i, double x, double y, void *aux) {
    (void)x; 
    (void)y; 
    int id = get_form_id(getTypeInfoSmuT(t, n), i); 
    int *target_id = (int *) aux;
    if (id == *target_id) {
        return true; 
    }

    return false;
}

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
    FormInfo cloned_form_info = clone_form(form_type, form_info, ++data->max_id, data->target_x, data->target_y);
    if (cloned_form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry cln command failed to clone form\n");
        return;
    }
    
    Info cloned_form = get_form_info(cloned_form_info);
    insertSmuT(data->tree, data->target_x, data->target_y, cloned_form, form_type, &get_form_bounding_box);
    
    free_form_info_wrapper_only(cloned_form_info);
}

static void cln(SmuTreap t, char *line_buffer, List *selections_list) {
    int n;
    double x, y;
    int parsed = sscanf(line_buffer, "%*s %d %lf %lf",  &n, &x, &y);
    if (parsed != 3) {
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

    struct cln_data data = {.max_id = 0, .tree = t, .target_x = x, .target_y = y};
    list_foreach(selection, &get_max_info_id, &data); // lookup for the maximum id in the selection
    list_foreach(selection, &cln_helper, &data); // clone the forms from the given selection
}

static void transp(SmuTreap t, char *line_buffer) {
    int id; 
    double x, y;
    int parsed = sscanf(line_buffer, "%*s %d %lf %lf", &id, &x, &y);
    if (parsed != 3) {
        fprintf(stderr, "ERROR: processor_qry transp command requires 3 parameters\n");
        return; 
    }

    Node found_node = procuraNoSmuT(t, &find_node_by_id, &id);
    if (found_node == NULL) {
        fprintf(stderr, "ERROR: processor_qry transp command did not find a form with id %d\n", id);
        return; 
    }
    DescritorTipoInfo form_type = getTypeInfoSmuT(t, found_node);
    Info form_info = getInfoSmuT(t, found_node);
    if (form_type == -1 || form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry transp command requires valid form type and info\n");
        return; 
    }

    transp_form(form_type, form_info,x, y);
    set_form_state_camouflaged(form_info, true);
}

static void cmflg(SmuTreap t, char *line_buffer) {
    int id; 
    char cb[10] = {0}, cp[10] = {0}, w[10] = {0};
    int parsed = sscanf(line_buffer, "%*s %d %s %s %s", &id, cb, cp, w);
    if (parsed != 4) {
        fprintf(stderr, "ERROR: processor_qry cmflg command requires 3 parameters\n");
        return; 
    }
    Node found_node = procuraNoSmuT(t, &find_node_by_id, &id);
    if (found_node == NULL) {
        fprintf(stderr, "ERROR: processor_qry cmflg command did not find a form with id %d\n", id);
        return; 
    }
    DescritorTipoInfo form_type = getTypeInfoSmuT(t, found_node);
    Info form_info = getInfoSmuT(t, found_node);
    if (form_type == -1 || form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry cmflg command requires valid form type and info\n");
        return; 
    }

    FormStyle style = get_form_style(form_type, form_info);
    set_form_border_color(style, cb);
    set_form_fill_color(style, cp);
    set_form_style_stroke_width(style, w);
}

bool is_point_internal_to_form(SmuTreap t, Node n, Info i, double x, double y) {
    DescritorTipoInfo form_type = getTypeInfoSmuT(t, n);
    if (form_type == -1 || i == NULL) {
        fprintf(stderr, "ERROR: processor_qry is_point_internal_to_form requires valid form type and info\n");
        return false; 
    }

    double form_x, form_y, form_w, form_h;
    get_form_coordinates(form_type, i, &form_x, &form_y);
    get_form_dimensions(form_type, i, &form_w, &form_h);
    // if (form_type == LINE && ) 
    //     printf("after: %lf %lf\n", form_w, form_h);
    if (form_w <= 0 || form_h <= 0) {
        // fprintf(stderr, "ERROR: processor_qry is_point_internal_to_form requires valid form dimensions [%d](%lf, %lf)\n", form_type, form_w, form_h);
        // return false; 
    }

    bool is_internal = (x >= form_x && x <= form_x + form_w) && (y >= form_y && y <= form_y + form_h);
    return is_internal;
}

static void set_form_as_blown(void *value, callback_data call_data) {
    Node node = (Node) value;
    SmuTreap t = (SmuTreap) call_data;
    if (node == NULL || t == NULL) {
        fprintf(stderr, "ERROR: processor_qry set_form_as_blown requires valid node and treap\n");
        return; 
    }

    DescritorTipoInfo form_type = getTypeInfoSmuT(t, node);
    Info form_info = getInfoSmuT(t, node);
    if (form_type == -1 || form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry set_form_as_blown requires valid form type and info\n");
        return; 
    }

    FormState state = get_form_state(form_type, form_info);
    set_form_state_blown(state, true);
}

static void blow(SmuTreap t, char *line_buffer) {
    int id; 
    int parsed = sscanf(line_buffer, "%*s %d", &id);
    if (parsed != 1) {
        fprintf(stderr, "ERROR: processor_qry blow command requires 1 parameter\n");
        return; 
    }

    Node found_node = procuraNoSmuT(t, &find_node_by_id, &id);
    if (found_node == NULL) {
        fprintf(stderr, "ERROR: processor_qry blow command did not find a form with id %d\n", id);
        return; 
    }
    
    DescritorTipoInfo form_type = getTypeInfoSmuT(t, found_node);
    Info form_info = getInfoSmuT(t, found_node);
    if (form_type == -1 || form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry blow command requires valid form type and info\n");
        return; 
    }
    double x, y;
    get_form_coordinates(form_type, form_info, &x, &y);

    List hit_nodes = new_list();
    bool is_hit_nodes = getInfosAtingidoPontoSmuT(t, x, y, &is_point_internal_to_form, hit_nodes);
    if (is_hit_nodes) {
        list_foreach(hit_nodes, &set_form_as_blown, t);
    }

    FormState state = get_form_state(form_type, form_info);
    set_form_state_blown(state, true);
    list_free(hit_nodes, NULL);
}

static void calc_disp_final_point(double startX, double startY, double lineX1, double lineY1, 
    double lineX2, double lineY2, double distance, double *endX, double *endY) {

    double dx = lineX2 - lineX1;
    double dy = lineY2 - lineY1;

    double vector_length = sqrt(dx * dx + dy * dy);

    if (vector_length == 0.0) {
        *endX = startX;
        *endY = startY;
        return;
    }

    double movX = (dx / vector_length) * distance;
    double movY = (dy / vector_length) * distance;

    *endX = startX + movX;
    *endY = startY + movY;
}

struct disp_data {
    SmuTreap smu_treap;
    Info line_info;
    List hit_nodes;
    double disp_distance; 
};

void disp_selection(void *value, callback_data call_data) {
    Node node = (Node) value;
    struct disp_data *data = (struct disp_data *) call_data;

    DescritorTipoInfo form_type = getTypeInfoSmuT(data->smu_treap, node);
    Info form_info = getInfoSmuT(data->smu_treap, node);
    FormState state = get_form_state(form_type, form_info);
    if (form_type == -1 || form_info == NULL || state == NULL) {
        fprintf(stderr, "ERROR: processor_qry disp_selection requires valid form type and info\n");
        return; 
    }

    double x, y;
    get_form_coordinates(form_type, form_info, &x, &y);

    double lineX1, lineY1, lineX2, lineY2;
    get_form_coordinates(LINE, data->line_info, &lineX1, &lineY1);
    get_form_dimensions(LINE, data->line_info, &lineX2, &lineY2);

    double endX, endY;
    calc_disp_final_point(x, y, lineX1, lineY1, lineX2, lineY2, data->disp_distance, &endX, &endY);
    transp_form(form_type, form_info, endX, endY);
    set_form_state_blown(state, true);

    getInfosAtingidoPontoSmuT(data->smu_treap, endX, endY, &is_point_internal_to_form, data->hit_nodes);
}

void set_nodes_as_hit(void *value, callback_data call_data) {
    Node node = (Node) value;
    SmuTreap t = (SmuTreap) call_data;

    Info form = getInfoSmuT(t, node);
    DescritorTipoInfo form_type = getTypeInfoSmuT(t, node);
    FormState state = get_form_state(form_type, form);
    if (form == NULL || form_type == -1 || state == NULL) {
        fprintf(stderr, "ERROR: processor_qry set_nodes_as_hit requires valid form\n");
        return; 
    }

    set_form_state_blown(state, true);
}

void disp(SmuTreap t, char *line_buffer, List *selections_list) {
    int id, n; 
    int parsed = sscanf(line_buffer, "%*s %d %d", &id, &n);
    if (parsed != 2) {
        fprintf(stderr, "ERROR: processor_qry disp command requires 1 parameter\n");
        return; 
    }

    Node found_node = procuraNoSmuT(t, &find_node_by_id, &id);
    if (found_node == NULL) {
        fprintf(stderr, "ERROR: processor_qry disp command did not find a form with id %d\n", id);
        return; 
    }

    DescritorTipoInfo form_type = getTypeInfoSmuT(t, found_node);
    if (form_type != LINE) {
        fprintf(stderr, "ERROR: processor_qry disp command requires a LINE to direct the forms\n");
        return; 
    }

    Info form_info = getInfoSmuT(t, found_node);
    if (form_info == NULL) {
        fprintf(stderr, "ERROR: processor_qry disp command requires valid form info\n");
        return; 
    }

    if (n < 0 || n >= selection_manager_get_region_count()) {
        fprintf(stderr, "ERROR: processor_qry disp command requires a valid selection index (0-%d)\n", selection_manager_get_region_count()-1);
        return; 
    }
    
    if (selections_list[n] == NULL) {
        fprintf(stderr, "ERROR: processor_qry disp command requires a valid selection (selection %d is empty)\n", n);
        return;
    }
    
    List selection = selections_list[n];
    double distance = get_form_distance_disp(form_type, form_info);
    if (distance <= 0) {
        fprintf(stderr, "ERROR: processor_qry disp command requires a positive distance for the LINE form\n");
        return; 
    }

    List hit_nodes = new_list();
    struct disp_data data = {.smu_treap = t, .line_info = form_info, .disp_distance = distance, .hit_nodes = hit_nodes};

    list_foreach(selection, &disp_selection, &data);
    list_foreach(hit_nodes, &set_nodes_as_hit, t);
    list_free(hit_nodes, NULL);
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
        } else if (strcmp(command_type, "seli") == 0) {
            seli(smu_treap, line_buffer, selections_list);
        } else if (strcmp(command_type, "cln") == 0) {
            cln(smu_treap, line_buffer, selections_list);
        } else if (strcmp(command_type, "transp") == 0) {
            transp(smu_treap, line_buffer);
        } else if (strcmp(command_type, "cmflg") == 0) {
            cmflg(smu_treap, line_buffer);
        } else if (strcmp(command_type, "blow") == 0) {
            blow(smu_treap, line_buffer);
        } else if (strcmp(command_type, "disp") == 0) {
            disp(smu_treap, line_buffer, selections_list);
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