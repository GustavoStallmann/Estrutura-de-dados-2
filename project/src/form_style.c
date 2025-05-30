#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include  "form_style.h"

typedef struct {
    char *borderColor;
    char *fillColor; 
    char *fontFamily; 
    char *fontWeight;
    char *textAnchor;
} FormStyle_st; 

static char* alloc_str(char *str) {
    if (str == NULL) return NULL; 

    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size);
    if (new_str == NULL) {
        fprintf(stderr, "(ERROR) form_text: insufficient memory to alloc text");
        return NULL;
    } 

    strcpy(new_str, str);

    return new_str; 
}

FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor) {
    FormStyle_st *style = (FormStyle_st *) malloc(sizeof(FormStyle_st)); 
    if (style == NULL) {
        fprintf(stderr, "(ERROR) form_style: insufficient memory to alloc formStyle_st");
        return NULL;
    }

    style->borderColor = alloc_str(borderColor); 
    style->fillColor = alloc_str(fillColor); 
    style->fontFamily = alloc_str(fontFamily); 
    style->textAnchor = alloc_str(textAnchor);
    style->fontWeight = alloc_str(fontWeight);

    return style; 
}

char* get_form_style_border_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "#000000";

    return style_st->borderColor; 
}

char* get_form_style_fill_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "#000000";

    return style_st->fillColor;  
}

char* get_form_style_font_family(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "Default";

    return style_st->fontFamily; 
}

char* get_form_style_font_weight(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "normal";

    return style_st->fontWeight; 
}

char* get_form_style_text_anchor(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "start";

    return style_st->textAnchor; 
}

void free_form_style(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;
    
    free(style_st->borderColor);
    free(style_st->fillColor);
    free(style_st->fontFamily);
    free(style_st->fontWeight);
    free(style_st);
}