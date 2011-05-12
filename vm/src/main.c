#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"
#include "image.h"
#include "vm.h"

int main (int argc, char * argv[]) {
    char * filename = NULL;
    char * output_filename = NULL;
    int c;
    int memory_view_offset = VM_MEMORY_SIZE - 32;
    int memory_view_bytes  = 32;
    int print_info = 0;
    int step = 0;
    int error;
    struct _vm * vm;
    
    while ((c = getopt(argc, argv, "b:i:m:o:sp")) != -1) {
        switch (c) {
            case 'b' :
                memory_view_bytes = strtol(optarg, NULL, 16);
                break;
            case 'i' :
                filename = optarg;
                break;
            case 'm' :
                memory_view_offset = strtol(optarg, NULL, 16);
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
    
    if (step | print_info)
        vm->step = 1;

    while (vm_run(vm)) {
        if (print_info) {
            printf("%s\n", debug_instruction_description(&(vm->memory[vm->IP])));
            debug_view_memory(vm,
                              memory_view_offset,
                              memory_view_bytes);
            debug_view_registers(vm);
        }
        if (step) getc(stdin);
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
