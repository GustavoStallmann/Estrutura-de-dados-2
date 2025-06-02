#ifndef FORM_STATE_H
#define FORM_STATE_H

#include <stdbool.h>
typedef void *FormState; 

FormState new_form_state();
void set_form_state_camouflaged(FormState state, bool camouflaged);
bool is_form_state_camouflaged(FormState state);
void set_form_state_selected(FormState state, bool selected);
bool is_form_state_selected(FormState state);
void set_form_state_blown(FormState state, bool blown);
bool is_form_state_blown(FormState state);
void free_form_state(FormState state);

#endif