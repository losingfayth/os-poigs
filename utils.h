#ifndef UTILS_H
#define UTILS_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>	// Key Press Reader

#include "command.h"
#include "list.h"

/**
 * UTILS
 * 
 * 	These files hold small functions primarily used for cleanup and handling
 * 	intricacies.
 * 
 * 	Also implemented here is character reading functionality utilizing the
 *  terminos library. Please see the included link to view the authors of this
 *  code and the code's original source.
 * 
 * 	Detailed function documentation in utils.c
 * 
 * @authors fayth quinn, et al.
*/

extern char* user;			// Declared in main
extern char cwd[];			// Declared in main

// TERMINOS & GETCH
//	[ This source code and its authors can be found here: ]
//	[ https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux ]
void initTermios(int echo);
/* Restore old terminal i/o settings */
void resetTermios(void);
char getch_(int echo);
char getch(void);/* Read 1 character without echo */
char getche(void);	/* Read 1 character with echo */

// CUSTOM FUNCTIONS
void backspace(char* str);	// Provides backspacing functionality for input loop

void full_line_clear();					// Clears the display of a full line
void partial_line_clear_i(int idx);		// Clears the display of idx characters
void partial_line_clear(char* line);	// Clears the display of given string

void free_arr_arr(int** arr, int n);				// Frees 2D arrays
void free_arr_arr_arr(int*** arr, int* ns, int k);	// Frees 3D arrays

#endif