#include "debug.h"


int debug_view_memory (struct _vm * vm, int address, int bytes) {
    int i;
    
    for (i = 0; i < bytes; i += 4) {
        if (i % 16 == 0) {
            if (i > 0)
                printf("\n");
            printf("%08x ", address + i);
        }
        printf("%02x%02x%02x%02x ",         vm->memory[address + i + 3],
               vm->memory[address + i + 2], vm->memory[address + i + 1],
               vm->memory[address + i + 0]);
    }
    printf("\n");
    return 0;
}


int debug_view_registers (struct _vm * vm) {
    printf("r0 %08x    r1 %08x    r2 %08x    r3 %08x\n", vm->reg[R0],
           vm->reg[R1], vm->reg[R2], vm->reg[R3]);
    printf("r4 %08x    r5 %08x    r6 %08x    r7 %08x\n", vm->reg[R4],
           vm->reg[R5], vm->reg[R6], vm->reg[R7]);
    printf("RSP %08x  RBP %08x  RIP %08x\n", vm->reg[RSP], vm->reg[RBP],
           vm->IP);
    return 0;
}


const char * debug_register_description (const unsigned char reg) {
    switch (reg) {
        case R0 :
            return "r0";
        case R1 :
            return "r1";
        case R2 :
            return "r2";
        case R3 :
            return "r3";
        case R4 :
            return "r4";
        case R5 :
            return "r5";
        case R6 :
            return "r6";
        case R7 :
            return "r7";
        case RSP :
            return "rsp";
        case RBP :
            return "rbp";
    }
    return NULL;
}


char instruction_description[32];
const char * debug_instruction_description (const unsigned char * instruction) {
    char tmp[32];
    strcpy(instruction_description, "Instruction Not Found");
    switch (instruction[0]) {
        case OP_ADDR :
        case OP_ADDC :
            strcpy(instruction_description, "ADD ");
            break;
        case OP_SUB :
            strcpy(instruction_description, "SUB ");
            break;
        case OP_MUL :
            strcpy(instruction_description, "MUL ");
            break;
        case OP_DIV :
            strcpy(instruction_description, "DIV ");
            break;
        case OP_MOD :
            strcpy(instruction_description, "MOD ");
            break;
        case OP_MOVC :
        case OP_MOVR :
        case OP_MOVL :
        case OP_MOVS :
            strcpy(instruction_description, "MOV ");
            break;
        case OP_JMP :
            strcpy(instruction_description, "JMP ");
            break;
        case OP_JZ :
            strcpy(instruction_description, "JZ ");
            break;
        case OP_JE :
            strcpy(instruction_description, "JE ");
            break;
        case OP_JG :
            strcpy(instruction_description, "JG ");
            break;
        case OP_CALL :
            strcpy(instruction_description, "CALL ");
            break;
        case OP_RET :
            strcpy(instruction_description, "RET ");
            break;
        case OP_PUSH :
            strcpy(instruction_description, "PUSH ");
            break;
        case OP_POP :
            strcpy(instruction_description, "POP ");
            break;
        case OP_CMPR :
        case OP_CMPC :
            strcpy(instruction_description, "CMP ");
            break;
        case OP_HLT :
            strcpy(instruction_description, "HLT ");
            break;
        case OP_NOP :
            strcpy(instruction_description, "NOP ");
            break;
    }
    switch (instruction[0]) {
        case OP_ADDR :
        case OP_MOVR :
        case OP_SUB :
        case OP_MUL :
        case OP_DIV :
        case OP_MOD :
        case OP_CMPR :
            strcat(instruction_description, debug_register_description(instruction[1]));
            strcat(instruction_description, ", ");
            strcat(instruction_description, debug_register_description(instruction[2]));
            break;
        case OP_ADDC :
        case OP_MOVC :
        case OP_CMPC :
            strcat(instruction_description, debug_register_description(instruction[1]));
            sprintf(tmp, ", %d", b2lendian(*((int*)&(instruction[2]))));
            strcat(instruction_description, tmp);
            break;
        case OP_PUSH :
        case OP_POP :
            strcat(instruction_description, debug_register_description(instruction[1]));
            break;
        case OP_MOVS :
            strcat(instruction_description, "[");
            strcat(instruction_description, debug_register_description(instruction[1]));
            strcat(instruction_description, "], ");
            strcat(instruction_description, debug_register_description(instruction[2]));
            break;
        case OP_MOVL :
            strcat(instruction_description, debug_register_description(instruction[1]));
            strcat(instruction_description, ", [");
            strcat(instruction_description, debug_register_description(instruction[2]));
            strcat(instruction_description, "]");
            break;
        case OP_JMP :
        case OP_JZ :
        case OP_JE :
        case OP_JG :
        case OP_CALL :
            sprintf(tmp, "%d", b2lendian(*((int*)&(instruction[1]))));
            strcat(instruction_description, tmp);
            break;
    }
    return instruction_description;
}
