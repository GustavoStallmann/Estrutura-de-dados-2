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

int main(int argc, char *argv[]) {
    SmuTreap* smu_treap = newSmuTreap(2, 1.1, 0.5);

    double formc_x = 0, formc_y = 0; 

    Circle formC = new_circle(formc_x, formc_y, 10); 
    insertSmuT(smu_treap, formc_x, formc_y, formC, CIRCLE, &computeBoundingBox);

    double formr_x = 0, formr_y = 0; 
    Rect formR = new_rect(formr_x, formr_y, 10, 10);
    insertSmuT(smu_treap, formr_x, formr_y, formR, RECT, &computeBoundingBox); 
    return 0;
}
