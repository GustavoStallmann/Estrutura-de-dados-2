#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "processor_args.h"
#include "file.h"
#include "file_svg.h"
#include "processor_form.h"
#include "smu_treap.h"
#include "form.h"
#include "processor_dir.h"
#include "processor_geo.h"
#include "processor_qry.h"

ArgManager check_args(int argc, char *argv[]); 
static void callback_insert_on_smu_treap(void *value, callback_data call_data); 
static void export_form_svg(SmuTreap t, Node n, Info info, double x, double y, void *aux);
static void combine_file_names(char *str1, char *str2, char *file_extension, char *result, int size);
static void export_svg_treap(SmuTreap smu_treap, char *base_output_dir, char *filename, SelectionManager selection_manager);
static void export_dot_treap(SmuTreap smu_treap, char *base_output_dir, char *geo_name, char *qry_name);

/*
TODOS: 
    - Valor do epsilon? 
    - Processar .qry 
    - Export de svg com atributos básicos (sem css)
    - Promover nós nas funções de consulta
    - Alinhar ponto de seleção com o centro do círculo
*/

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);

    // Gets dir related args
    char *base_input_dir, *base_output_dir, *geo_file, *qry_file;
    get_arg_value_by_particle(argm, "-e", &base_input_dir);
    get_arg_value_by_particle(argm, "-o", &base_output_dir);
    get_arg_value_by_particle(argm, "-f", &geo_file);
    get_arg_value_by_particle(argm, "-q", &qry_file);
    
    // Get args for the SMU treap
    int max_priority, hit_count; 
    double promotion_rate; 
    get_arg_value_by_particle(argm, "-p", &max_priority);
    get_arg_value_by_particle(argm, "-hc", &hit_count);
    get_arg_value_by_particle(argm, "-pr", &promotion_rate);

    // Process the geo file
    Dir geo_dir = dir_combine_path_and_file(base_input_dir, geo_file);
    List forms_list = geo_process(geo_dir);
    if (forms_list == NULL) {
        fprintf(stderr, "ERROR: main couldn't process the geo file\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize the SMU treap
    SmuTreap smu_treap = newSmuTreap(hit_count, promotion_rate, 0.5, max_priority);
    list_foreach(forms_list, &callback_insert_on_smu_treap, smu_treap);
    list_free(forms_list, &free_form_info_wrapper_only);
    
    // Export dot file after tree creation
    export_dot_treap(smu_treap, base_output_dir, get_dir_file_name(geo_dir), NULL);
    
    // Export the default SVG forms
    char svg_file_name[50];
    combine_file_names(get_dir_file_name(geo_dir), NULL, "svg", svg_file_name, sizeof(svg_file_name));
    export_svg_treap(smu_treap, base_output_dir, svg_file_name, NULL);

    // Process the qry file
    if (qry_file != NULL) {
        Dir qry_dir = dir_combine_path_and_file(base_input_dir, qry_file);
        
        // Criar um gerenciador de seleções para capturar as regiões
        SelectionManager selection_manager = selection_manager_create();
        qry_process(qry_dir, smu_treap, selection_manager);
        
        // Export dot file after qry processing
        export_dot_treap(smu_treap, base_output_dir, get_dir_file_name(geo_dir), get_dir_file_name(qry_dir));
        
        // Re-export SVG after qry processing with selection regions
        char svg_qry_file_name[50];
        combine_file_names(get_dir_file_name(geo_dir), get_dir_file_name(qry_dir), "svg", svg_qry_file_name, sizeof(svg_qry_file_name));
        export_svg_treap(smu_treap, base_output_dir, svg_qry_file_name, selection_manager);
        
        selection_manager_destroy(selection_manager);
        dir_free(qry_dir);
    } else {
        fprintf(stderr, "WARNING: No query file provided, skipping query processing.\n");
    }
    
    dir_free(geo_dir);
    killSmuTreap(smu_treap); 
    free_arg_manager(argm);
    return 0;
}

static void export_svg_treap(SmuTreap smu_treap, char *base_output_dir, char *filename, SelectionManager selection_manager) {
    Dir svg_dir = dir_combine_path_and_file(base_output_dir, filename);
    FILE *svg_file = file_open_writable(svg_dir);
    dir_free(svg_dir);
    
    svg_init(svg_file, 1000, 1000);
    visitaProfundidadeSmuT(smu_treap, &export_form_svg, svg_file);
    
    // Exportar as regiões de seleção se fornecidas
    if (selection_manager != NULL) {
        svg_export_selection_regions(svg_file, selection_manager);
    }
    
    svg_close(svg_file);
    file_close(svg_file);
}

static void combine_file_names(char *str1, char *str2, char *file_extension, char *result, int size) {
    if (str2 != NULL) {
        snprintf(result, size, "%s-%s.%s", str1, str2, file_extension);
        return; 
    }
    snprintf(result, size, "%s.%s", str1, file_extension);
}

ArgManager check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager();
    add_new_arg(argm, "-e", false, "Diretório-base de entrada (BED)", STR, "/"); 
    add_new_arg(argm, "-q", false, "Arquivo com consultas (QRY)", STR, NULL); 
    add_new_arg(argm, "-p", false, "Prioridade máxima (número inteiro)", INT, "10000"); 
    add_new_arg(argm, "-hc", false, "Parâmetro hit count usado na árvore (número inteiro)", INT, "3"); 
    add_new_arg(argm, "-pr", false, "Fator de promoção a ser usado quando hit count é atingido (número decimal)", DOUBLE, "1.10"); 
    add_new_arg(argm, "-o", true, "Diretório-base de saída", STR, "/"); 
    add_new_arg(argm, "-f", true, "Arquivo com a descrição da treap (GEO)", STR, NULL); 
    
    bool status = verify_args(argm, argc, argv); 
    if (!status) {
        free_arg_manager(argm);
        exit(EXIT_FAILURE);
    }

    return argm; 
}

static void callback_insert_on_smu_treap(void *value, callback_data call_data) {
    SmuTreap smu_treap = (SmuTreap) call_data;
    FormInfo form_info = (FormInfo) value;
    
    DescritorTipoInfo form_type = get_form_type(form_info);
    Info form = get_form_info(form_info);

    double x, y; 
    get_form_coordinates(form_type, form, &x, &y);

    insertSmuT(smu_treap, x, y, form, form_type, &get_form_bounding_box);
}

static void export_form_svg(SmuTreap t, Node n, Info info, double x, double y, void *aux) {
    (void)x; //ignore unused args
    (void)y; 
    (void)t; 
    (void)n; 
    
    FILE *svg_file = (FILE *) aux;
    DescritorTipoInfo info_type = getTypeInfoSmuT(t, n);

    svg_export_form(svg_file, info, info_type);
}

static void export_dot_treap(SmuTreap smu_treap, char *base_output_dir, char *geo_name, char *qry_name) {
    char dot_file_name[50], dot_full_path[50];
    combine_file_names(geo_name, qry_name, "dot", dot_file_name, sizeof(dot_file_name));
    
    Dir dot_dir = dir_combine_path_and_file(base_output_dir, dot_file_name);
    get_full_dir(dot_dir, dot_full_path);
    printDotSmuTreap(smu_treap, dot_full_path);
    dir_free(dot_dir);
}