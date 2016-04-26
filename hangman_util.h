#ifndef HANGMAN_UTIL_H
#define HANGMAN_UTIL_H

#include <stdbool.h>

/**
 *  Determines whether a string is a valid representation of a decimal integer.
 *  A valid integer follows the regex /[\-\+]?[0-9]+/
 *  
 *  param str = the string to validate
 *  param len = the length of the string to validate
 *  
 *  returns: true if the string is a valid integer representation, false otherwise
 */
bool isInt(const char* str, int len);

/**
 *  Reads a line from a file, ignoring whitespace.
 *  Lines are considered to end on any of the following:
 *      EOF, '\n', '\0'
 *  If the character limit is reached before end-of-line, the rest of the line is eaten.
 *  If the line does not match the criteria specified by the arguments, it is discarded.
 *  
 *  param f = the file to read from
 *  param min_chars = the minimum number of non-whitespace characters the line must have
 *  param max_chars = the maximum number of non-whitespace characters the line should hold
 *                      (excess characters on the line are eaten)
 *  param last_char = pointer filled in with the charcter which terminated the line 
 *                      (always one of the terminators listed above)
 *  param chars_read = pointer filled in with the number of characters read. Has no meaning if the line was not valid.
 *  
 *  returns: pointer to a string of the line read, or NULL if the line was invalid
 */
char* readlnwht(FILE* f, int min_chars, int max_chars, int* last_char, int* chars_read);

/**
 *  Reads a line from standard input.
 *  Characters beyond the buffer maximum but before the end of the line are eaten.
 *  
 *  param stor = pointer to a buffer to store the string in
 *  param max = the maximum number of characters (not including the terminating null) to write to the buffer
 *  
 *  returns: the length of the line stored in the buffer
 */
int getln_s(char* stor, int max);

/**
 *  Frees the elements of an array of char*, and then the array itself.
 *  param arr = pointer to the first element of the array. This function does nothing if arr is NULL.
 *  param n = the number of characters in the array
 */
void arrFree(char** arr, int n);

/**
 *  Gets a random integer between 0 (inclusive) and a maximum (exclusive).
 *  Client code is responsible for seeding the generator with srand()
 *  param max = the maximum of the range
 *  
 *  returns: a random integer in [0, max)
 */
int getRand(int max);

#endif
