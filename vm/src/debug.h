#ifndef debug_HEADER
#define debug_HEADER

#include <stdio.h>

#include "vm.h"

#define b2lendian(x) ((x << 24) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | (x >> 24))

int debug_view_memory (struct _vm * vm, int address, int bytes);
int debug_view_registers (struct _vm * vm);

const char * debug_instruction_description (const unsigned char * instruction);
#endif
