#ifndef token_HEADER
#define token_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXER_BUF_SIZE 128
#define LEXER_MATCH_TOKENS 35

#define TOKEN_ADD  0
#define TOKEN_SUB  1
#define TOKEN_MUL  2
#define TOKEN_DIV  3
#define TOKEN_MOD  4
#define TOKEN_MOV  5
#define TOKEN_JMP  6
#define TOKEN_JZ   7
#define TOKEN_JE   8
#define TOKEN_JG   9
#define TOKEN_CALL 10
#define TOKEN_RET  11
#define TOKEN_PUSH 12
#define TOKEN_POP  13
#define TOKEN_CMP  14
#define TOKEN_HLT  15
#define TOKEN_NOP  16
#define TOKEN_R0   17
#define TOKEN_R1   18
#define TOKEN_R2   19
#define TOKEN_R3   20
#define TOKEN_R4   21
#define TOKEN_R5   22
#define TOKEN_R6   23
#define TOKEN_R7   24
#define TOKEN_RSP  25
#define TOKEN_RBP  26
#define TOKEN_BRACKET_OPEN  27
#define TOKEN_BRACKET_CLOSE 28
#define TOKEN_COLON         29
#define TOKEN_TERMINATOR    30
#define TOKEN_SEPERATOR     31
#define TOKEN_AND 32
#define TOKEN_OR  33
#define TOKEN_XOR 34

#define TOKEN_REG 300

#define TOKEN_CONSTANT 200
#define TOKEN_LABEL    201

struct _match {
    char text[8];
    int type;
};

struct _token {
    int type;
    char * text;
    int constant;
    int line;
    
    struct _token * next;
};

void lexer_init();
struct _token * lexer (const char * input);
void lexer_tokens_delete (struct _token * token);

#endif
