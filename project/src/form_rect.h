#ifndef FORM_RECT
#define FORM_RECT
#include "form_style.h"

typedef void *Rect;

/*
 * Create a new Rect form
 * @param x the horizontal coordinate 
 * @param y the vertical coordinate 
 * @param r the radius of the Rect
 * @return the new Rect
*/
Rect new_rect(int id, double x, double y, double w, double h, FormStyle style);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param r the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void get_rect_bounding_box(Rect r, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param r the form to get the position
 * @param x, y, w, h the available positions 
*/
void get_rect_positions(Rect r, double *x, double *y, double *w, double *h);

/* 
 * Gets the style of the Rect
 * @param r the form to get the style
 * @return the style of the Rect
*/
FormStyle get_rect_style(Rect r); 

/* 
 * Free memory of the given form
 */
void free_rect(Rect r);

#endif