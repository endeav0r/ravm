#ifndef parser_HEADER
#define parser_HEADER

#include <stdlib.h>
#include <string.h>

#include "label.h"
#include "lexer.h"
#include "memory_definition.h"
#include "parameter_list.h"
#include "vm.h"

#define PARSER_RULES 44
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
#define RULE_PUSHR 16
#define RULE_POPR  17
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
#define RULE_MOVLB 41
#define RULE_MOVSB 42
#define RULE_PUSHC 43

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
