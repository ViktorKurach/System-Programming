#include "allocator.h"

void init() {
	int i;
	MEMORY = (unsigned char*)malloc((MEM_SIZE + 4) * sizeof(unsigned char));
	for (i = 0; i < MEM_SIZE; ++i) MEMORY[i] = 0;
	MEMORY[7] = MEM_SIZE & 0xff;
	MEMORY[6] = (MEM_SIZE & 0xff00) >> 8;
	MEMORY[5] = (MEM_SIZE & 0xff0000) >> 16;
	MEMORY[4] = (MEM_SIZE & 0xff000000) >> 24;
}

void menu() {
	int command = 0, block_size = 0, block_address = 0;
	while (command != 5) {
		printf("1. Allocate memory\n2. Reallocate memory\n3. Free memory\n4. Memory map\n5. Exit\n");
		scanf_s("%d", &command);
		switch (command) {
			case 1: {
				printf("Block size: ");
				scanf_s("%d", &block_size);
				if (mem_alloc(block_size) == NULL)
					printf("Allocation failed\n");
				break;
			} case 2: {
				printf("Block address (hex): ");
				scanf_s("%x", &block_address);
				printf("New block size: ");
				scanf_s("%d", &block_size);
				if (mem_realloc(MEMORY + block_address, block_size) == NULL)
					printf("Allocation failed\n");
				break;
			} case 3: {
				printf("Block address (hex): ");
				scanf_s("%x", &block_address);
				mem_free(MEMORY + block_address);
				break;
			} case 4: {
				mem_dump();
				break;
			} case 5: break;
		}
		printf("\n");
	}
}

void end() {
	free(MEMORY);
}

main () {
	init();
	menu();
	end();
}