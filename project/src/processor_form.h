#ifndef PROCESSOR_FORM_H
#define PROCESSOR_FORM_H
#include "smu_treap.h"
#include "form_style.h"

typedef void* FormInfo;

FormInfo process_form(char *formType, char *line_buffer, FormStyle *actual_font_style); 
Info get_form_info(FormInfo formInfo);
DescritorTipoInfo get_form_type(FormInfo formInfo);
void free_form_info(FormInfo formInfo);
void free_form_info_wrapper_only(FormInfo formInfo);

#endif