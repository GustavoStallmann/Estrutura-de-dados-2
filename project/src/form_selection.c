#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "form_selection.h"

#define MAX_SELECTION_REGIONS 100

typedef struct {
    double x, y, width, height;
    bool is_active;
} SelectionRegion_st;

typedef struct {
    SelectionRegion_st regions[MAX_SELECTION_REGIONS];
} SelectionManager_st;

SelectionRegion selection_region_create(double x, double y, double width, double height) {
    SelectionRegion_st* region = malloc(sizeof(SelectionRegion_st));
    if (region == NULL) return NULL;
    
    region->x = x;
    region->y = y;
    region->width = width;
    region->height = height;
    region->is_active = false;
    
    return (SelectionRegion)region;
}

void selection_region_destroy(SelectionRegion region) {
    if (region == NULL) return;
    free(region);
}

void selection_region_set_active(SelectionRegion region, bool active) {
    if (region == NULL) return;
    SelectionRegion_st* r = (SelectionRegion_st*)region;
    r->is_active = active;
}

bool selection_region_is_active(SelectionRegion region) {
    if (region == NULL) return false;
    SelectionRegion_st* r = (SelectionRegion_st*)region;
    return r->is_active;
}

void selection_region_get_bounds(SelectionRegion region, double* x, double* y, double* width, double* height) {
    if (region == NULL || x == NULL || y == NULL || width == NULL || height == NULL) return;
    SelectionRegion_st* r = (SelectionRegion_st*)region;
    *x = r->x;
    *y = r->y;
    *width = r->width;
    *height = r->height;
}

void selection_region_set_bounds(SelectionRegion region, double x, double y, double width, double height) {
    if (region == NULL) return;
    SelectionRegion_st* r = (SelectionRegion_st*)region;
    r->x = x;
    r->y = y;
    r->width = width;
    r->height = height;
}

SelectionManager selection_manager_create() {
    SelectionManager_st* manager = malloc(sizeof(SelectionManager_st));
    if (manager == NULL) return NULL;
    
    for (int i = 0; i < MAX_SELECTION_REGIONS; i++) {
        manager->regions[i].is_active = false;
    }
    
    return (SelectionManager)manager;
}

void selection_manager_destroy(SelectionManager manager) {
    if (manager == NULL) return;
    free(manager);
}

int selection_manager_get_region_count() {
    return MAX_SELECTION_REGIONS;
}

SelectionRegion selection_manager_get_region(SelectionManager manager, int index) {
    if (manager == NULL || index < 0 || index >= MAX_SELECTION_REGIONS) return NULL;
    SelectionManager_st* mgr = (SelectionManager_st*)manager;
    return (SelectionRegion)&(mgr->regions[index]);
}

void selection_manager_set_region_active(SelectionManager manager, int index, bool active) {
    if (manager == NULL || index < 0 || index >= MAX_SELECTION_REGIONS) return;
    SelectionManager_st* mgr = (SelectionManager_st*)manager;
    mgr->regions[index].is_active = active;
}

bool selection_manager_is_region_active(SelectionManager manager, int index) {
    if (manager == NULL || index < 0 || index >= MAX_SELECTION_REGIONS) return false;
    SelectionManager_st* mgr = (SelectionManager_st*)manager;
    return mgr->regions[index].is_active;
}

void selection_manager_get_region_data(SelectionManager manager, int index, double* x, double* y, double* width, double* height) {
    if (manager == NULL || index < 0 || index >= MAX_SELECTION_REGIONS || 
        x == NULL || y == NULL || width == NULL || height == NULL) return;
    
    SelectionManager_st* mgr = (SelectionManager_st*)manager;
    if (!mgr->regions[index].is_active) return; 

    *x = mgr->regions[index].x;
    *y = mgr->regions[index].y;
    *width = mgr->regions[index].width;
    *height = mgr->regions[index].height;
}

void selection_manager_set_region_data(SelectionManager manager, int index, double x, double y, double width, double height) {
    if (manager == NULL || index < 0 || index >= MAX_SELECTION_REGIONS) return;
    SelectionManager_st* mgr = (SelectionManager_st*)manager;
    mgr->regions[index].x = x;
    mgr->regions[index].y = y;
    mgr->regions[index].width = width;
    mgr->regions[index].height = height;
    mgr->regions[index].is_active = true;
}
