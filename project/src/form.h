#ifndef FORM_H
#define FORM_H

#include "smu_treap.h"

enum Form {
    CIRCLE, 
    RECT, 
    TEXT, 
};

void computeFormBoundingBox(DescritorTipoInfo tp, Info form, double *x, double *y, double *w, double *h);

#endif