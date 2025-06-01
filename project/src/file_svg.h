#ifndef OUTPUT_SVG
#define OUTPUT_SVG

#include <stdio.h>
#include "smu_treap.h"
#include "form_selection.h"

/*
 * Initializes a file as an svg
 * @param svg_file the file reference
 * @param width the width of the canvas
 * @param height the height of the canvas
 */
void svg_init(FILE *svg_file, double width, double height);

/* 
 * Export the given form to the svg file
 * @param svg_file the file reference
 * @param Info the form to be exported
 * @param form_type the form identifier
 */
void svg_export_form(FILE *svg_file, Info f, DescritorTipoInfo form_type); 
 
 /* 
 * Export the sub-bounding box of a SMU Treap node to the svg file as a dashed rectangle
 * @param svg_file the file reference
 * @param treap the SMU Treap structure
 * @param node the node to get the sub-bounding box from
 */
 void svg_export_sub_bounding_box(FILE *svg_file, SmuTreap treap, Node node);
 
 /* 
 * Closes the given svg file
 * @param svg_file the file reference
 */
 void svg_close(FILE *svg_file);

/*
 * Export selection regions to the SVG file as dashed rectangles
 * @param svg_file the file reference
 * @param selection_manager the selection manager containing regions to export
 */
void svg_export_selection_regions(FILE *svg_file, SelectionManager selection_manager);

#endif