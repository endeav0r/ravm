#ifndef vm_HEADER
#define vm_HEADER

#include <stdlib.h>
#include <string.h>

#include "error.h"

#define VM_MEMORY_SIZE 0x00001000

#define OP_ADDR 0x01
#define OP_SUB  0x02
#define OP_MUL  0x03
#define OP_DIV  0x04
#define OP_MOD  0x05
#define OP_ADDC 0x06
#define OP_ANDR 0x07
#define OP_ANDC 0x08
#define OP_ORR  0x09
#define OP_ORC  0x0A
#define OP_XORR 0x0B
#define OP_XORC 0x0C
#define OP_MOVC 0x10
#define OP_MOVR 0x11
#define OP_MOVL 0x12
#define OP_MOVS 0x13
#define OP_JMP  0x20
#define OP_JZ   0x21
#define OP_JE   0x22
#define OP_JG   0x23
#define OP_CALL 0x30
#define OP_RET  0x31
#define OP_PUSH 0x32
#define OP_POP  0x33
#define OP_CMPR 0x40
#define OP_CMPC 0x41
#define OP_HLT  0x80
#define OP_NOP  0x90

#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R4 0x04
#define R5 0x05
#define R6 0x06
#define R7 0x07
#define RSP 0x08
#define RBP 0x09


// breakpoint
struct _bp {
    int address;
    struct _bp * next;
};

struct _vm {
    unsigned char memory[VM_MEMORY_SIZE];
    int reg[10];
    int IP;
    int FLAGS;
    
    struct _bp * last_bp; // address of last bp we broke on
    struct _bp * bps;
    
    int step; // if set, we return after each instruction
};

int vm_initialize (struct _vm * vm);
int vm_breakpoint_add (struct _vm * vm, int address);
int vm_breakpoint_del (struct _vm * vm, int address);

// @return 1 if IP is on breakpoint, 0 otherwise
int vm_breakpoint_check (struct _vm * vm);

// @return 0 on HLT, 1 on breakpoint, 2 on step, errno otherwise
int vm_run (struct _vm * vm);

#endif
