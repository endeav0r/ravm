#ifndef parameter_list_HEADER
#define parameter_list_HEADER

#include "lexer.h"
#include "memory_definition.h"

#define PARAMETER_STRING 1
#define PARAMETER_WORD   2

struct _parameter_list {
    int type;
    int bytes; // this will only be set in the FIRST node in the list, and will
               // be set to the sum of the size in bytes of all elements
               // this includes the 0x00 to null-terminate strings
    char * string;
    int word;
    
    struct _parameter_list * next;
};

struct _parameter_list * parameter_list_create (struct _token * tokens);

#endif
