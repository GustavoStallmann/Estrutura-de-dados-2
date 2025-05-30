#include <assert.h>
#include "form.h" 
#include "file_svg.h"
#include "form_circle.h"
#include "form_line.h"
#include "form_rect.h"
#include "form_style.h"
#include "form_text.h"
#include <stdio.h>

static void svg_export_circle(FILE *svg_file, Circle circle, FormStyle style) {
    double x, y, r; 
    get_circle_positions(circle, &x, &y, &r); 

    fprintf(svg_file, 
        "<circle r='%lf' cx='%lf' cy='%lf' fill='red' style='fill: %s; stroke-width: 2; stroke:%s'/>\n",
        r, x, y, get_form_style_fill_color(style), get_form_style_border_color(style)
    );     
}

static void svg_export_rectangle(FILE *svg_file, Rect rect, FormStyle style) {
    double x, y, w, h; 
    get_rect_positions(rect, &x, &y, &w, &h); 

    fprintf(svg_file, 
        "<rect width='%lf' height='%lf' x='%lf' y='%lf' rx='7' ry='7' style='fill: %s; stroke-width: 2; stroke:%s' />\n",
        w, h, x, y, get_form_style_fill_color(style), get_form_style_border_color(style)
    ); 
}

static void svg_export_text(FILE *svg_file, Text text, FormStyle style) {
    double x, y; 
    get_text_positions(text, &x, &y); 

    fprintf(svg_file, 
        "<text x='%lf' y='%lf' fill='black' style='font-family: %s; font-weight: %s'; fill: '%s'; stroke-width:2; stroke:%s>%s</text>\n",
        x, y, get_form_style_font_family(style), get_form_style_font_weight(style), 
        get_form_style_fill_color(style), get_form_style_border_color(style), get_text_string(text)
    ); 
}

static void svg_export_line(FILE *svg_file, Line line, FormStyle style) {
    double x1, y1, x2, y2;
    get_line_positions(line, &x1, &y1, &x2, &y2);

    fprintf(svg_file, 
        "<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' style='stroke: %s; stroke-width: 2;'/>\n",
        x1, y1, x2, y2, get_form_style_border_color(style)
    );
}

void svg_export_form(FILE *svg_file, Info form, DescritorTipoInfo form_type) {
    assert(svg_file); 
    FormStyle style = get_form_style(form_type, form);

    switch (form_type) {
        case CIRCLE:
            svg_export_circle(svg_file, (Circle) form, style); 
            break; 
        case RECT: 
            svg_export_rectangle(svg_file, (Rect) form, style); 
            break; 
        case TEXT:
            svg_export_text(svg_file, (Text) form, style); 
            break;
        case LINE: 
            svg_export_line(svg_file, (Line) form, style);
            break;
        default:
            fprintf(stderr, "(ERROR) file_svg: couldn't indentify the given form to export (form: %d)", form_type); 
            break; 
    }
}

void svg_init(FILE *svg_file, double width, double height) {
    assert(svg_file);
    
    fprintf(svg_file, 
        "<svg width='%lf' height='%lf' xmlns='http://www.w3.org/2000/svg'>\n", 
        width, height
    ); 
}

void svg_close(FILE *svg_file) {
    assert(svg_file);
    
    fprintf(svg_file, "</svg>");
}   

//REMOVE
void svg_export_bounding_box(FILE *svg_file, Info form, DescritorTipoInfo form_type) {
    assert(svg_file);
    
    double x, y, w, h;
    get_form_bounding_box(form_type, form, &x, &y, &w, &h);
    
    // Export bounding box as a dashed rectangle with no fill and red stroke
    fprintf(svg_file, 
        "<rect width='%lf' height='%lf' x='%lf' y='%lf' style='fill: none; stroke: red; stroke-width: 1; stroke-dasharray: 5,5; opacity: 0.7' />\n",
        w, h, x, y
    );
}

void svg_export_sub_bounding_box(FILE *svg_file, SmuTreap treap, Node node) {
    assert(svg_file);
    assert(node);
    
    double x, y, w, h;
    getBoundingBoxSmuT(treap, node, &x, &y, &w, &h);
    
    // Export sub-bounding box as a dashed rectangle with no fill and blue stroke
    fprintf(svg_file, 
        "<rect width='%lf' height='%lf' x='%lf' y='%lf' style='fill: none; stroke: blue; stroke-width: 2; stroke-dasharray: 10,5; opacity: 0.5' />\n",
        w, h, x, y
    );
}

// Helper function for tree traversal to export sub-bounding boxes
static void export_sub_bb_visitor(SmuTreap t, Node n, Info i, double x, double y, void *aux) {
    FILE *svg_file = (FILE *)aux;
    svg_export_sub_bounding_box(svg_file, t, n);
}

void svg_export_all_sub_bounding_boxes(FILE *svg_file, SmuTreap treap) {
    assert(svg_file);
    assert(treap);
    
    visitaProfundidadeSmuT(treap, export_sub_bb_visitor, svg_file);
}