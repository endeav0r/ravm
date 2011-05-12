#ifndef label_HEADER
#define label_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
struct _label_loc {
    int loc;
    struct _label_loc * next;
}
*/

struct _label {
    char * text;
    int location;
    struct _label * next;
};

// always returns a valid label (creates one if it doesnt exist)
struct _label * label_get  (struct _label ** labels, const char * text);

// returns NULL if label not found
struct _label * label_find (struct _label * labels, const char * text);

#endif
