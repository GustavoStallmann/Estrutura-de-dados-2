#ifndef FORM_STYLE_H
#define FORM_STYLE_H

typedef void* FormStyle; 

FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor, char *fontSize);
char* get_form_style_border_color(FormStyle style);
char* get_form_style_fill_color(FormStyle style);
char* get_form_style_font_family(FormStyle style);
char* get_form_style_text_anchor(FormStyle style);
char* get_form_style_font_weight(FormStyle style); 
char* get_form_style_font_size(FormStyle style);
void free_form_style(FormStyle style);

#endif