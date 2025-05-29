#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#include "form_circle.h"
#include "form_style.h"

typedef struct {
    int id; 
    double x, y, r; 
    FormStyle style;
} Circle_st; 

Circle new_circle(int id, double x, double y, double r, FormStyle style) {
    Circle_st *circle = (Circle_st *) malloc(sizeof(Circle_st)); 
    if (circle == NULL) {
        fprintf(stderr, "(ERROR) form_circle: memória insuficiente para alocar circulo");
        return NULL;
    }

    circle->id = id; 
    circle->x = x; 
    circle->y = y; 
    circle->r = r;
    circle->style = style; 

    return circle; 
}

void get_circle_bounding_box(Circle c, double *x, double *y, double *w, double *h) {
    assert(c);
    
    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return; 

    *x = circle->x - circle->r; 
    *y = circle->y - circle->r; 
    *w = circle->r * 2; 
    *h = circle->r * 2; 
}

void get_circle_positions(Circle c, double *x, double *y, double *r) {
    assert(c); 
    
    Circle_st *circle = (Circle_st *) c;
    if (circle == NULL) return; 

    *x = circle->x; 
    *y = circle->y; 
    *r = circle->r; 
}

FormStyle get_circle_style(Circle c) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return NULL; 

    return circle->style; 
}

void free_circle(Circle c) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return; 

    free_form_style(circle->style);
    free(circle);
}