#include "file.h"
#include "file_svg.h"
#include "smu_treap.h"
#include "form_circle.h"
#include "form_rect.h"
#include "form.h"
#include "processor_dir.h"
#include "processor_args.h"
#include <stdlib.h>

ArgManager check_args(int argc, char *argv[]) {
    // for (int i = 0; i < argc; i++) {
    //     printf("arg[%d]: %s\n", i, argv[i]); 
    // }

    ArgManager argm = new_arg_manager();
    add_new_arg(argm, "-e", false, "Diretório-base de entrada (BED)", DIR, NULL); 
    add_new_arg(argm, "-q", false, "Arquivo com consultas", DIR, NULL); 
    add_new_arg(argm, "-p", false, "Prioridade máxima (número inteiro)", INT, "10000"); 
    add_new_arg(argm, "-hc", false, "Parâmetro hit count usado na árvore (número inteiro)", INT, "3"); 
    add_new_arg(argm, "-pr", false, "Fator de promoção a ser usado quando hit count é atingido (número decimal)", DOUBLE, "1.10"); 
    add_new_arg(argm, "-o", true, "Diretório-base de saída", DIR, NULL); 
    add_new_arg(argm, "-f", true, "Arquivo com a descrição da treap", DIR, NULL); 
    
    bool status = verify_args(argm, argc, argv); 
    if (!status) exit(EXIT_FAILURE);

    free_arg_manager(argm);
    return argm; 
}

int main(int argc, char *argv[]) {
    check_args(argc, argv);

    SmuTreap* smu_treap = newSmuTreap(2, 1.1, 0.5);

    double formc_x = 200, formc_y = 100; 
    Circle formC = new_circle(formc_x, formc_y, 100); 
    insertSmuT(smu_treap, formc_x, formc_y, formC, CIRCLE, &computeFormBoundingBox);

    double formr_x = 0, formr_y = 0; 
    Rect formR = new_rect(formr_x, formr_y, 120, 120);
    insertSmuT(smu_treap, formr_x, formr_y, formR, RECT, &computeFormBoundingBox); 

    FILE *svg_file = file_open_writable("output/test.svg"); 
    svg_init(svg_file, 800, 600);
    svg_export_form(svg_file, formC, CIRCLE); 
    svg_export_form(svg_file, formR, RECT); 
    svg_close(svg_file);

    return 0;
}