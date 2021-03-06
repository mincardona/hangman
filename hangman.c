#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "hangman_util.h"

char** readWords(FILE* f, int min_word_chars, int max_word_chars, int max_words, int* nof_words);
int getArgs(int argc, char** argv);
int guess(char* word, char c);
void clearLetters();
void printWordMasked(const char* word, const bool* mask);
void printLettersGuessed(const bool* mask);
int getLettersLeft(const char* word, const bool* mask);
int getNofCorrect(const bool* mask);
int execCommand(const char* cmd);

#define DEBUG

#ifdef DEBUG
    #define printdbg(...) fprintf(__VA_ARGS__)
#else
    #define printdbg(...) ;
#endif

#define GUESS_CORRECT 0
#define GUESS_WRONG 1
#define GUESS_ALREADY 2

#define LETTERS_IN_ALPHABET 26

const char* USE_STR = "hangman "
                      "-d dictionaryfile "
                      "[-n minimum_letters] "
                      "[-x maximum_letters] "
                      "[-p]\n";
const char* HELP_STR = "! to quit\n"
                       "? for help\n"
                       "\\ to skip word\n"
                       "= to guess entire word\n";

static bool letters[LETTERS_IN_ALPHABET];

static int maxWordChars = 25;
static int minWordChars = 1;
static int maxWords = 20000;

static int nof_words = 0;
static int max_fail = 10;
static int print_on_fail = false;

char* wordFileName = NULL;
char** words = NULL;

static int wi = 0;
static int failed = 0;

int main(int argc, char** argv) {
    int argRet = getArgs(argc, argv);
    if (argRet)
        return argRet;
    
    if (wordFileName == NULL) {
        fprintf(stderr, "%s", USE_STR);
        return 1;
    }
    FILE* wordFile = fopen(wordFileName, "r");
    if (!wordFile) {
        printdbg(stdout, "File could not be read: %s", wordFileName);
        return 1;
    }
    words = readWords(wordFile, minWordChars, maxWordChars, maxWords, &nof_words);
    fclose(wordFile);
    
    if (nof_words < 1) {
        fprintf(stderr, "No words matching the filter criteria were found.\n");
        return 2;
    }
    
    printf("=== %d words read ===\n%s", nof_words, HELP_STR);
    
    clearLetters();
    // file I/O helps to randomize the seed
    srand(time(0));
    
    bool quit = false;
    char prompt[25];
    
    while(!quit) {
        bool fail = false;
        failed = 0;
        clearLetters();
        wi = getRand(nof_words);
        while (getLettersLeft(words[wi], letters) && !quit && !fail) {
            printf("??? ");
            printWordMasked(words[wi], letters);
            printf(" ???\n(%d/%d) ", max_fail - failed, max_fail);
            printLettersGuessed(letters);
            printf("\n> ");
            int prompt_len = getln_s(prompt, 25-1);
            if (!prompt_len) {
                continue;
            } else if (prompt[0] == '!') {
                quit = true;
                break;
            } else if (prompt[0] == '?') {
                printf("\n%s\n", HELP_STR);
            } else if (prompt[0] == '\\') {
                fail = true;
                break;
            } else if (prompt_len == 1 && isalpha(prompt[0])) {
                switch (guess(words[wi], prompt[0])) {
                    case GUESS_CORRECT:
                        puts("Good!");
                        break;
                    case GUESS_WRONG:
                        puts("Incorrect.");
                        if (++failed >= max_fail) {
                            fail = true;
                        }
                        break;
                    case GUESS_ALREADY:
                        puts("Already guessed.");
                        break;
                    default:
                        puts("Undefined switch case");
                }
            } else if (prompt[0] == '=') {
                if (!strcmp(prompt+1, words[wi])) {
                    puts("Jackpot!\n");
                    break;
                } else {
                    fail = true;
                    break;
                }
            } else if (prompt[0] == '/') {
                if (execCommand(prompt+1) == -1) {
                    puts("ERR: Unknown command");
                }
            }
            puts("");
        }
        
        if (fail) {
            printf("Sorry, you missed that one.");
            if (print_on_fail) {
                printf("\nThe word was \"%s\".", words[wi]);
            }
            printf(" Here\'s another:\n\n\n");
        } else if (!quit) {
            printf("The word was \"%s\".\n\n\n", words[wi]);
        }
    }
    
    arrFree(words, nof_words);
    
    return 0;
}

