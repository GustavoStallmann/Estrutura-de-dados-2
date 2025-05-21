#ifndef FORM_RECT
#define FORM_RECT

typedef void *Rect;

/*
 * Create a new Rect form
 * @param x the horizontal coordinate 
 * @param y the vertical coordinate 
 * @param r the radius of the Rect
 * @return the new Rect
*/
Rect new_rect(double x, double y, double r, double w, double h);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param r the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void getRectBoundingBox(Rect r, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param r the form to get the position
 * @param x, y, w, h the available positions 
*/
void getRectPositions(Rect r, double *x, double *y, double *w, double *h);

#endif