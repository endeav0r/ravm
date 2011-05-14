#ifndef parser_HEADER
#define parser_HEADER

#include <stdlib.h>
#include <string.h>

#include "label.h"
#include "lexer.h"
#include "memory_definition.h"
#include "parameter_list.h"

#define PARSER_RULES 41
#define PARSER_RULES_NONTERMS 64
#define PARSER_REGISTERS 10

// RULE_LABEL and RULE_DB are special
// RULE_LABEL deals with labels, and RULE_DB deals with memory definitions

#define RULE_ADDR  0
#define RULE_SUB   1
#define RULE_MUL   2
#define RULE_DIV   3
#define RULE_MOD   4
#define RULE_ADDC  5
#define RULE_MOVC  6
#define RULE_MOVR  7
#define RULE_MOVL  8
#define RULE_MOVS  9
#define RULE_JMP   10
#define RULE_JZ    11
#define RULE_JE    12
#define RULE_JG    13
#define RULE_CALL  14
#define RULE_RET   15
#define RULE_PUSH  16
#define RULE_POP   17
#define RULE_CMPR  18
#define RULE_CMPC  19
#define RULE_HLT   20
#define RULE_NOP   21
#define RULE_DB    22
#define RULE_ANDR  23
#define RULE_ANDC  24
#define RULE_ORR   25
#define RULE_ORC   26
#define RULE_XORR  27
#define RULE_XORC  28
#define RULE_MULC  29
#define RULE_DIVC  30
#define RULE_MODC  31
#define RULE_LABEL 32
#define RULE_ADDL  33
#define RULE_MULL  34
#define RULE_DIVL  35
#define RULE_ANDL  36
#define RULE_ORL   37
#define RULE_XORL  38
#define RULE_CMPL  39
#define RULE_MOVLA 40

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
#define OP_MULC 0x0D
#define OP_DIVC 0x0E
#define OP_MODC 0x0F
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

struct _instruction {
    unsigned char op;
    unsigned char rd;
    unsigned char rs;
    int constant;
    struct _label * label;
    
    int size; // of the instruction once assembled in bytes
    int location; // of this instruction in the object code
    
    struct _instruction * next;
};


struct _parser {
    int size; // of the instructions/TEXT in bytes. does not include the
              // memory_definitions/DATA
    struct _instruction * instructions;
    struct _instruction * last_instruction;
    struct _label       * labels;
    struct _memory_definition * memory_definitions;
};
    

int parser_parse (struct _parser * parser, struct _token * tokens);

#endif
