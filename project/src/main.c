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

ArgManager check_args(int argc, char *argv[]); 
static void callback_insert_on_smu_treap(void *value, callback_data call_data); 
static void export_form_svg(SmuTreap t, Node n, Info info, double x, double y, void *aux);

/*
TODOS: 
    - Valor do epsilon? 
    - Processar .qry 
    - Export .dot
*/

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);

    char *base_input_dir;
    get_arg_value_by_particle(argm, "-e", &base_input_dir);

    char *base_output_dir;
    get_arg_value_by_particle(argm, "-o", &base_output_dir);

    char *geo_file; 
    get_arg_value_by_particle(argm, "-f", &geo_file);

    Dir geo_dir = dir_combine_path_and_file(base_input_dir, geo_file);
    List forms_list = geo_process(geo_dir);
    
    if (forms_list == NULL) {
        fprintf(stderr, "ERROR: main couldn't process the geo file\n");
        exit(EXIT_FAILURE);
    }
    
    // Get args for the SMU treap
    int max_priority; 
    int hit_count; 
    double promotion_rate; 
    get_arg_value_by_particle(argm, "-p", &max_priority);
    get_arg_value_by_particle(argm, "-hc", &hit_count);
    get_arg_value_by_particle(argm, "-pr", &promotion_rate);
    
    // Initialize the SMU treap
    SmuTreap smu_treap = newSmuTreap(hit_count, promotion_rate, 0.5, max_priority);
    list_foreach(forms_list, &callback_insert_on_smu_treap, smu_treap);
    list_free(forms_list, &free_form_info_wrapper_only);

    // Export the default SVG forms
    Dir svg_dir = dir_combine_path_and_file(base_output_dir, "arq.svg");
    FILE *svg_file = file_open_writable(svg_dir); 
    svg_init(svg_file, 1280, 720);
    visitaProfundidadeSmuT(smu_treap, &export_form_svg, svg_file);
    svg_close(svg_file);
    file_close(svg_file);
    dir_free(svg_dir);
    
    killSmuTreap(smu_treap); 
    free_arg_manager(argm);
    dir_free(geo_dir);
    
    return 0;
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
    };

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