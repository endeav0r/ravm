#include "god_mode.h"

int cur = 0;

void god_mode_finish () {
    endwin();
    exit(0);
}


void god_mode_resize () {
    //resizeterm();
}


void god_mode_init () {
    (void) signal(SIGINT, god_mode_finish);      /* arrange interrupts to terminate */
    signal (SIGWINCH, god_mode_resize);

    (void) initscr();      /* initialize the curses library */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    //(void) echo();       /* echo input - in color */
    noecho();
    keypad(stdscr, TRUE);  /* enable keyboard mapping */

    start_color();
    
    init_pair(GM_COLOR,       COLOR_WHITE, COLOR_BLACK  );
    init_pair(GM_COLOR_CUR,   COLOR_BLACK, COLOR_YELLOW );
    init_pair(GM_COLOR_STACK, COLOR_WHITE, COLOR_BLUE   );
    init_pair(GM_COLOR_RSP,   COLOR_WHITE, COLOR_RED    );
    init_pair(GM_COLOR_RBP,   COLOR_WHITE, COLOR_MAGENTA);
    init_pair(GM_COLOR_TEXT,  COLOR_WHITE, COLOR_CYAN   );
    init_pair(GM_COLOR_IP  ,  COLOR_WHITE, COLOR_GREEN  );
    
    refresh();
}


void god_mode_draw (struct _vm * vm) {
    char buf[128];
    int i;
    int instruction_size;
    unsigned int word;
    unsigned short c;
    unsigned short color;
    int line_width, offset, y, x;
    
    //clear();
    
    line_width = COLS / 9;
    offset = 0;
    // start by drawing memory to the screen
    for (y = 0; y < LINES - 3; y++) {
        if (offset >= VM_MEMORY_SIZE)
            break;
        sprintf(buf, "%08x", offset);
        mvaddstr(y, 0, buf);
        for (x = 1; x < COLS / 9; x++) {
        
            if (offset >= VM_MEMORY_SIZE)
                break;
            
            if (offset == cur)
                color = GM_COLOR_CUR;
            else if (offset < vm->text_size)
                color = GM_COLOR_TEXT;
            else if (offset == vm->reg[RSP])
                color = GM_COLOR_RSP;
            else if (offset == vm->reg[RBP])
                color = GM_COLOR_RBP;
            else if (offset > vm->reg[RSP])
                color = GM_COLOR_STACK;
            else
                color = GM_COLOR;
            
            word = *((int*) &(vm->memory[offset]));
            
            sprintf(buf, "%08x", b2lendian(word));
            
            for (i = 0; i < 8; i++) {
                c = buf[i];
                
                // special rules for picking instructions out of words
                // if invalid instruction size, default to 1
                if ((instruction_size = debug_instruction_size(vm->memory[vm->IP])) == -1)
                    instruction_size = 1;
                if ((offset + i/2 >= vm->IP) && (offset + i/2
                     < vm->IP + instruction_size))
                    c |= COLOR_PAIR(GM_COLOR_IP);
                else {
                    c |= COLOR_PAIR(color);
                }
                
                mvaddch(y, x*9 + i, c);
            }
            offset += 4;
        }
    }
    
    // now draw the registers
    sprintf(buf, "r0=%08x r1=%08x r2=%08x r3=%08x  r4=%08x\n" \
                 "r5=%08x r6=%08x r7=%08x rsp=%08x rbp=%08x",
            vm->reg[R0], vm->reg[R1], vm->reg[R2], vm->reg[R3], vm->reg[R4],
            vm->reg[R5], vm->reg[R6], vm->reg[R7], vm->reg[RSP], vm->reg[RBP]);
    mvaddstr(LINES - 2, 0, buf);
    // draw cursor location and instruction description
    sprintf(buf, "%08x IP=%08x %s\n", cur, vm->IP,
            debug_instruction_description(&(vm->memory[vm->IP])));
    mvaddstr(LINES - 3, 0, buf);
    refresh();
}


int god_mode (struct _vm * vm) {
    int c;
    int words_per_line = 0;
    
    god_mode_init();
    
    vm->step = 1;
    while (1) {
        god_mode_draw(vm);
        c = getch();
        words_per_line = COLS / 9 - 1;
        switch (c) {
            case KEY_UP :
                cur -= words_per_line * 4;
                if (cur < 0) cur = 0;
                break;
            case KEY_DOWN :
                cur += words_per_line * 4;
                if (cur > VM_MEMORY_SIZE - 4) cur = VM_MEMORY_SIZE - 4;
                break;
            case KEY_LEFT :
                if (cur > 3) cur -= 4;
                break;
            case KEY_RIGHT :
                if (cur < VM_MEMORY_SIZE - 4) cur += 4;
                break;
            case 's' :
                vm_run(vm);
                break;  
            case 'q':
                god_mode_finish();
                break;
        }
    }
    endwin();
    return 0;
}
