#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Calculate the next array for KMP
void calculate_next(const char *t, int *next) {
    int i = 0, j = 0;
    int len = strlen(t);
    next[0] = 0;
    while (i < len ) {
        if (j == -1 || t[i] == t[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// Find all occurrences of t in s
int* find_all_occurrences(const char *s, const char *t, int *count) {
    if (!s || !t) {
        *count = 0;
        return NULL;
    }

    int s_len = strlen(s);
    int t_len = strlen(t);
    int *occurrences = NULL;
    *count = 0;

    if (t_len == 0) {
        return NULL; 
    }


    int *next = (int *)malloc(sizeof(int) * (t_len + 1));
    if (!next) {
        *count = 0;
        return NULL;
    }
    calculate_next(t, next);

    int i = 0, j = 0;
    while (i < s_len) {
        if (j == -1 || s[i] == t[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
        if (j == t_len) {
            (*count)++;
            occurrences = (int *)realloc(occurrences, sizeof(int) * (*count));
            if (!occurrences) {
                free(next);
                *count = 0;
                return NULL;
            }
            occurrences[(*count) - 1] = i - t_len;
            j = next[j]; // Continue searching for overlapping occurrences
        }
    }

    free(next);
    return occurrences;
}

// Count occurrences of t in s
int count_occurrences(const char *s, const char *t) {
    int count = 0;
    find_all_occurrences(s, t, &count); // Reuse the finding function
    return count;
}



int main() {
    char s[] = "goodgooggoodgoogleasgon";
    char t[] = "goodgoogle";
 //   char t[] = "abcabcd";
    int count = 0;

    int *occurrences = find_all_occurrences(s, t, &count);

    if (occurrences) {
        printf("Occurrences of '%s' in '%s':\n", t, s);
        for (int i = 0; i < count; i++) {
            printf("Index: %d\n", occurrences[i]);
        }
        free(occurrences);
    } else if(t && s && strlen(t) > 0){
        printf("No occurrences found or input is NULL.\n");
    }

   


    count = count_occurrences(s, t);
    printf("Number of occurrences: %d\n", count);

    return 0;
}
