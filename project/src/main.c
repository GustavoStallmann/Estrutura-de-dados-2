#include "file.h"
#include "file_svg.h"
#include "smu_treap.h"
#include "form_circle.h"
#include "form_rect.h"
#include "form.h"
#include "args.h"

ArgManager check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager();
    add_new_arg(argm, 'e', true, "Argumento de teste"); 
    
    verify_args(argm, argc, argv); 
    free_arg_manager(argm);
    return argm; 
}

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);

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
