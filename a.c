#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

#define MAX_CACHE_SIZE 1000
#define MAX_THREADS 4 // Adjust based on your system

// Cache for Armstrong numbers up to 1000
static uint8_t armstrong_cache[MAX_CACHE_SIZE + 1] = {0};

// Structure to pass data to threads
typedef struct {
    int start;
    int end;
} ThreadData;


// Function to check if a number is an Armstrong number (optimized with inline assembly and pointer arithmetic)
static inline int is_armstrong(int n) {
    if (n <= MAX_CACHE_SIZE && armstrong_cache[n] != 0) {
        return armstrong_cache[n] == 1;  // Use cached value
    }

    int sum = 0;
    int tempNumber = n;
    int rem;

    while (tempNumber != 0) {
        rem = tempNumber % 10;

        // Inline assembly for cube calculation (x86)
        int cube;
        __asm__ (
            "mov %1, %%eax;"  // Move rem into eax
            "imul %%eax;"    // Multiply eax by eax (rem * rem)
            "imul %1;"       // Multiply by rem again (rem * rem * rem)
            "mov %%eax, %0;"  // Store result in cube
            : "=r" (cube)    // Output operand
            : "r" (rem)      // Input operand
            : "%eax"          // Clobbered register
        );

        sum += cube;
        tempNumber /= 10;
    }

    if (n <= MAX_CACHE_SIZE) {
        armstrong_cache[n] = (sum == n) ? 1 : 2; // Cache the result (1=true, 2=false)
    }
    return sum == n;
}

// Thread function to find Armstrong numbers within a range
void* find_armstrong(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int n = data->start; n <= data->end; n++) {
        if (is_armstrong(n)) {
            printf("Armstrong number: %d\n", n);
        }
    }
    return NULL; 
}



int main(int argc, char *argv[]) {
    int start, end;

    // Initialize Armstrong cache
    armstrong_cache[0] = 2; // 0 is not an Armstrong number
    armstrong_cache[1] = 1; // 1 is an Armstrong number


    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
        return 1;
    }

    // Error handling for command-line arguments
    if (sscanf(argv[1], "%d", &start) != 1 || sscanf(argv[2], "%d", &end) != 1) {
       fprintf(stderr, "Invalid input.  Please enter integers.\n");
       return 1; 
    }


    // Ensure start <= end
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }


    // Multi-threading implementation
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int range_per_thread = (end - start + 1) / MAX_THREADS;

     for (int i = 0; i < MAX_THREADS; i++) {
        thread_data[i].start = start + i * range_per_thread;
        thread_data[i].end = (i == MAX_THREADS - 1) ? end : start + (i + 1) * range_per_thread - 1;
        pthread_create(&threads[i], NULL, find_armstrong, &thread_data[i]);
    }


    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

