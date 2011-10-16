#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

void disassemble (unsigned char * bin, int bin_size) {
    int ip, i;
    int instruction_size;
    int constant;
    char tmp[32];
    
    ip = 0;
    while (ip < bin_size) {
        printf("%08x  ", ip);
        instruction_size = debug_instruction_size(bin[ip]);
        if (instruction_size < 1) {
            printf("%02x\n", bin[ip]);
            ip += 1;
        }
        else {
            for (i = instruction_size; i < 8; i++) {
                printf("  ");
            }
            for (i = 0; i < instruction_size; i++) {
                printf("%02x", bin[ip + i]);
            }
            printf("  ");
            
            switch (bin[ip]) {
                case OP_ADDR :
                case OP_ADDC :
                    printf("ADD  ");
                    break;
                case OP_SUB :
                    printf("SUB  ");
                    break;
                case OP_MUL  :
                case OP_MULC :
                    printf("MUL  ");
                    break;
                case OP_DIV  :
                case OP_DIVC :
                    printf("DIV  ");
                    break;
                case OP_MOD  :
                case OP_MODC :
                    printf("MOD  ");
                    break;
                case OP_ANDR :
                case OP_ANDC :
                    printf("AND  ");
                    break;
                case OP_ORR :
                case OP_ORC :
                    printf("OR  ");
                    break;
                case OP_XORR :
                case OP_XORC :
                    printf("XOR  ");
                    break;
                case OP_MOVC  :
                case OP_MOVR  :
                case OP_MOVL  :
                case OP_MOVS  :
                case OP_MOVLB :
                case OP_MOVSB :
                    printf("MOV  ");
                    break;
                case OP_JMP :
                    printf("JMP  ");
                    break;
                case OP_JZ :
                    printf("JZ   ");
                    break;
                case OP_JE :
                    printf("JE   ");
                    break;
                case OP_JG :
                    printf("JG   ");
                    break;
                case OP_CALL :
                    printf("CALL ");
                    break;
                case OP_RET :
                    printf("RET  ");
                    break;
                case OP_PUSHR :
                case OP_PUSHC :
                    printf("PUSH ");
                    break;
                case OP_POPR :
                    printf("POP  ");
                    break;
                case OP_CMPR :
                case OP_CMPC :
                    printf("CMP  ");
                    break;
                case OP_HLT :
                    printf("HLT  ");
                    break;
                case OP_NOP :
                    printf("NOP  ");
                    break;
            }
            
            switch (bin[ip]) {
                case OP_ADDR :
                case OP_MOVR :
                case OP_SUB  :
                case OP_MUL  :
                case OP_DIV  :
                case OP_MOD  :
                case OP_CMPR :
                case OP_ANDR :
                case OP_ORR  :
                case OP_XORR :
                    printf("%s, ", debug_register_description(bin[ip+1]));
                    printf("%s", debug_register_description(bin[ip+2]));
                    break;
                case OP_ADDC :
                case OP_MULC :
                case OP_DIVC :
                case OP_MODC :
                case OP_MOVC :
                case OP_CMPC :
                case OP_ANDC :
                case OP_ORC  :
                case OP_XORC :
                    printf("%s, ", debug_register_description(bin[ip+1]));
                    constant  = ((int) bin[ip+2]) << 24;
                    constant |= ((int) bin[ip+3]) << 16;
                    constant |= ((int) bin[ip+4]) << 8;
                    constant |= ((int) bin[ip+5]);
                    sprintf(tmp, "%d (%08x)", constant, ip + constant);
                    printf("%s", tmp);
                    break;
                case OP_PUSHR :
                case OP_POPR :
                    printf("%s", debug_register_description(bin[ip+1]));
                    break;
                case OP_MOVL :
                    printf("%s, ", debug_register_description(bin[ip+1]));
                    printf("[%s]", debug_register_description(bin[ip+2]));
                    break;
                case OP_MOVS :
                    printf("[%s], ", debug_register_description(bin[ip+1]));
                    printf("%s", debug_register_description(bin[ip+2]));
                    break;
                case OP_MOVLB :
                    printf("%s, BYTE [", debug_register_description(bin[ip+1]));
                    printf("%s]", debug_register_description(bin[ip+2]));
                    break;
                case OP_MOVSB :
                    printf("%s, BYTE [", debug_register_description(bin[ip+1]));
                    printf("%s]", debug_register_description(bin[ip+2]));
                    break;
                case OP_JMP :
                case OP_JZ :
                case OP_JE :
                case OP_JG :
                case OP_CALL :
                case OP_PUSHC :
                    constant  = ((int) bin[ip+1]) << 24;
                    constant |= ((int) bin[ip+2]) << 16;
                    constant |= ((int) bin[ip+3]) << 8;
                    constant |= ((int) bin[ip+4]);
                    sprintf(tmp, "%d (%08x)", constant, ip + constant + 5);
                    printf("%s", tmp);
                    break;
            }
            
            printf("\n");
            fflush(stdout);
            ip += instruction_size;
        }
    }
    
}

int main (int argc, char * argv[]) {
    int filesize;
    unsigned char * buf = NULL;
    FILE * fh;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <binary file>\n", argv[0]);
        exit(0);
    }
    
    fh = fopen(argv[1], "rb");
    if (fh == NULL) {
        fprintf(stderr, "error opening file %s\n", argv[1]);
        exit(0);
    }
    
    fseek(fh, 0, SEEK_END);
    filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    buf = (unsigned char *) malloc(filesize);

    fread(buf, 1, filesize, fh);

    disassemble(buf, filesize);
    
    free(buf);
    
    fclose(fh);   

    return 0;

}
