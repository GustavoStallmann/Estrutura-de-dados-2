#ifndef PROCESSOR_QRY_H
#define PROCESSOR_QRY_H

#include "processor_dir.h"
#include "smu_treap.h"
#include "form_selection.h"

void qry_process(Dir qry, Dir txt, SmuTreap smu_treap, SelectionManager selection_manager);

#endif