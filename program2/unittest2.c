// Authors: Parker Lovin and Filoubatir Fadel
// Course: CSC-2770-001
// Date: 9/26/2024

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
    if (size > MEMORY_SIZE) {
    	printf("DEBUG: The size argument was greater than the available space!\n");
    	return NULL;
    }
    // Ensure that bad blocks in the large memory are reflected in the normal memory.
    //*(large_memory + 11) = BAD_BLOCK; // DEBUG
    for (int x = 0; x < MEMORY_SIZE; x++) {
    	if (*(large_memory + x) == BAD_BLOCK) {
    		*(memory + x) = BAD_BLOCK;
    		//printf("DEBUG: BAD BLOCK FOUND at %d\n", x);
    	}
    }
    
    int i = 0; // Keeps track of the start of a hypothetical allocated section.
    int j = 1; // Scans a hypothetical allocated section for bad blocks.
    
    //*(memory = BAD_BLOCK; // DEBUG to test handling of bad blocks
    //*(memory + 21) = BAD_BLOCK; // DEBUG
    //printf("Value at index 10: %c\n", *(memory + 10)); // DEBUG to test memory array
    
    while (i < MEMORY_SIZE) { // May need to adjust this condition to avoid being out of bounds or to loop around.
    	//bool canUseBlock = false;
    	if (*(memory + i) != BAD_BLOCK) {
    	
    		// Let i be the starting point of the hypothetical allocated memory. j starts at the second potential index.
    		j = i + 1;
    		// Since i keeps track of the start of the allocated memory, j scans the other potential indices for bad blocks.
    		while (j < i + size) {
    			//printf("j = %d\n", j); // DEBUG
    			// If a bad block is found by j, move i immediately after j. This effectively disregards all possible
    			// sections of memory that could include this particular bad block.
    			if (*(memory + j) == BAD_BLOCK) {
    				i = j + 1;
    				//printf("DEBUG: Bad block found at index %d\n", j);
    				//canUseBlock = false;
    			}
    			j++;
    		}
    		printf("DEBUG: Valid memory found from indices %d to %d\n", i, i + size - 1);
    		return memory + i;
    	}
    	//printf("i = %d.\n", i);
    	//printf("Bad blocks: %d.\n", bad_block_counter);
    	i++;
    }
    return NULL; // Placeholder return value
}

// Skeleton function: Free the allocated memory
void myFree(void *ptr, int size) {
	char *array = (char*)ptr;
    // STUDENTS: Implement logic to free the allocated memory
    for (int i = 0; i < size; i++) {
    	*(array + i) = '\0'; // To clear the array, replace each element with the default character.
    }
}

// Unit Test Functions
void testMemoryAllocation() {
    // Test 1: Allocate memory for an array of size 10
    int *array = (int*)myMalloc(10 * sizeof(int));
    if (array != NULL) {
        printf("Test 1 Passed: Memory allocated for an array of size 10\n");
        myFree(array, 10 * sizeof(int)); // Note: for simplicity, I added a second parameter to myFree.
    } else {
        printf("Test 1 Failed: Memory allocation for array of size 10 failed.\n");
    }

    // Test 2: Try to allocate more memory than available
    int *largeArray = (int*)myMalloc(MEMORY_SIZE + 1);
    if (largeArray == NULL) {
        printf("Test 2 Passed: Large memory block allocation failed as expected.\n");
    } else {
        printf("Test 2 Failed: Large memory block should not have been allocated.\n");
        myFree(largeArray, LARGE_MEMORY_SIZE + 1); // Note: for simplicity, I added a second parameter to myFree.
    }

    // Test 3: Allocate memory while skipping bad blocks
    int *testBlock = (int*)myMalloc(50 * sizeof(int));
    if (testBlock != NULL) {
        printf("Test 3 Passed: Memory allocated by skipping bad blocks.\n");
        myFree(testBlock, 50 * sizeof(int)); // Note: for simplicity, I added a second parameter to myFree.
    } else {
        printf("Test 3 Failed: Memory allocation should have succeeded by skipping bad blocks.\n");
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
    
    // Mark some blocks as "bad"
    markBadBlocks(large_memory, LARGE_MEMORY_SIZE, 1000); // Mark 1000 bad blocks
    
    // Run tests
    testMemoryAllocation();
    
    // Free memory
    free(large_memory);

    return 0;
}
