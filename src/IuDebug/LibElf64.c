#include "LibElf64.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// For Iu_Elf64::loaderTag
enum {
    Iu_Elf64LoaderNone,
    Iu_Elf64LoaderMmap,
};

typedef struct {
    void* data;
    size_t size;
} Iu_Elf64MmapLoader;

void Iu_InitElf64(Iu_Elf64* data) {
    memset(data, 0, sizeof(data));
    data->loaderTag = Iu_Elf64LoaderNone;
}

void Iu_DestroyElf64(Iu_Elf64* data) {
    switch (data->loaderTag) {
        case Iu_Elf64LoaderNone: break;
        
        case Iu_Elf64LoaderMmap: {
            Iu_Elf64MmapLoader* loader = data->loader;
            munmap(loader->data, loader->size);
        } break;
    }
    
    free(data->loader);
    free(data->sections);
    free(data->segments);
}

Iu_CreateElf64MmapLoader_err_t
Iu_CreateElf64MmapLoader(Iu_Elf64* data, int fd) {
    off_t end = lseek(fd, 0, SEEK_END);
    if (end == (off_t)-1) {
        return Iu_CreateElf64MmapLoader_err_IoError;
    }
    void* mmapData = mmap(NULL, end, PROT_READ, MAP_SHARED, fd, 0);
    if (mmapData == MAP_FAILED) {
        return Iu_CreateElf64MmapLoader_err_IoError;
    }
    close(fd);
    
    Iu_Elf64MmapLoader* loader = malloc(sizeof(Iu_Elf64MmapLoader));
    loader->data = mmapData;
    loader->size = end;
    
    data->loader = loader;
    data->loaderTag = Iu_Elf64LoaderMmap;
    
    return Iu_CreateElf64MmapLoader_err_Success;
}
