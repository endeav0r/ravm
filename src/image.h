#ifndef image_HEADER
#define image_HEADER

#include <stdio.h>

#include "error.h"
#include "vm.h"

struct _vm;

int image_load (struct _vm * vm, const char * filename);
int image_dump (struct _vm * vm, const char * filename);

#endif
