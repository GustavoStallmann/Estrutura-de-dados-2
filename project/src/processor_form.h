#ifndef PROCESSOR_FORM_H
#define PROCESSOR_FORM_H
#include "smu_treap.h"


typedef void* FormInfo;

Info process_form(char *formType, char *line_buffer); 
Info get_form_info(FormInfo formInfo);
DescritorTipoInfo get_form_type(FormInfo formInfo);
void free_form_info(FormInfo formInfo);

#endif