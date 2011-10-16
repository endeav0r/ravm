#include "vm.h"

int vm_initialize (struct _vm * vm) {
    int i;
    
    for (i = 0; i < 10; i++) {
        vm->reg[i] = 0x00000000;
    }
    
    vm->reg[RSP] = VM_MEMORY_SIZE - 4;
    vm->reg[RBP] = VM_MEMORY_SIZE - 4;
    vm->IP = 0;
    vm->FLAGS = 0;
    
    vm->step = 0;
    
    vm->bps = NULL;
    vm->last_bp = NULL;
    
    vm->instruction_count = 0;
    
    memset(vm->memory, 0, VM_MEMORY_SIZE * sizeof(unsigned char));

    return 0;
}


int vm_breakpoint_add (struct _vm * vm, int address) {
    struct _bp * bp;
    
    if (vm->bps == NULL) {
        vm->bps = (struct _bp *) malloc(sizeof(struct _bp));
        bp = vm->bps;
    }
    else {
        bp = vm->bps;
        while (bp->next != NULL)
            bp = bp->next;
        bp->next = (struct _bp *) malloc(sizeof(struct _bp));
        bp = bp->next;
    }
    
    bp->address = address;
    bp->next = NULL;
    
    return 0;
}


int vm_breakpoint_del (struct _vm * vm, int address) {
    struct _bp * bp, * prev;
    
    if (vm->bps == NULL)
        return ERROR_VM_BP_NOT_FOUND;

    if (vm->bps->address == address) {
        bp = vm->bps->next;
        free(vm->bps);
        vm->bps = bp;
        return 0;
    }
    
    bp = vm->bps->next;
    prev = vm->bps;
    while (bp != NULL) {
        if (bp->address == address) {
            prev->next = bp->next;
            free(bp);
            return 0;
        }
        prev = bp;
        bp = bp->next;
    }
    return ERROR_VM_BP_NOT_FOUND;
}


int vm_breakpoint_check (struct _vm * vm) {
    struct _bp * bp;
    
    bp = vm->bps;
    while (bp != NULL) {
        if ((bp->address == vm->IP) && (bp != vm->last_bp)) {
            vm->last_bp = bp;
            return 1;
        }
        bp = bp->next;
    }
    return 0;
}

int vm_run (struct _vm * vm) {
    unsigned char op = 0x90;
    unsigned char rd, rs;
    int constant;

    while (vm->memory[vm->IP] != 0x80) {
    
        if (vm_breakpoint_check(vm))
            return 1;
        vm->last_bp = NULL;
        
        op = vm->memory[vm->IP++];
        switch (op) {
            // |op|rd|rs|
            case OP_ADDR  :
            case OP_SUB   :
            case OP_MUL   :
            case OP_DIV   :
            case OP_MOD   :
            case OP_ANDR  :
            case OP_ORR   :
            case OP_XORR  :
            case OP_MOVR  :
            case OP_MOVL  :
            case OP_MOVS  :
            case OP_MOVLB :
            case OP_MOVSB :
            case OP_CMPR  :
                rd = vm->memory[vm->IP++];
                rs = vm->memory[vm->IP++];
            switch (op) {
                case OP_ADDR :
                    vm->reg[rd] += vm->reg[rs];
                    break;
                case OP_SUB :
                    vm->reg[rd] -= vm->reg[rs];
                    break;
                case OP_MUL :
                    vm->reg[rd] *= vm->reg[rs];
                    break;
                case OP_DIV :
                    vm->reg[rd] /= vm->reg[rs];
                    break;
                case OP_MOD :
                    vm->reg[rd] %= vm->reg[rs];
                    break;
                case OP_ANDR :
                    vm->reg[rd] &= vm->reg[rs];
                    break;
                case OP_ORR :
                    vm->reg[rd] |= vm->reg[rs];
                    break;
                case OP_XORR :
                    vm->reg[rd] ^= vm->reg[rs];
                    break;
                case OP_MOVR :
                    vm->reg[rd] = vm->reg[rs];
                    break;
                case OP_MOVL :
                    vm->reg[rd] = *((int*) &(vm->memory[vm->reg[rs]]));
                    break;
                case OP_MOVS :
                    *((int*) &(vm->memory[vm->reg[rd]])) = vm->reg[rs];
                    break;
                case OP_MOVLB :
                    vm->reg[rd] = (vm->memory[vm->reg[rs]]) & 0x000000ff;
                    break;
                case OP_MOVSB :
                    vm->memory[vm->reg[rd]] = (unsigned char) (vm->reg[rs]);
                    break;
                case OP_CMPR :
                    vm->FLAGS = vm->reg[rd] - vm->reg[rs];
                    break;
            }
            break;
            // |op|co|ns|ta|nt|
            case OP_JMP   :
            case OP_JE    :
            case OP_JG    :
            case OP_JZ    :
            case OP_CALL  :
            case OP_PUSHC :
                constant = 0x00000000;
                constant |= vm->memory[vm->IP++] << 24;
                constant |= vm->memory[vm->IP++] << 16;
                constant |= vm->memory[vm->IP++] << 8;
                constant |= vm->memory[vm->IP++];
                switch (op) {
                    case OP_JMP :
                        vm->IP += constant;
                        break;
                    case OP_JE :
                    case OP_JZ :
                        if (vm->FLAGS == 0) vm->IP += constant;
                        break;
                    case OP_JG :
                        if (vm->FLAGS > 0) vm->IP += constant;
                        break;
                    case OP_CALL :
                        vm->reg[RSP] -= 4;
                        vm->memory[vm->reg[RSP]] = vm->IP;
                        vm->IP += constant;
                        break;
                    case OP_PUSHC :
                        vm->reg[RSP] -= 4;
                        *((int*) &(vm->memory[vm->reg[RSP]])) = constant;
                        break;
                }
                break;
            // |op|rd|co|ns|ta|nt|
            case OP_ADDC :
            case OP_MULC :
            case OP_DIVC :
            case OP_MODC :
            case OP_ANDC :
            case OP_ORC  :
            case OP_XORC :
            case OP_MOVC :
            case OP_CMPC :
                rd = vm->memory[vm->IP++];
                constant = 0x00000000;
                constant |= vm->memory[vm->IP++] << 24;
                constant |= vm->memory[vm->IP++] << 16;
                constant |= vm->memory[vm->IP++] << 8;
                constant |= vm->memory[vm->IP++];
                switch (op) {
                    case OP_ADDC :
                        vm->reg[rd] += constant;
                        break;
                    case OP_MULC :
                        vm->reg[rd] *= constant;
                        break;
                    case OP_DIVC :
                        vm->reg[rd] /= constant;
                        break;
                    case OP_MODC :
                        vm->reg[rd] %= constant;
                        break;
                    case OP_ANDC :
                        vm->reg[rd] &= constant;
                        break;
                    case OP_ORC :
                        vm->reg[rd] |= constant;
                        break;
                    case OP_XORC :
                        vm->reg[rd] ^= constant;
                        break;
                    case OP_MOVC :
                        vm->reg[rd] = constant;
                        break;
                    case OP_CMPC :
                        vm->FLAGS = vm->reg[rd] - constant;
                        break;
                }
                break;
            // |op|rs|
            case OP_PUSHR :
            case OP_POPR :
                rs = vm->memory[vm->IP++];
                switch (op) {
                    case OP_PUSHR :
                        vm->reg[RSP] -= 4;
                        *((int*) &(vm->memory[vm->reg[RSP]])) = (int) vm->reg[rs];
                        break;
                    case OP_POPR :
                        vm->reg[rs] = *((int*) &(vm->memory[vm->reg[RSP]]));
                        vm->reg[RSP] += 4;
                        break;
                }
                break;
            // ret
            case OP_RET :
                vm->IP = *((int*) &(vm->memory[vm->reg[RSP]]));
                vm->reg[RSP] += 4;
                break;
            // nop, hlt
            case OP_HLT :
            case OP_NOP :
                break;
            default :
                return ERROR_VM_UNKNOWN_OP;
        }
        
        vm->instruction_count++;
        
        if (vm->step == 1)
            return 2;
    }
    return 0;
}



