#include <stdio.h>

#include "form.h"
#include "form_circle.h"
#include "form_rect.h"

void computeFormBoundingBox(DescritorTipoInfo tp, Info form, double *x, double *y, double *w, double *h) {
    switch (tp) {
        case CIRCLE:
            getCircleBoundingBox((Circle) form, x, y, w, h);
            break; 
        case RECT: 
            getRectBoundingBox((Rect) form, x, y, w, h); 
            break; 
        
        default:
            fprintf(stderr, "main: invalid form provided to compute the bounding box");
            break;
    }
}