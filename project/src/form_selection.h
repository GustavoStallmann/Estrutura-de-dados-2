#ifndef FORM_SELECTION_H
#define FORM_SELECTION_H

#include <stdbool.h>

#define MAX_SELECTION_REGIONS 100

typedef void* SelectionRegion;
typedef void* SelectionManager;


SelectionRegion selection_region_create(double x, double y, double width, double height);
void selection_region_destroy(SelectionRegion region);
void selection_region_set_active(SelectionRegion region, bool active);
bool selection_region_is_active(SelectionRegion region);
void selection_region_get_bounds(SelectionRegion region, double* x, double* y, double* width, double* height);
void selection_region_set_bounds(SelectionRegion region, double x, double y, double width, double height);

SelectionManager selection_manager_create();
void selection_manager_destroy(SelectionManager manager);
int selection_manager_get_region_count();
SelectionRegion selection_manager_get_region(SelectionManager manager, int index);
void selection_manager_set_region_active(SelectionManager manager, int index, bool active);
bool selection_manager_is_region_active(SelectionManager manager, int index);
void selection_manager_get_region_data(SelectionManager manager, int index, double* x, double* y, double* width, double* height);
void selection_manager_set_region_data(SelectionManager manager, int index, double x, double y, double width, double height);

#endif
