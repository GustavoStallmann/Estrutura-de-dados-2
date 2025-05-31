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
    char *fontSize; 
    char *textAnchor;
} FormStyle_st; 

static char* alloc_str(char *str) {
    if (str == NULL) return NULL; 

    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size);
    if (new_str == NULL) {
        fprintf(stderr, "(ERROR) form_style: insufficient memory to alloc string");
        return NULL;
    } 

    strcpy(new_str, str);

    return new_str; 
}

FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor, char *fontSize) {
    FormStyle_st *style = (FormStyle_st *) malloc(sizeof(FormStyle_st)); 
    if (style == NULL) {
        fprintf(stderr, "(ERROR) form_style: insufficient memory to alloc formStyle_st");
        return NULL;
    }

    style->borderColor = alloc_str(borderColor); 
    style->fillColor = alloc_str(fillColor); 
    style->fontFamily = alloc_str(fontFamily); 
    style->fontWeight = alloc_str(fontWeight);
    if (textAnchor != NULL && strcmp(textAnchor, "m") == 0) {
        style->textAnchor = alloc_str("middle");
    } else if (textAnchor != NULL && strcmp(textAnchor, "f") == 0) {
        style->textAnchor = alloc_str("end");
    } else {
        style->textAnchor = alloc_str("start");
    }
    style->fontSize = alloc_str(fontSize);

    return style; 
}

char* get_form_style_border_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->borderColor == NULL) return "#000000";

    return style_st->borderColor; 
}

char* get_form_style_fill_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fillColor == NULL) return "#000000";

    return style_st->fillColor;  
}

char* get_form_style_font_family(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fontFamily == NULL) return "Arial";

    return style_st->fontFamily; 
}

char* get_form_style_font_weight(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fontWeight == NULL) return "normal";

    return style_st->fontWeight; 
}

char* get_form_style_text_anchor(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->textAnchor == NULL) return "start";

    return style_st->textAnchor; 
}

char* get_form_style_font_size(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "12px";

    return style_st->fontSize; 
}

void free_form_style(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;
    
    free(style_st->borderColor);
    free(style_st->fillColor);
    free(style_st->fontFamily);
    free(style_st->fontWeight);
    free(style_st->textAnchor);
    free(style_st->fontSize);
    free(style_st);
}