#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "form_line.h"
#include "form_style.h"

typedef struct {
    int id; 
    double x, y, x2, y2; 
    FormStyle style;
} Line_st; 

Line new_line(int id, double x, double y, double x2, double y2, FormStyle style) {
    Line_st *line = (Line_st *) malloc(sizeof(Line_st)); 
    if (line == NULL) {
        fprintf(stderr, "(ERROR) form_line: insufficient memory to alloc line");
        return NULL;
    }

    line->id = id; 
    line->x = x; 
    line->y = y; 
    line->x2 = x2; 
    line->y2 = y2; 
    line->style = style;

    return line; 
}

void get_line_bounding_box(Line l, double *x, double *y, double *w, double *h) {
    assert(l); 

    Line_st *line = (Line_st *) l; 
    if (line == NULL) return;  
    
    *x = line->x < line->x2? line->x: line->x2; 
    *y = line->y < line->y2? line->y: line->y2; 
    *w = fabs(line->x - line->x2);
    *h = fabs(line->y - line->y2);
}

void get_line_positions(Line l, double *x, double *y, double *x2, double *y2){
    assert(l); 

    Line_st *line = (Line_st *) l; 
    if (line == NULL) return; 

    if (x != NULL)
        *x = line->x; 
    if (y != NULL)
        *y = line->y; 
    if (x2 != NULL)
        *x2 = line->x2; 
    if (y2 != NULL)
        *y2 = line->y2; 
}

FormStyle get_line_style(Line l) {
    assert(l); 

    Line_st *line = (Line_st *) l; 
    if (line == NULL) return NULL; 

    return line->style; 
}

void free_line(Line l) {
    assert(l); 

    Line_st *line = (Line_st *) l; 
    if (line == NULL) return; 

    free_form_style(line->style);
    free(line);
}