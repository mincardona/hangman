#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool isInt(const char* str, int len) {
    bool hadDigit = false;
    for (int i = 0; i < len; i++) {
        if (str[i] == '-' || str[i] == '+') {
            if (i != 0) {
                return false;
            }
        }
        if (!isdigit(str[i])) {
            return false;
        }
        hadDigit = true;
    }
    return hadDigit;
}

char* readlnwht(FILE* f, int min_chars, int max_chars, int* last_char, int* chars_read) {
    // read a character
    // if the character is whitespace, skip it
    // if the character is EOF, \n, or \0, then end the loop
    // if the character count has been reached, skip it
    // increment the character counter
    // if the buffer size is less than or equal to the char counter, double it
    // write the character to [char counter - 1]
    *chars_read = 0;
    int buf_siz = 10;
    char* str = malloc(buf_siz);
    int c;
    bool fr = false;
    
    for (;;) {
        c = getc(f);
        if (c == EOF || c == '\n' || c == '\0') {
            break;
        } else if (*chars_read == max_chars) {
            fr = true;
            continue;
        } else if (isspace(c)) {
            continue;
        } else if (!isalpha(c)) {
            break;
        }
        (*chars_read)++;
        if (buf_siz <= *chars_read)
            str = realloc(str, buf_siz *= 2);
        str[*chars_read - 1] = c;
    }
    *last_char = c;
    if (fr || *chars_read < min_chars) {
        free(str);
        return NULL;
    }
    str[*chars_read] = '\0';
    return realloc(str, *chars_read + 1);
}

int getln_s(char* stor, int max) {
    int c;      // input character
    int i = 0;  // counts the line length
    while ((c = getchar()) != EOF && c != '\n' && c != '\0') {
        if (i < max && !isspace(c))
            stor[i++] = c;
    }
    stor[i] = '\0';
    return i;
}

void arrFree(char** arr, int n) {
    // do nothing if array is NULL (already freed)
    if (!arr) {
        return;
    }
    for (n--; n >= 0; n--) {
        // free the array pointer
        free(arr[n]);
    }
    free(arr);
}

int getRand(int max) {
    return rand() % max;
}
