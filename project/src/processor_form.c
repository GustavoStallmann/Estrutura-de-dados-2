#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processor_form.h"
#include "form_style.h"
#include "form.h"
#include "form_circle.h"
#include "form_rect.h"
#include "form_line.h"
#include "form_text.h"
#include "smu_treap.h"

#define ARG_SIZE 20

typedef struct {
    DescritorTipoInfo tp;
    Info form;
} FormInfo_st;

FormInfo new_form_info(DescritorTipoInfo tp, Info form) {
    FormInfo_st *formInfo = (FormInfo_st *) malloc(sizeof(FormInfo_st));
    if (formInfo == NULL) {
        fprintf(stderr, "(ERROR) form: couldn't allocate memory for FormInfo_st");
        return NULL; 
    }

    formInfo->tp = tp;
    formInfo->form = form;

    return formInfo;
}

Info get_form_info(FormInfo formInfo) {
    if (formInfo == NULL) {
        fprintf(stderr, "(ERROR) form: the form info is NULL");
        return NULL; 
    }

    FormInfo_st *info = (FormInfo_st *) formInfo;
    return (Info) info->form;
}

DescritorTipoInfo get_form_type(FormInfo formInfo) {
    if (formInfo == NULL) {
        fprintf(stderr, "(ERROR) form: the form info is NULL");
        return -1; 
    }

    FormInfo_st *info = (FormInfo_st *) formInfo;
    return info->tp;
}

void free_form_info(FormInfo formInfo) {
    if (formInfo == NULL) {
        fprintf(stderr, "(ERROR) form: the form info is NULL");
        return; 
    }

    free(formInfo);
}

static Info process_circle(char *line_buffer) {
    int id; 
    double x, y, r;
    char fillColor[ARG_SIZE], borderColor[ARG_SIZE]; 
    sscanf(line_buffer, "%d %lf %lf %lf %s %s", &id, &x, &y, &r, borderColor, fillColor);
    FormStyle style = new_form_style(borderColor, fillColor, NULL, NULL, NULL); 
    Circle circle = new_circle(id, x, y, r, style);
    if (circle == NULL) {
        fprintf(stderr, "(ERROR) form: couldn't create circle with id %d", id);
        return NULL; 
    }

    return new_form_info(CIRCLE, circle); 
}

static Info process_rect(char * line_buffer) {
    int id; 
    double x, y, w, h;
    char fillColor[ARG_SIZE], borderColor[ARG_SIZE]; 
    sscanf(line_buffer, "%d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, borderColor, fillColor);
    FormStyle style = new_form_style(borderColor, fillColor, NULL, NULL, NULL); 
    Rect rect = new_rect(id, x, y, w, h, style);
    if (rect == NULL) {
        fprintf(stderr, "(ERROR) form: couldn't create rect with id %d", id);
        return NULL; 
    }

    return new_form_info(RECT, rect); 
}

static Info process_text(char *line_buffer) {
    int id; 
    double x, y; 
    char text[ARG_SIZE], fillColor[ARG_SIZE], anchor[ARG_SIZE], borderColor[ARG_SIZE]; 
    sscanf(line_buffer, "%d %lf %lf %s %s %c %s", &id, &x, &y, borderColor, fillColor, anchor, text);

    FormStyle style = new_form_style(borderColor, fillColor, NULL, NULL, anchor); 
    Text textForm = new_text(id, x, y, text, style);
    if (textForm == NULL) {
        fprintf(stderr, "(ERROR) form: couldn't create text with id %d", id);
        return NULL; 
    }

    return new_form_info(TEXT, textForm); 
}

static Info process_line(char *line_buffer) {
    int id; 
    double x, y, x2, y2;
    char borderColor[ARG_SIZE]; 
    sscanf(line_buffer, "%d %lf %lf %lf %lf %s", &id, &x, &y, &x2, &y2, borderColor);
    FormStyle style = new_form_style(borderColor, NULL, NULL, NULL, NULL); 
    Line line = new_line(id, x, y, x2, y2, style);
    if (line == NULL) {
        fprintf(stderr, "(ERROR) form: couldn't create line with id %d", id);
        return NULL; 
    }

    return new_form_info(LINE, line); 
}

FormInfo process_form(char *formType, char *line_buffer) {
    if (formType == NULL || line_buffer == NULL) {
        fprintf(stderr, "(ERROR) form: the form type or line buffer is NULL");
        return NULL; 
    }

    if (strcmp(formType, "ts") == 0) 
        return NULL;
    else if (strcmp(formType, "r") == 0) 
        return process_rect(line_buffer);
    else if (strcmp(formType, "c") == 0) 
        return process_circle(line_buffer); 
    else if (strcmp(formType, "l") == 0) 
        return process_line(line_buffer);
    else if (strcmp(formType, "t") == 0) 
        return process_text(line_buffer);
    else {
        fprintf(stderr, "(ERROR) form: the .geo form type informed is invalid: %s", formType); 
    }

    return NULL; 
}