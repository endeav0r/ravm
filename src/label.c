#include "label.h"

// adds a new label to the end of labels and returns the new label
struct _label * label_add (struct _label * labels, const char * text) {
    struct _label * l;
    
    // if no labels we are already at the end. create a label
    if (labels == NULL) {
        l = (struct _label *) malloc(sizeof(struct _label));
        labels = l;
    }
    else {
        l = labels;
        // find the end of the labels
        while (l->next != NULL) { 
            if (strcmp(l->text, text) == 0) {
                fprintf(stderr, "found duplicate label: %s\n", text);
                exit(0);
            }
            l = l->next;
        }
        // create a new label at the end of these labels
        l->next = (struct _label *) malloc(sizeof(struct _label));
        l = l->next;
    }
    // initialize this label
    l->text = (char *) malloc(strlen(text) + 1);
    strcpy(l->text, text);
    l->type = -1;
    l->location = -1;
    l->next = NULL;
    
    // return the new label
    return l;
}


struct _label * label_get (struct _label ** labels, const char * text) {
    struct _label * label;
    
    // if there are no labels, create one
    if (*labels == NULL) {
        *labels = label_add(*labels, text);
        label = *labels;
    }
    else {
        // search for this label and return it if it exists
        label = *labels;
        while (1) {
            if (strcmp(label->text, text) == 0)
                return label;
            if (label->next == NULL) break;
            label = label->next;
        }
        
        // if the label doesn't exist, create a new one
        label->next = label_add(label, text);
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
