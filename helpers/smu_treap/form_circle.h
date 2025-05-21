#ifndef FORM_CIRCLE
#define FORM_CIRCLE

typedef void *Circle;

/*
 * Create a new circle form
 * @param x the horizontal coordinate 
 * @param y the vertical coordinate 
 * @param r the radius of the circle
 * @return the new circle
*/
Circle new_circle(double x, double y, double r);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param c the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void getCircleBoundingBox(Circle c, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param c the form to get the position
 * @param x, y, r the available positions 
*/
void getCirclePositions(Circle c, double *x, double *y, double *r);

#endif