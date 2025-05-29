#ifndef FORM_H
#define FORM_H

#include "smu_treap.h"
#include "form_style.h"

enum Form {
    CIRCLE, 
    RECT, 
    TEXT, 
    LINE
};

/* 
 * Sets the minimum bounding box of the form in the given variables
 * @param tp the type of the form
 * @param form the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
 */
void get_form_bounding_box(DescritorTipoInfo tp, Info form, double *x, double *y, double *w, double *h);

/*
 * Gets the style of the form
 * @param tp the type of the form
 * @param form the form to get the style
 * @return the style of the form
 */
FormStyle get_form_style(DescritorTipoInfo tp, Info form); 

/*
 * Frees the memory of the form
 * @param tp the type of the form
 * @param form the form to free
 */
void free_form(DescritorTipoInfo tp, Info form);

#endif