#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <assert.h>

#include "utils.h"
#include "list.h"
#include "art.h"

/**
 * COMMAND
 * 
 * 	These files hold the functions responsible for processing the input and
 * 	and correctly executing it.
 * 	
 * Primary Functions:
 *  * parse_command()
 *  * execute_command()
 * 
 *  Detailed function documentation in command.c
 * 
 * @author fayth quinn
*/

extern char* user;			// Declared in main
extern char cwd[];			// Declared in main
extern bool run_swi;		// Declared in main

// BUILT-IN COMMANDS
int exit_poigs();		// Exit shell
void cd(char* path);	// 'Change Directory'
void history_manl();	// History, manual mode

// COMMAND PROCESSING
int parse_command(char* input);					// Processes input
int execute_command(char** cmdv, 	int cmdc,
					char*** flgv, 	int* flgc,
					int* andv, 		int andc);	// Executes input

// EXECUTION HELPERS
void dir_in(char* path);			// Sets up STDIN redirection
void dir_out(char* path, int mode);	// Sets up STDOUT redirection
void piper(char* cmda, char* cmdb, char** flga, char** flgb);	// Piping
void run(char* cmd, char** flg, bool bckgrnd);	// Wraps execvp()

#endif