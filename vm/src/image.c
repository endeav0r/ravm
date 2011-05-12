#include "image.h"

int image_load (struct _vm * vm, const char * filename) {
    int filesize;
    int read;
    FILE * fh;
    
    fh = fopen(filename, "rb");
    if (fh == NULL)
        return ERROR_IMAGE_FILE_IO;
    
    fseek(fh, 0, SEEK_END);
    filesize = (int) ftell(fh);
    fseek(fh, 0, SEEK_SET);
    
    if (filesize > VM_MEMORY_SIZE) {
        fclose(fh);
        return ERROR_IMAGE_TOO_BIG;
    }
    
    read = fread(vm->memory, filesize, 1, fh);
    
    fclose(fh);
    
    if (read != 1)
        return ERROR_IMAGE_FILE_IO;
    return 0;
}


int image_dump (struct _vm * vm, const char * filename) {
    FILE * fh;
    
    fh = fopen(filename, "wb");
    if (fh == NULL)
        return ERROR_IMAGE_FILE_IO;
    
    fwrite(vm->memory, 1, VM_MEMORY_SIZE, fh);
    
    fclose(fh);
    
    return 0;
}