int execCommand(const char* cmd) {
    if (!strcmp(cmd, "debug")) {
        printf("\n~~~ DEBUG ~~~\n");
        printf("max nof words: %d\n"
               "nof words: %d\n"
                
               "min chars: %d\n"
               "max chars: %d\n"
                
               "word file: %s\n"
               "word index: %d\n"
                
               "current word: %s\n"
               "current word chars: %zu\n"
               "nof failed guesses: %d\n",

                maxWords, nof_words,
                minWordChars, maxWordChars,
                wordFileName, wi,
                words[wi], strlen(words[wi]), failed );
        return 0;
    }
    return -1;
}

int guess(char* word, char c) {
    int reg = c - 'a';
    if (letters[reg]) {
        return GUESS_ALREADY;
    }
    int pre = getLettersLeft(word, letters);
    letters[reg] = true;
    if (getLettersLeft(word, letters) - pre) {
        return GUESS_CORRECT;
    }
    return GUESS_WRONG;
}

void clearLetters() {
    for (int i = 0; i < LETTERS_IN_ALPHABET; i++)
        letters[i] = false;
}

// all in word must be a-z
void printWordMasked(const char* word, const bool* mask) {
    for(; *word; word++) {
        if (mask[*word - 'a']) {
            putchar(*word);
        } else {
            putchar('-');
        }
    }
}

void printLettersGuessed(const bool* mask) {
    for (int i = 0; i < LETTERS_IN_ALPHABET; i++) {
        if (mask[i]) {
            putchar('a' + i);
        }
    }
}

int getLettersLeft(const char* word, const bool* mask) {
    int ret = 0;
    for(; *word; word++) {
        if (!mask[*word - 'a']) {
            ret++;
        }
    }
    return ret;
}

int getNofCorrect(const bool* mask) {
    int ret = 0;
    for (int i = 0; i < LETTERS_IN_ALPHABET; i++) {
        if (mask[i]) {
            ret++;
        }
    }
    return ret;
}

char** readWords(FILE* f, int min_word_chars, int max_word_chars, int max_words, int* nof_words) {
    int buf_siz = 500;
    char** buf = malloc(buf_siz * sizeof(char*));
    int c;
    *nof_words = 0;
    char* str;
    int chars_read;
    
    while (*nof_words < max_words) {
        str = readlnwht(f, min_word_chars, max_word_chars, &c, &chars_read);
        if (str) {
            (*nof_words)++;
            if (buf_siz < *nof_words) {
                buf = realloc(buf, (buf_siz *= 2) * sizeof(char*));
            }
            buf[*nof_words-1] = str;
        }
        
        if (c == EOF) {
            break;
        }
    }
    
    return realloc(buf, *nof_words * sizeof(char*));
}

int getArgs(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "%s", USE_STR);
        return 1;
    }
    
    bool argError = false;
    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-n")) {
            i++;
            if (i >= argc) {
                argError = true;
                fprintf(stderr, "ERR: Expected additional arg after -n\n");
                break;
            }
            if (!isInt(argv[i], strlen(argv[i]))) {
                argError = true;
                fprintf(stderr, "ERR: Expected integer after -n\n");
                break;
            }
            minWordChars = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-x")) {
            i++;
            if (i >= argc) {
                argError = true;
                fprintf(stderr, "ERR: Expected additional arg after -x\n");
                break;
            }
            if (!isInt(argv[i], strlen(argv[i]))) {
                argError = true;
                fprintf(stderr, "ERR: Expected integer after -x\n");
                break;
            }
            maxWordChars = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-d")) {
            i++;
            if (i >= argc) {
                argError = true;
                fprintf(stderr, "ERR: Expected additional arg after -d\n");
                break;
            }
            wordFileName = argv[i];
        } else if (!strcmp(argv[i], "-p")) {
            print_on_fail = true;
        } else {
            argError = true;
            fprintf(stderr, "Unrecognized command option: \"%s\"\n", argv[i]);
            break;
        }
    }
    
    if (maxWordChars < 1 || minWordChars < 1) {
        argError = true;
        fprintf(stderr, "ERR: Char count must be at least 1\n");
    }
    
    if (argError) {
        return 1;
    }
    
    return 0;
}
