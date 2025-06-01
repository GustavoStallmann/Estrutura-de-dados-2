#ifndef SELECTION_TYPES_H
#define SELECTION_TYPES_H

#include <stdbool.h>

// Selection region structure
typedef struct {
    double x, y, width, height;
    bool is_active;
} SelectionRegion;

// Selection manager structure
typedef struct {
    SelectionRegion regions[100];
} SelectionManager_st;

#endif
