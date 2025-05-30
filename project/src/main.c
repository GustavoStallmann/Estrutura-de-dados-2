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
    - Funções de free das formas
    - Leitura do arquivo de entrada (GEO)
    - Funções de export das formas SVG
*/

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);

    int max_priority; 
    get_arg_value_by_particle(argm, "-p", &max_priority);

    Dir geo_dir = new_dir("input/00-vaso-simples.geo");
    List forms_list = geo_process(geo_dir);
    if (forms_list == NULL) {
        fprintf(stderr, "ERROR: main couldn't process the geo file\n");
        exit(EXIT_FAILURE);
    }

    Dir svg_dir = new_dir("output/test.svg");
    FILE *svg_file = file_open_writable(svg_dir); 
    svg_init(svg_file, 800, 600);

    SmuTreap smu_treap = newSmuTreap(2, 1.1, 0.5, max_priority);
    list_foreach(forms_list, &callback_insert_on_smu_treap, smu_treap);
    visitaProfundidadeSmuT(smu_treap, &export_form_svg, svg_file);

    // // IF ARGS NOT INFORMED SEG FAULTS
    // // Dir input_dir; 
    // // get_arg_value_by_particle(argm, "-e", &input_dir);
    // // printf("dir: %s", get_full_dir(input_dir)); 
    
    // for (int i = 0; i < 10; i++) {
    //     double r = 10; 
    //     double formc_x = i * 50 + r, formc_y = i * 50 + r; 
    //     FormStyle style = new_form_style("#696969", "#FF0000", NULL, NULL); 
    //     Circle formC = new_circle(i, formc_x, formc_y, r, style); 
    //     insertSmuT(smu_treap, formc_x, formc_y, formC, CIRCLE, &get_form_bounding_box);
    //     svg_export_form(svg_file, formC, CIRCLE); 
    // }
    
    // // double formr_x = 0, formr_y = 0; 
    // // FormStyle rect_style = new_form_style("#0000FF", "#00FF00", NULL, NULL);
    // // Rect formR = new_rect(11, formr_x, formr_y, 120, 120, rect_style);
    // // insertSmuT(smu_treap, formr_x, formr_y, formR, RECT, &get_form_bounding_box); 
    // // svg_export_form(svg_file, formR, RECT);
    
    // // Export all sub-bounding boxes from the SMU Treap
    // svg_export_all_sub_bounding_boxes(svg_file, smu_treap);
    
    svg_close(svg_file);
    file_close(svg_file);
    
    free_arg_manager(argm);
    
    return 0;
}

ArgManager check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager();
    add_new_arg(argm, "-e", false, "Diretório-base de entrada (BED)", DIR, NULL); 
    add_new_arg(argm, "-q", false, "Arquivo com consultas", DIR, NULL); 
    add_new_arg(argm, "-p", false, "Prioridade máxima (número inteiro)", INT, "10000"); 
    add_new_arg(argm, "-hc", false, "Parâmetro hit count usado na árvore (número inteiro)", INT, "3"); 
    add_new_arg(argm, "-pr", false, "Fator de promoção a ser usado quando hit count é atingido (número decimal)", DOUBLE, "1.10"); 
    // add_new_arg(argm, "-o", true, "Diretório-base de saída", DIR, NULL); 
    // add_new_arg(argm, "-f", true, "Arquivo com a descrição da treap", DIR, NULL); 
    
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
    FILE *svg_file = (FILE *) aux;
    DescritorTipoInfo info_type = getTypeInfoSmuT(t, n);

    svg_export_form(svg_file, info, info_type);
}