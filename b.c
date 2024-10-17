#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_next(const char *t, int *next) {
    int i = 1, j = 0;
    int len = strlen(t);
    next[0] = 0;
    while (i < len) {
        if (j == 0 || t[i - 1] == t[j - 1]) {
            i++;
            j++;
            next[i - 1] = j;
        } else {
            j = next[j - 1];
        }
    }
    printf("Next array: ");
    for (i = 0; i < len; i++) {
        printf("%d ", next[i]);
    }
    printf("\n");
}

int* find_all_occurrences(const char *s, const char *t, int *count) {
    if (!s || !t) {
        *count = 0;
        return NULL;
    }

    int n = strlen(s);
    int m = strlen(t);
    int *occurrences = NULL;
    *count = 0;

    if (m == 0) return NULL;

    int *next = (int *)malloc(sizeof(int) * m);
    if (!next) return NULL;
    compute_next(t, next);

    int i = 0, j = 0;
    while (i < n) {
        if (j == 0 || s[i] == t[j]) {
            i++;
            j++;
        } else {
            j = next[j - 1];
        }
        if (j == m) {
            (*count)++;
            occurrences = (int *)realloc(occurrences, sizeof(int) * (*count));
            if (!occurrences) {
                free(next);
                return NULL;
            }
            occurrences[*count - 1] = i - m;
            j = next[j - 1];
        }
        printf("i: %d, j: %d\n", i, j);  // Debug print
    }

    free(next);
    return occurrences;
}

int count_occurrences(const char *s, const char *t, int *count) {
    if (!s || !t) {
        *count = 0;
        return 0;
    }
    int *occurrences = find_all_occurrences(s, t, count);
    if (occurrences) {
        free(occurrences);
    }
    return *count;
}

int main() {
    char s[] = "goodgooggoodgoogleasgon";
    char t[] = "goodgoogle";

    printf("Searching for '%s' in '%s'\n", t, s);

    int count = 0;
    int *occurrences = find_all_occurrences(s, t, &count);

    if (occurrences) {
        printf("Occurrences of '%s' in '%s':\n", t, s);
        for (int i = 0; i < count; i++) {
            printf("Index: %d\n", occurrences[i]);
        }
        free(occurrences);
    } else {
        printf("No occurrences found or input error.\n");
    }

    int num_occurrences = 0;
    count_occurrences(s, t, &num_occurrences);
    printf("Number of occurrences: %d\n", num_occurrences);

    return 0;
}