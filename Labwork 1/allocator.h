#include <stdio.h>
#include <malloc.h>

#define MEM_SIZE 256

unsigned char* MEMORY;

void *mem_alloc(size_t size);
void *mem_realloc(void *addr, size_t size);
void mem_free(void* addr);
void mem_dump();