int main (int argc, char * argv[]) {
    char * filename = NULL;
    char * output_filename = NULL;
    int opt_god_mode = 0;
    int c;
    int memory_view_offset = VM_MEMORY_SIZE - 32;
    int memory_view_bytes  = 32;
    int print_info = 0;
    int step = 0;
    int error;
    struct _vm * vm;
    
    while ((c = getopt(argc, argv, "b:gi:m:o:sp")) != -1) {
        switch (c) {
            case 'b' :
                memory_view_bytes = strtoul(optarg, NULL, 16);
                break;
            case 'g' :
                opt_god_mode = 1;
                break;
            case 'i' :
                filename = optarg;
                break;
            case 'm' :
                memory_view_offset = strtoul(optarg, NULL, 16);
                break;
            case 'o' :
                output_filename = optarg;
                break;
            case 'p' :
                print_info = 1;
                break;
            case 's' :
                step = 1;
                print_info = 1;
                break;
            case '?' :
                if ((optopt == 'f') || (optopt == 'o')) {
                    fprintf(stderr, "option %c requires argument\n", optopt);
                    exit(0);
                }
                else {
                    fprintf(stderr, "Unknown option: %c\n", optopt);
                    exit(0);
                }
        }
    }
    
    if (filename == NULL) {
        fprintf(stderr, "Usage: %s [-ps] [-o output] -i image\n", argv[0]);
        fprintf(stderr, "Runs an assembled image for the rnp_vm\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "  -b [hex]   BYTES of memory to view in debug output\n");
        fprintf(stderr, "  -g         god mode allows visualization of memory\n");
        fprintf(stderr, "  -i [path]  path to IMAGE\n");
        fprintf(stderr, "  -m [hex]   OFFSET in memory to view in debug output\n");
        fprintf(stderr, "  -o [path]  path to OUTPUT memory dump at HLT\n");
        fprintf(stderr, "  -p         PRINT info at each step\n");
        fprintf(stderr, "  -s         STEP through instruction (implies PRINT)\n");
        exit(0);
    }
    
    vm = (struct _vm *) malloc(sizeof(struct _vm));
    vm_initialize(vm);
    
    if ((error = image_load(vm, filename)) != 0) {
        fprintf(stderr, "error %d\n", error);
        exit(error);
    }
    
    if (opt_god_mode) {
        god_mode(vm);
    }
    else {    
        if (step | print_info)
            vm->step = 1;

        if (print_info) {
            debug_view_memory(vm,
                              memory_view_offset,
                              memory_view_bytes);
            debug_view_registers(vm);
            fflush(stdout);
            printf("%s\n", debug_instruction_description(&(vm->memory[vm->IP])));
            printf("\n");
        }

        while (vm_run(vm)) {
            if (print_info) {
                debug_view_memory(vm,
                                  memory_view_offset,
                                  memory_view_bytes);
                debug_view_registers(vm);
                fflush(stdout);
                printf("%s\n", debug_instruction_description(&(vm->memory[vm->IP])));
                printf("\n");
            }
            if (step) getc(stdin);
        }
    }
    
    if (output_filename != NULL) {
        if ((error = image_dump(vm, output_filename)) != 0) {
            fprintf(stderr, "error dumping memory to file: %d\n", error);
            exit(error);
        }
    }
    
    free(vm);
    
    return 0;
}
