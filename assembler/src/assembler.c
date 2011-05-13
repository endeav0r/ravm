#include <stdio.h>
#include <string.h>

#include "label.h"
#include "lexer.h"
#include "parser.h"

#define l2bendian(x) ((((x) << 24) & 0xff000000) | (((x) << 8) & 0xff0000) \
                      | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0x000000ff))

char * assembler_read_file (const char * filename) {
    char * text;
    FILE * fh;
    int filesize;
    int bytes_read;
    
    fh = fopen(filename, "r");
    if (fh == NULL) { 
        fprintf(stderr, "failed to open file\n");
        exit(0);
    }
    
    fseek(fh, 0, SEEK_END);
    filesize = (int) ftell(fh);
    fseek(fh, 0, SEEK_SET);
    
    text = (char *) malloc(filesize + 1);
    
    text[filesize] = 0;
    
    bytes_read = fread(text, 1, filesize, fh);

    return text;
}


int assemble (struct _parser * parser, const char * filename) {
    int endian_tmp;
    FILE * fh;
    struct _label * label;
    struct _instruction * instruction;
    
    fh = fopen(filename, "wb");
    if (fh == NULL) { 
        fprintf(stderr, "failed to open file\n");
        exit(0);
    }
    
    instruction = parser->instructions;
    while (instruction != NULL) {
        fwrite(&(instruction->op), 1, 1, fh);
        switch (instruction->op) {
            case OP_ADDR :
            case OP_SUB :
            case OP_MUL :
            case OP_DIV :
            case OP_MOD :
            case OP_ANDR :
            case OP_ORR  :
            case OP_XORR :
            case OP_MOVR :
            case OP_CMPR :
            case OP_MOVL :
            case OP_MOVS :
                fwrite(&(instruction->rd), 1, 1, fh);
                fwrite(&(instruction->rs), 1, 1, fh);
                break;
            case OP_ADDC :
            case OP_MULC :
            case OP_DIVC :
            case OP_MODC :
            case OP_ANDC :
            case OP_ORC  :
            case OP_XORC :
            case OP_MOVC :
            case OP_CMPC :
                fwrite(&(instruction->rd), 1, 1, fh);
                endian_tmp = l2bendian(instruction->constant);
                fwrite(&endian_tmp, 4, 1, fh);
                break;
            case OP_JMP :
            case OP_JZ :
            case OP_JE :
            case OP_JG :
            case OP_CALL :
                label = instruction->jump;
                if (label->location == -1) {
                    fprintf(stderr, "no location for label: %s\n", label->text);
                    exit(0);
                }
                // because the jump will take place *after* this operation has
                // executed, we subtract 5 to account for the 5 bytes of these
                // operations
                endian_tmp = l2bendian(label->location - instruction->location - 5);
                fwrite(&endian_tmp, 4, 1, fh);
                break;
            case OP_PUSH :
            case OP_POP :
                fwrite(&(instruction->rd), 1, 1, fh);
                break;
        }
        instruction = instruction->next;
    }
    
    fclose(fh);
    
    return 0;
}


int main (int argc, char * argv[]) {
    char * text;
    struct _token * tokens;
    struct _parser parser;
    
    memset(&parser, 0, sizeof(struct _parser));
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s <assembly_source> <binary_out>\n", argv[0]);
        exit(0);
    }
    
    text = assembler_read_file(argv[1]);
    
    lexer_init();
    tokens = lexer(text);
    parser_parse(&parser, tokens);
    
    assemble(&parser, argv[2]);
    
    lexer_tokens_delete(tokens);
    free(text);
       
    return 0;
}
