#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "form_rect.h"
#include "form_style.h"

typedef struct {
    int id; 
    double x, y, w, h; 
    FormStyle style;
} Rect_st;

Rect new_rect(int id, double x, double y, double w, double h, FormStyle style) {
    Rect_st *rect = (Rect_st *) malloc(sizeof(Rect_st)); 
    if (rect == NULL) {
        fprintf(stderr, "(ERROR) form_rect: memória insuficiente para alocar nova forma");
        return NULL; 
    }

    rect->id = id; 
    rect->x = x; 
    rect->y = y; 
    rect->w = w; 
    rect->h = h; 
    rect->style = style;

    return rect; 
}

void get_rect_bounding_box(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    *x = rect->x; 
    *y = rect->y; 
    *w = rect->w; 
    *h = rect->h; 
}

FormStyle get_rect_style(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return NULL; 

    return rect->style; 
}

void get_rect_positions(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    *x = rect->x; 
    *y = rect->y; 
    *w = rect->w; 
    *h = rect->h; 
}

void free_rect(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    free_form_style(rect->style);
    free(rect);
}