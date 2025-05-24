#include "smu_treap.h"
#include "form_circle.h"
#include "form_rect.h"
#include "form.h"

int main(int argc, char *argv[]) {
    SmuTreap* smu_treap = newSmuTreap(2, 1.1, 0.5);

    double formc_x = 0, formc_y = 0; 
    Circle formC = new_circle(formc_x, formc_y, 10); 
    insertSmuT(smu_treap, formc_x, formc_y, formC, CIRCLE, &computeFormBoundingBox);

    double formr_x = 0, formr_y = 0; 
    Rect formR = new_rect(formr_x, formr_y, 10, 10);
    insertSmuT(smu_treap, formr_x, formr_y, formR, RECT, &computeFormBoundingBox); 
    return 0;
}
