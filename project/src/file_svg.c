#include <assert.h>
#include "form.h" 
#include "file_svg.h"
#include "form_circle.h"
#include "form_rect.h"
#include <stdio.h>

static void svg_export_circle(FILE *svg_file, Circle circle) {
    double x, y, r; 
    getCirclePositions(circle, &x, &y, &r); 

    fprintf(svg_file, 
        "<circle r=\"%lf\" cx=\"%lf\" cy=\"%lf\" fill=\"red\" />\n",
        r, x, y
    );     
}

static void svg_export_rectangle(FILE *svg_file, Rect rect) {
    double x, y, w, h; 
    getRectPositions(rect, &x, &y, &w, &h); 

    fprintf(svg_file, 
        "<rect width='%lf' height='%lf' x='%lf' y='%lf' rx='7' ry='7' fill='blue' />",
        w, h, x, y
    ); 
}

void svg_export_form(FILE *svg_file, Info form, DescritorTipoInfo form_type) {
    assert(svg_file); 

    switch (form_type) {
        case CIRCLE:
            svg_export_circle(svg_file, (Circle) form); 
            break; 
        case RECT: 
            svg_export_rectangle(svg_file, (Rect) form); 
            break; 
        default:
            fprintf(stderr, "file_svg: couldn't indentify the given form to export (form: %d)", form_type); 
            break; 
    }
}

void svg_init(FILE *svg_file, double width, double height) {
    assert(svg_file);

    fprintf(svg_file, 
        "<svg width='%lf' height='%lf' xmlns='http://www.w3.org/2000/svg'>", 
        width, height
    ); 
}

void svg_close(FILE *svg_file) {
    assert(svg_file);

    fprintf(svg_file, "</svg>");
}