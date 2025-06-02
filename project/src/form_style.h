#ifndef FORM_STYLE_H
#define FORM_STYLE_H

typedef void* FormStyle; 

FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor, char *fontSize, char *strokeWidth);
char* get_form_style_border_color(FormStyle style);
char* get_form_style_fill_color(FormStyle style);
char* get_form_style_font_family(FormStyle style);
char* get_form_style_text_anchor(FormStyle style);
char* get_form_style_font_weight(FormStyle style); 
char* get_form_style_font_size(FormStyle style);
char* get_form_style_stroke_width(FormStyle style);
void set_form_fill_color(FormStyle style, char *fillColor);
void set_form_border_color(FormStyle style, char *borderColor);
void set_form_style_stroke_width(FormStyle style, char *strokeWidth);
void free_form_style(FormStyle style);

#endif