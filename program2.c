// Authors: Parker Lovin and Filoubatir Fadel

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MEMORY_SIZE 256
#define LARGE_MEMORY_SIZE (1 * 1024 * 1024) // 1 MB
#define BAD_BLOCK 'X'

// Simulate the memory space for our allocator
static char memory[MEMORY_SIZE];

// Simulate a large memory space for testing
char *large_memory = NULL;

// Structure to manage memory blocks
typedef struct Block {
    size_t size;
    bool free;
    struct Block *next;
} Block;

Block *freeList = (Block*)memory;

// Initialize the memory manager
void initializeMemory() {
    freeList->size = MEMORY_SIZE - sizeof(Block);
    freeList->free = true;
    freeList->next = NULL;
}

// Randomly mark blocks as "bad" in the large memory
void markBadBlocks(char *memory, size_t size, size_t badBlockCount) {
    srand(time(NULL));
    for (size_t i = 0; i < badBlockCount; i++) {
        size_t randomIndex = rand() % size;
        memory[randomIndex] = BAD_BLOCK; // Mark as bad block
    }
}

// Skeleton function: Allocate memory dynamically, skipping bad blocks
void* myMalloc(size_t size) {
    // STUDENTS: Implement logic to allocate memory dynamically, ensuring that you skip over bad blocks
    int i = 0;
    while (i < MEMORY_SIZE) {
    	if (memory[i] == BAD_BLOCK) {
    		bad_block_counter += 1;
    		continue;
    	}
    	//printf("i = %d.\n", i);
    	//printf("Bad blocks: %d.\n", bad_block_counter);
    	i++;
    }
    return NULL; // Placeholder return value
}

// Skeleton function: Free the allocated memory
void myFree(void *ptr) {
    // STUDENTS: Implement logic to free the allocated memory
    for (int i = 0; i < LARGE_MEMORY_SIZE; i++) {
    	memory[i] = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <size_of_allocation>\n", argv[0]);
        return 1;
    }

    // Convert the command-line argument to an integer for allocation size
    size_t allocationSize = atoi(argv[1]);

    // Initialize memory management
    initializeMemory();

    // Allocate a large memory block (1 MB)
    large_memory = (char*)malloc(LARGE_MEMORY_SIZE);
    if (large_memory == NULL) {
        printf("Failed to allocate large memory.\n");
        return 1;
    }
    printf("Successfully allocated large memory\n"); //DEBUG
    // Mark some blocks as "bad"
    markBadBlocks(large_memory, LARGE_MEMORY_SIZE, 1000); // Mark 1000 bad blocks
    printf("Bad blocks marked\n"); // DEBUG

    // Simulate memory allocation
    int *array = (int*)myMalloc(allocationSize * sizeof(int));  // Allocate memory for an array of integers
    printf("myMalloc function did not crash\n"); //DEBUG
    if (array == NULL) {
        printf("Memory allocation failed.\n");
    } else {
        // Assign values to the array and print them
        for (int i = 0; i < allocationSize; i++) {
            array[i] = i * i;  // Assign square of index
            printf("Array[%d] = %d\n", i, array[i]);
        }

        // Free the allocated memory
        myFree(array);
        printf("Memory successfully freed.\n");
    }

    // DEBUG
    int bad_count = 0;
    for (int i = 0; i < LARGE_MEMORY_SIZE; i++) {
    	if (large_memory[i] == BAD_BLOCK) {
    		bad_count++;
    	}
    }
    printf("Bad blocks: %d\n", bad_count);
	
    // Clean up large memory block using system's free function
    //myFree(large_memory);
    free(large_memory); // DEBUG

    return 0;
}
