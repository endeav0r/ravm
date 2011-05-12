#include "label.h"


struct _label * label_add (struct _label * labels, const char * text) {
    struct _label * l;
    
    if (labels == NULL) {
        l = (struct _label *) malloc(sizeof(struct _label));
        labels = l;
    }
    else {
        l = labels;
        while (l->next != NULL) { 
            if (strcmp(l->text, text) == 0) {
                fprintf(stderr, "found duplicate label: %s\n", text);
                exit(0);
            }
            l = l->next;
        }
        l->next = (struct _label *) malloc(sizeof(struct _label));
        l = l->next;
    }
    l->text = (char *) malloc(strlen(text) + 1);
    strcpy(l->text, text);
    l->location = -1;
    l->next = NULL;
    return labels;
}


struct _label * label_get (struct _label ** labels, const char * text) {
    struct _label * label;
    
    if (*labels == NULL) {
        *labels = label_add(*labels, text);
        label = *labels;
    }
    else {
        label = *labels;
        while (1) {
            if (strcmp(label->text, text) == 0)
                return label;
            if (label->next == NULL) break;
            label = label->next;
        }
        
        label = label_add(label, text);
        label = label->next;
    }
    return label;
}


struct _label * label_find (struct _label * labels, const char * text) {
    struct _label * label;
    
    label = labels;
    while (label != NULL) {
        if (strcmp(label->text, text) == 0)
            break;
        label = label->next;
    }
    
    return label;
}
