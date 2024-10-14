#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <x86intrin.h>

#define CACHE_SIZE 1000
#define NUM_THREADS 4

static int armstrong_cache[CACHE_SIZE] = {0};

inline int cube(int x) {
    int result;
    __asm__ (
        "imull %1, %0\n\t"
        "imull %1, %0"
        : "=r" (result)
        : "r" (x), "0" (x)
    );
    return result;
}

int is_armstrong(int n) {
    if (n < CACHE_SIZE && armstrong_cache[n] != 0) {
        return armstrong_cache[n] == 1;
    }

    int sum = 0;
    int temp = n;
    while (temp) {
        int digit = temp % 10;
        sum += cube(digit);
        temp /= 10;
    }

    int result = (sum == n);
    if (n < CACHE_SIZE) {
        armstrong_cache[n] = result ? 1 : 2;
    }
    return result;
}

struct thread_data {
    int start;
    int end;
    int* count;
};

void* check_range(void* arg) {
    struct thread_data* data = (struct thread_data*)arg;
    for (int i = data->start; i <= data->end; i++) {
        if (is_armstrong(i)) {
            printf("Armstrong number found: %d\n", i);
            (*data->count)++;
        }
    }
    return NULL;
}

void print_progress(float progress) {
    int bar_width = 50;
    int pos = bar_width * progress;
    printf("[");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %.2f%%\r", progress * 100.0);
    fflush(stdout);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    if (start < 0 || end < 0 || start > end) {
        fprintf(stderr, "Invalid range. Please provide positive integers with start <= end.\n");
        return 1;
    }

    int range = end - start + 1;
    int chunk = range / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_args[NUM_THREADS];
    int total_count = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].start = start + i * chunk;
        thread_args[i].end = (i == NUM_THREADS - 1) ? end : start + (i + 1) * chunk - 1;
        thread_args[i].count = &total_count;
        if (pthread_create(&threads[i], NULL, check_range, &thread_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        print_progress((float)(i + 1) / NUM_THREADS);
    }

    printf("\nTotal Armstrong numbers found: %d\n", total_count);
    return 0;
}