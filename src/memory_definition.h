#ifndef memory_definition_HEADER
#define memory_definition_HEADER

#include "parameter_list.h"

struct _memory_definition {
    struct _label * label;
    struct _parameter_list * parameter_list;
    
    struct _memory_definition * next;
};

struct _memory_definition * memory_definition_add (struct _memory_definition * memory_definition,
                                                   struct _label * label,
                                                   struct _parameter_list * parameter_list);

#endif
