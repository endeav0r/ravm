#include "memory_definition.h"

struct _memory_definition * memory_definition_add (struct _memory_definition * memory_definitions,
                                                   struct _label * label,
                                                   struct _parameter_list * parameter_list) {
    struct _memory_definition * memory_definition;
    
    if (memory_definitions == NULL) {
        memory_definition = (struct _memory_definition *)
                            malloc(sizeof(struct _memory_definition));
        memory_definitions = memory_definition;
    }
    else {
        memory_definition = memory_definitions;
        while (memory_definition->next != NULL) {
            memory_definition = memory_definition->next;
        }
        memory_definition->next = (struct _memory_definition *)
                                  malloc(sizeof(struct _memory_definition));
        memory_definition = memory_definition->next;
    }
    
    memory_definition->label = label;
    memory_definition->parameter_list = parameter_list;
    memory_definition->next = NULL;
    
    return memory_definitions;
}
