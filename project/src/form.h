#ifndef FORM_H
#define FORM_H

#include "smu_treap.h"
#include "form_style.h"
#include "form_state.h"

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

void get_form_coordinates(DescritorTipoInfo tp, Info form, double *x, double *y);
void get_form_dimensions(DescritorTipoInfo tp, Info form, double *w, double *h);
FormState get_form_state(DescritorTipoInfo tp, Info form);
bool is_form_inside_region(SmuTreap t, Node n, Info i, double x1, double y1, double x2, double y2);
double get_form_distance_disp(DescritorTipoInfo tp, Info form); 

/* 
 * Translates the form to the given coordinates
 * @param tp the type of the form
 * @param form the form to translate
 * @param x, y the new coordinates of the form
*/
void transp_form(DescritorTipoInfo tp, Info form, double x, double y); 

/*
 * Gets the id of the form
 * @param tp the type of the form
 * @param form the form to get the id
 * @return the id of the form
*/
int get_form_id(DescritorTipoInfo tp, Info form);

/*
 * Frees the memory of the form
 * @param tp the type of the form
 * @param form the form to free
 */
void free_form(DescritorTipoInfo tp, Info form);

#endif