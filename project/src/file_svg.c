#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "form.h" 
#include "form_circle.h"
#include "form_line.h"
#include "form_rect.h"
#include "form_state.h"
#include "form_text.h"
#include "file_svg.h"

static void svg_export_form_states(FILE *svg_file, int form_id, FormState state, double x, double y) {
    if (is_form_state_blown(state)) {
        fprintf(svg_file, 
            "\t<text id='b-%d' x='%lf' y='%lf' fill='red' font-size='8px'>X</text>\n",
            form_id, x, y
        ); 
        return; 
    }; 

    if (is_form_state_selected(state)) {
        fprintf(svg_file, 
            "\t<circle id='s-%d' cx='%lf' cy='%lf' r='0.5' fill='none' stroke='red' stroke-width='1'/>\n",
            form_id, x, y
        );
        return; 
    }; 
}

static void svg_export_circle(FILE *svg_file, int form_id, Circle circle, FormStyle style, FormState state) {
    double x, y, r; 
    get_circle_positions(circle, &x, &y, &r); 

    fprintf(svg_file, 
        "\t<circle id='%d' r='%lf' cx='%lf' cy='%lf' fill='%s' stroke='%s' stroke-width='%s'/>\n",
        form_id, r, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style)
    );     
    svg_export_form_states(svg_file, form_id, state, x, y);
}

static void svg_export_rectangle(FILE *svg_file, int form_id, Rect rect, FormStyle style, FormState state) {
    double x, y, w, h; 
    get_rect_positions(rect, &x, &y, &w, &h); 

    fprintf(svg_file, 
        "\t<rect id='%d' width='%lf' height='%lf' x='%lf' y='%lf' fill='%s' stroke='%s' stroke-width='%s' fill-opacity='0.8'/>\n",
        form_id, w, h, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style)
    ); 
    svg_export_form_states(svg_file, form_id, state, x, y);
}

static void svg_export_text(FILE *svg_file, int form_id, Text text, FormStyle style, FormState state) {
    double x, y; 
    get_text_positions(text, &x, &y); 
    char *font_weight = get_form_style_font_weight(style);

    if (font_weight != NULL && strcmp(font_weight, "b") == 0) {
        font_weight = "bold"; 
    } else if (font_weight != NULL && strcmp(font_weight, "i") == 0) {
        font_weight = "italic"; 
    } else {
        font_weight = "normal"; 
    }

    char *form_text = get_text_string(text);
    if (text == NULL) {
        fprintf(stderr, "(ERROR) file_svg: couldn't escape text content\n");
        return;
    }

    fprintf(svg_file, 
        "\t<text id='%d' x='%lf' y='%lf' fill='%s' stroke='%s' stroke-width='%s' text-anchor='%s' font-weight='%s' font-size='%s'><![CDATA[ %s ]]></text>\n",
        form_id, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style), 
        get_form_style_text_anchor(style), font_weight, get_form_style_font_size(style), form_text
    ); 
    svg_export_form_states(svg_file, form_id, state, x, y);
}

static void svg_export_line(FILE *svg_file, int form_id, Line line, FormStyle style, FormState state) {
    double x1, y1, x2, y2;
    get_line_positions(line, &x1, &y1, &x2, &y2);

    fprintf(svg_file, 
        "\t<line id='%d' x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' stroke-width='%s'/>\n",
        form_id, x1, y1, x2, y2, get_form_style_border_color(style), get_form_style_stroke_width(style)
    );

    svg_export_form_states(svg_file, form_id, state, (x1 + x2) / 2, (y1 + y2) / 2);
}

void svg_export_form(FILE *svg_file, Info form, DescritorTipoInfo form_type) {
    assert(svg_file); 
    FormStyle style = get_form_style(form_type, form);
    FormState state = get_form_state(form_type, form);
    int form_id = get_form_id(form_type, form);

    switch (form_type) {
        case CIRCLE:
            svg_export_circle(svg_file, form_id, (Circle) form, style, state); 
            break; 
        case RECT: 
            svg_export_rectangle(svg_file, form_id, (Rect) form, style, state); 
            break; 
        case TEXT:
            svg_export_text(svg_file, form_id, (Text) form, style, state); 
            break;
        case LINE: 
            svg_export_line(svg_file, form_id, (Line) form, style, state);
            break;
        default:
            fprintf(stderr, "(ERROR) file_svg: couldn't indentify the given form to export (form: %d)", form_type); 
            break; 
    }

    if (state == NULL) {
        fprintf(stderr, "(ERROR) file_svg: form state is NULL, cannot export form state attributes\n");
        return; 
    }; 
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

void svg_export_selection_regions(FILE *svg_file, SelectionManager selection_manager) {
    if (svg_file == NULL || selection_manager == NULL) return;
    
    int region_count = selection_manager_get_region_count();
    
    for (int i = 0; i < region_count; i++) {
        if (!selection_manager_is_region_active(selection_manager, i)) continue; 

        double x, y, width, height;
        selection_manager_get_region_data(selection_manager, i, &x, &y, &width, &height);
        
        fprintf(svg_file, 
            "\t<rect id='selection-%d' x='%lf' y='%lf' width='%lf' height='%lf' "
            "fill='none' stroke='red' stroke-width='1' stroke-dasharray='0.5' opacity='0.7' />\n",
            i, x, y, width, height
        );

    }
}   