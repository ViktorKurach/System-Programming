#include "allocator.h"

void *mem_alloc(size_t size) {
	int i = 0, k, div;
	int prev = MEMORY[i+3] + (MEMORY[i+2] << 8) + (MEMORY[i+1] << 16) + (MEMORY[i] << 24);
	int next = MEMORY[i+7] + (MEMORY[i+6] << 8) + (MEMORY[i+5] << 16) + (MEMORY[i+4] << 24);
	size_t avail_memory = next - i - 8;
	if (size == 0) return NULL;
	// Searching for available memory block
	while (size + 8 > avail_memory || (MEMORY[i] & 0x80) != 0) {
		prev = i;
		i = next;
		if (i >= MEM_SIZE) return NULL;
		next = MEMORY[i+7] + (MEMORY[i+6] << 8) + (MEMORY[i+5] << 16) + (MEMORY[i+4] << 24);
		avail_memory = next - i - 8;
	}
	div = i + size + 8;
	while (div % 4 != 0) ++div;
	// Updating new block caption
	MEMORY[i] = (prev & 0xff000000) >> 24;
	MEMORY[i] = MEMORY[i] | 0x80;
	MEMORY[i+1] = (prev & 0xff0000) >> 16;
	MEMORY[i+2] = (prev & 0xff00) >> 8;
	MEMORY[i+3] = prev & 0xff;
	MEMORY[i+4] = (div & 0xff000000) >> 24;
	MEMORY[i+5] = (div & 0xff0000) >> 16;
	MEMORY[i+6] = (div & 0xff00) >> 8;
	MEMORY[i+7] = div & 0xff;
	// Updating data
	for (k = i + 8; k < div; ++k) MEMORY[k] = 0x11; 
	// Division of memory
	MEMORY[div] = (i & 0xff000000) >> 24;
	MEMORY[div+1] = (i & 0xff0000) >> 16;
	MEMORY[div+2] = (i & 0xff00) >> 8;
	MEMORY[div+3] = i & 0xff;
	MEMORY[div+4] = (next & 0xff000000) >> 24;
	MEMORY[div+5] = (next & 0xff0000) >> 16;
	MEMORY[div+6] = (next & 0xff00) >> 8;
	MEMORY[div+7] = next & 0xff;
	// Updating prev block caption
	if (prev != 0) {
		MEMORY[prev+4] = (i & 0xff000000) >> 24;
		MEMORY[prev+5] = (i & 0xff0000) >> 16;
		MEMORY[prev+6] = (i & 0xff00) >> 8;
		MEMORY[prev+7] = i & 0xff;
	}
	// Updating next block caption
	if (next + 8 <= MEM_SIZE) {
		MEMORY[next] = (div & 0xff000000) >> 24;
		MEMORY[next+1] = (div & 0xff0000) >> 16;
		MEMORY[next+2] = (div & 0xff00) >> 8;
		MEMORY[next+3] = div & 0xff;
	}
	return MEMORY + i;
}

void *mem_realloc(void *addr, size_t size) {
	int i = 0;
	void* new_addr;
	while (MEMORY + i != addr) {
		++i;
		if (i == MEM_SIZE) return NULL;
	}
	new_addr = mem_alloc(size);
	if (new_addr != NULL) mem_free(addr);
	return new_addr;
}

void mem_free(void* addr) {
	int i = 0, k, prev, next, nextnext;
	unsigned char alloc_flag;
	// Checking whether addr is memory block address
	while (MEMORY + i != addr) {
		++i;
		if (i == MEM_SIZE) return;
	}
	// Freeing memory
	MEMORY[i] = MEMORY[i] & 0x7f;
	prev = MEMORY[i+3] + (MEMORY[i+2] << 8) + (MEMORY[i+1] << 16) + (MEMORY[i] << 24);
	next = MEMORY[i+7] + (MEMORY[i+6] << 8) + (MEMORY[i+5] << 16) + (MEMORY[i+4] << 24);
	for (k = i + 8; k < next; ++k) MEMORY[k] = 0;
	// Merging with previous free memory block
	if ((MEMORY[prev] & 0x80) == 0) {
		for (k = 4; k < 8; ++k) MEMORY[prev+k] = MEMORY[i+k];
		for (k = 0; k < 8; ++k) MEMORY[i+k] = 0;
		alloc_flag = MEMORY[next];
		MEMORY[next] = ((prev & 0xff000000) >> 24) | (alloc_flag & 0x80);
		MEMORY[next+1] = (prev & 0xff0000) >> 16;
		MEMORY[next+2] = (prev & 0xff00) >> 8;
		MEMORY[next+3] = prev & 0xff;
		i = prev;
	}
	// Merging with next free memory block
	if ((MEMORY[next] & 0x80) == 0) {
		nextnext = MEMORY[next+7] + (MEMORY[next+6] << 8) + (MEMORY[next+5] << 16) + (MEMORY[next+4] << 24);
		for (k = 4; k < 8; ++k) MEMORY[i+k] = MEMORY[next+k];
		alloc_flag = MEMORY[nextnext];
		for (k = 0; k < 4; ++k) MEMORY[nextnext+k] = MEMORY[next+k];
		MEMORY[nextnext] = MEMORY[nextnext] | (alloc_flag & 0x80);
		for (k = 0; k < 8; ++k) MEMORY[next+k] = 0;
	}
}

void mem_dump() {
	int i;
	for (i = 0; i < MEM_SIZE; ++i) {
		if (i % 16 == 0) printf("\n%0*x\t|", 4, i);
		if (i % 4 == 0) printf(" ");
		printf("%0*x ", 2, MEMORY[i]);
	}
	printf("\n");
}