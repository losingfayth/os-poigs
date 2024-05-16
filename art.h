#ifndef ART_H
#define ART_H

#include <stdio.h>

/**
 * ART
 * 
 * 	These files hold the functions that utilize the defined colors and would be 
 * 	very cumbersome to include in any of the other sources.
 * 
 * 	Detailed function documentation in art.c
 * 
 * @author fayth quinn
*/

// Color Codes for Formatted Printing
// (colors differ based on terminal)
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"

#define RESET "\x1B[0m"	// Resets colors

extern char* user;		// Declared in main
extern char cwd[];		// Declared in main
extern int bar_size;	// Declared in art.c

void print_art();		// Prints ASCII art
void print_entry_bar();	// Prints formatted entry bar
void print_exit_msg();	// Prints formatted exit message

#endif