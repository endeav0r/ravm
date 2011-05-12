#ifndef god_mode_HEADER
#define god_mode_HEADER

#include <curses.h>
#include <signal.h>

#include "debug.h"
#include "vm.h"


#define GM_COLOR       1
#define GM_COLOR_CUR   2
#define GM_COLOR_STACK 3
#define GM_COLOR_RSP   4
#define GM_COLOR_RBP   5
#define GM_COLOR_TEXT  6
#define GM_COLOR_IP    7


int god_mode (struct _vm * vm);

#endif
