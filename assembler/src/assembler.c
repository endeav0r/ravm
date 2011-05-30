#include <stdio.h>
#include <string.h>

#include "label.h"
#include "lexer.h"
#include "memory_definition.h"
#include "parameter_list.h"
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


// once all of the instructions have been added to the parser, we can go back
// and set the locations of the memory_definition labels in the DATA section,
// which will follow the TEXT section
int assembler_memory_definition_labels (struct _parser * parser) {
    struct _memory_definition * memory_definition;
    int location;
    
    location = parser->size;
    location += 4 - (location % 4); // align on a 4 byte boundary
    
    memory_definition = parser->memory_definitions;
    while (memory_definition != NULL) {
        memory_definition->label->location = location;
        location += memory_definition->parameter_list->bytes;
        location += 4 - (location % 4);
        memory_definition = memory_definition->next;
    }
    
    return 0;
}


int assemble (struct _parser * parser, const char * filename) {
    int endian_tmp;
    int location = 0;
    FILE * fh;
    struct _label * label;
    struct _instruction * instruction;
    struct _memory_definition * memory_definition;
    struct _parameter_list * parameter_list;
    
    fh = fopen(filename, "wb");
    if (fh == NULL) { 
        fprintf(stderr, "failed to open file\n");
        exit(0);
    }
    
    /*******
    * TEXT *
    *******/
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
            case OP_MOVLB :
            case OP_MOVSB :
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
                if (instruction->label == NULL) {
                    endian_tmp = l2bendian(instruction->constant);
                    fwrite(&endian_tmp, 4, 1, fh);
                }
                else {
                    label = instruction->label;
                    if (label->location == -1) {
                        fprintf(stderr, "no location for label: %s\n", label->text);
                        exit(0);
                    }
                    endian_tmp = l2bendian(label->location);
                    fwrite(&endian_tmp, 4, 1, fh);
                }
                break;
            case OP_JMP :
            case OP_JZ :
            case OP_JE :
            case OP_JG :
            case OP_CALL :
                label = instruction->label;
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
    
    
    /*******
    * DATA *
    *******/
    location = parser->size;
    memory_definition = parser->memory_definitions;
    while (memory_definition != NULL) {
        // align to 4 byte boundary
        endian_tmp = 0x00000000;
        fwrite(&(endian_tmp), 1, 4 - (location % 4), fh);
        location += 4 - (location % 4);
        
        parameter_list = memory_definition->parameter_list;
        while (parameter_list != NULL) {
            switch (parameter_list->type) {
                case PARAMETER_STRING :
                    fwrite(parameter_list->string, 1, strlen(parameter_list->string) + 1, fh);
                    location += strlen(parameter_list->string);
                    break;
                case PARAMETER_WORD :
                    endian_tmp = l2bendian(parameter_list->word);
                    fwrite(&endian_tmp, 1, sizeof(int), fh);
                    location += sizeof(int);
                    break;
            }
            parameter_list = parameter_list->next;
        }
        memory_definition = memory_definition->next;
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
    
    assembler_memory_definition_labels(&parser);
    
    assemble(&parser, argv[2]);
    lexer_tokens_delete(tokens);
    free(text);
       
    return 0;
}
