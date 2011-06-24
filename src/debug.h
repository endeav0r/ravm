#ifndef debug_HEADER
#define debug_HEADER

#include <stdio.h>

#include "vm.h"


struct _vm;

unsigned int b2lendian (unsigned int x);

int debug_view_memory (struct _vm * vm, int address, int bytes);
int debug_view_registers (struct _vm * vm);

int debug_instruction_size (const unsigned char instruction);

const char * debug_register_description (const unsigned char reg);
const char * debug_instruction_description (const unsigned char * instruction);
#endif
