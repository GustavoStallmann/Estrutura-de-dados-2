#include <stdio.h>
#include <string.h>

#include "form.h"
#include "form_circle.h"
#include "form_line.h"
#include "form_rect.h"
#include "form_style.h"
#include "form_text.h"

void get_form_bounding_box(DescritorTipoInfo tp, Info form, double *x, double *y, double *w, double *h) {
    switch (tp) {
        case CIRCLE:
            get_circle_bounding_box((Circle) form, x, y, w, h);
            break; 
        case RECT: 
            get_rect_bounding_box((Rect) form, x, y, w, h); 
            break; 
        case TEXT:
            get_text_bounding_box((Text) form, x, y, w, h); 
            break;
        case LINE: 
            get_line_bounding_box((Line) form, x, y, w, h);
            break;
        
        default:
            fprintf(stderr, "(ERROR) form: invalid form provided to compute the bounding box");
            break;
    }
}

FormStyle get_form_style(DescritorTipoInfo tp, Info form) {
    switch (tp) {
        case CIRCLE:
            return get_circle_style((Circle) form);
        case RECT: 
            return get_rect_style((Rect) form);
        case TEXT:
            return get_text_style((Text) form);
        case LINE: 
            return get_line_style((Line) form);
        
        default:
            fprintf(stderr, "(ERROR) form: invalid form provided to get style");
            return NULL;
    }
}

void free_form(DescritorTipoInfo tp, Info form) {
    switch (tp) {
        case CIRCLE:
            free_circle((Circle) form);
            break; 
        case RECT: 
            free_rect((Rect) form);
            break; 
        case TEXT:
            free_text((Text) form);
            break;
        case LINE: 
            free_line((Line) form);
            break;
        
        default:
            fprintf(stderr, "(ERROR) form: invalid form provided to free memory");
            break;
    }
}

void process_form(char *formType) {
    if (strcmp(formType, "ts") == 0) {
    
    } else if (strcmp(formType, "r") == 0) {
    } else if (strcmp(formType, "c") == 0) {
    } else if (strcmp(formType, "l") == 0) {
    } else if (strcmp(formType, "t") == 0) {
    } else {
        fprintf(stderr, "(ERROR) form: the .geo form type informed is invalid: %s", formType); 
    }
}