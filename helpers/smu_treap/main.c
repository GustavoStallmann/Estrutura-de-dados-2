#include <stdio.h>

#include "smu_treap.h"
#include "form_circle.h"
#include "form_rect.h"

enum Form {
    CIRCLE, 
    RECT, 
    TEXT, 
};

void computeBoundingBox(DescritorTipoInfo tp, Info form, double *x, double *y, double *w, double *h) {
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

int main(int argc, char const *argv[]) {
    SmuTreap* smu_treap = newSmuTreap(2, 1.1, 0.5);

    const double form_x = 0, form_y = 0; 

    Circle form = new_circle(form_x, form_y, 10); 
    insertSmuT(smu_treap, form_x, form_y, form, CIRCLE, &computeBoundingBox);
    return 0;
}
