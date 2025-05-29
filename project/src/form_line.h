#ifndef FORM_LINE_H 
#define FORM_LINE_H
#include "form_style.h"

typedef void *Line;

/*
 * Create a new line form
 * @param i the id of the line
 * @param x the first point horizontal coordinate
 * @param y the first point vertical coordinate 
 * @param x2 the second point horizontal coordinate
 * @param y2 the second point vertical coordinate
 * @return the new line
*/
Line new_line(int i, double x, double y, double x2, double y2, FormStyle style);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param l the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void get_line_bounding_box(Line l, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param l the form to get the position
 * @param x, y, x2, y2 the available positions 
*/
void get_line_position(Line l, double *x, double *y, double *x2, double *y2);

/* 
 * Gets the style of the line
 * @param l the form to get the style
 * @return the style of the line
*/
FormStyle get_line_style(Line l); 

/* 
 * Free memory of the given form
 */
void free_line(Line l);
 
#endif