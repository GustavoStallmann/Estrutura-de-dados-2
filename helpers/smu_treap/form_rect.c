#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "form_rect.h"

typedef struct {
    double x, y, w, h; 
} Rect_st;

Rect new_rect(double x, double y, double w, double h) {
    Rect_st* rect = (Rect_st *) malloc(sizeof(Rect_st)); 
    if (rect == NULL) {
        fprintf(stderr, "form_rect: memória insuficiente para alocar nova forma");
        return NULL; 
    }

    rect->x = x; 
    rect->y = y; 
    rect->w = w; 
    rect->h = h; 

    return rect; 
}

void getRectBoundingBox(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    *x = rect->x; 
    *y = rect->y; 
    *w = rect->w; 
    *h = rect->h; 
}

void getRectPositions(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    *x = rect->x; 
    *y = rect->y; 
    *w = rect->w; 
    *h = rect->h; 
}
