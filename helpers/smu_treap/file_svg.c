#include "form.h" 
#include "form_circle.h"
#include "file_svg.h"

static void svg_export_circle(FILE *svg_file, Circle circle, char *output) {
    double x, y, r; 
    getCirclePositions(circle, &x, &y, &r); 

    fprintf(svg_file, 
        "<circle r=\"%lf\" cx=\"%lf\" cy=\"%lf\" fill=\"red\" />\n",
        r, x, y
    );     
}

void svg_export_form(FILE *svg_file, Info form, DescritorTipoInfo formType) {
    char *form_output; 

    switch (formType) {
        case CIRCLE:
            svg_export_circle(svg_file, (Circle) form, form_output); 
            break; 
        case RECT: 
            break; 

        default:
            break; 
    
    }
}