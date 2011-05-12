#ifndef image_HEADER
#define image_HEADER

#include <stdio.h>

#include "error.h"
#include "vm.h"

int image_load (struct _vm * vm, const char * filename);
int image_dump (struct _vm * vm, const char * filename);

#endif
