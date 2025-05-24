#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#include "form_circle.h"

typedef struct {
    double x, y, r; 
} Circle_st; 

Circle new_circle(double x, double y, double r) {
    Circle_st *circle = (Circle_st *) malloc(sizeof(Circle_st)); 
    if (circle == NULL) {
        fprintf(stderr, "form_circle: memória insuficiente para alocar circulo");
        return NULL;
    }

    circle->x = x; 
    circle->y = y; 
    circle->r = r;

    return circle; 
}


void getCircleBoundingBox(Circle c, double *x, double *y, double *w, double *h) {
    assert(c);
    
    Circle_st *circle = (Circle_st *) c; 
    *x = circle->x - circle->r; 
    *y = circle->y - circle->r; 
    *w = circle->r * 2; 
    *h = circle->r * 2; 
}

void getCirclePositions(Circle c, double *x, double *y, double *r) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c;
    *x = circle->x; 
    *y = circle->y; 
    *r = circle->r; 
}