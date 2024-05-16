#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>

#include "art.h"
#include "command.h"
#include "list.h"
#include "utils.h"

/**
 *                                #POIGS#
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * The Iteration on the Iteration of the Iteration on the Iteration of * * *
 * * * * * * * * * * the "Pretty Okay, I Guess" Shell! * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * +
 * Outline:
 * 		This program implements a ruedimuntrie shell for GNU/Linux systems,
 * 		taking cues from Bash, amaunq others.
 * 
 * Basic Functionality:
 * 	(from POIGS)
 * 	 * Run programs in the background using '&'!
 * 	 * Redirect in/out with '<', '>', and '>>'!
 * 
 * Extended Functionality:
 * 	(NEW with #POIGS#)
 * 	 * Run commands from a batch file!
 * 	 * List command history with 'history'!
 * 	 * Scroll through past commands with the (up and down) arrow keys!
 * 	 * Enter autocomplete mode with CTRL-C
 * 
 * Other Improvements:
 * 	 * Streamlined parse_command()
 * 	 * Fixed memory leaks
 * 	 * Reorganized project structure
 * 
 * Known Issues:
 * 	 * Command 'yes abc | head -10' hangs (still, sorry)
 * 	 * Need to press a key after using the SIGINT handler before program resumes
 * 	 * Overdocumented?
 * 
 * FAQs:
 * 	 * Where's POIGS++?
 * 	 * * We scrapped her and started over :)
 * 	 *
 * 	 * What about ++POIGS++?
 * 	 * * We scrapped her and started over :)
 * 	 *
 * 	 * What about ++POIGS#?
 * 	 * * Maui.
 * 	 *
 * 	 * Does feature XYZ work on your machine?
 * 	 * * Yes, every required feature works on my machine (with the caveat that
 * 	 * * SIGINT needs additional input to switch modes). I tested everything as
 * 	 * * extensively as possible, so I absolutely will be heartbroken if she
 * 	 * * don't work .·´¯`(>▂<)´¯`·.
 * 
 * @author fayth quinn
 * 
 * * p.s. this project was a lot of fun! i had a great time with it! (✿◠‿◠) 
*/

int user_mode();
int batch_mode(char* path);
void autoc_swi();				// SIGINT handler

char* user;						// Current user's username
char cwd[1024];					// 'Current Working Directory'

bool run_swi = true;			// Loop control variable
bool autoc = false;				// Autocomplete mode control variable
bool reset_in;					// Loop control variable for user_moder()

int main(int argc, char** argv)
{
	print_art();	// ASCII Art & Intro Message; print_art() in art.h

	if (argc > 1) return batch_mode(argv[1]);	// User path to file
	else return user_mode();					// User did not submit path

	return 0;
}

/**
 * USER MODE
 * 
 * 	This is essentially the main loop from POIGS.
 * 	It allows the user to type commands to be executed by the shell
 * 
 * 	This shell forgoes fgets() or getline(), instead opting to read character
 * 	by character from the input stream. As such, it handles string building,
 * 	backspacing, and entering by itself, as well as handling arrow keys.
 * 
 * 	This can cause issues, as the number of special cases accounted for is
 * 	pretty slim (as Brandon said, this project is mostly thinking about edge
 * 	cases). Any specific issues will be outlined in their given sections.
 * 
 * 
 * HISTORY - AUTOCOMPLETE
 * 
 * 	The autocomplete mode is implemented here. When enabled, it searches
 *  through the history list every time the 'input' string updates and stores
 *  the result of the search in the 'cmd' string. It then writes the 'cmd'
 *  value to the screen if it's longer than the input value. Searching the
 * 	list on every update is kind of costly, but works well for this purpose.
 * 
 * 	When in autocomplete mode, when the user presses enter, the program
 * 	determines whether or not to pass in the command returned from the history
 * 	search or the input and executes that. Commands not in the history can still
 * 	be executed in autocomplete mode.
 * 
 * @returns 0 on Success
*/
int user_mode()
{
	signal(SIGINT, autoc_swi);	// Change SIGINT handler

	// Get the current user's username for display and path purposes
	uid_t uid = getuid();
	struct passwd *pw = getpwuid(uid);
	user = strdup(pw->pw_name);

	char input[128] = "";	// Storing user input; Max command length = 128
	char* cmd = NULL;		// Storing commands from history (autocomplete mode)

	// The custom input loop
	// Runs continously until exit() from command.h is called
	while(run_swi)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL) strcpy(cwd, "");
		full_line_clear();		// Clear line of current output
		print_entry_bar();		// Entry bar using 'user' and 'cwd'
		if (autoc) printf("autoc> ");	// Signal user is in autocomplete mode

		fflush(stdout);

		strcpy(input, "\0");	// Resets 'input' to empty string
		cmd = NULL;				// Point to "NULL"
		reset_in = false;		// Loop control variable

		while(!reset_in)
		{
			char c = getch();	// Get next chararacter; getch() in utils.h

			switch(c)
			{
				// If user presses the 'enter' key...
				case '\n':
					printf("\n");	// Print new line
					// If history command is stored, run that
					if (autoc && cmd != NULL && strcmp(cmd, "") != 0)
					{ parse_command(cmd); }
					else
					{
						// If empty input, return
						if (strcmp(input, "") == 0)
						{
							if (!autoc) print_entry_bar();	// Do nothing
							else autoc_swi();	// Exit autocomplete mode
						}
						else						// Standard input
						{
							// li_add() and li_resetc() in list.h
							li_add(input);			// Add to command history
							parse_command(input);	// Run current input
							li_resetc();			// Reset list cursor
													// (searching; see below)
						}
					}
					reset_in = true;				// Reset input loop	
					break;
				case 8:		// Both possible backspace
				case 127:	// ASCII codes
					backspace(input);	// backspace() in utils.h
					if (autoc) 
					{
						// 'cmd' won't be NULL unless no search has been 
						// performed (no input read)
						if (cmd == NULL) break;
						int old_cmd_len = strlen(cmd);	// Unnecessary

						// li_cmd_cmp() in list.h
						cmd = li_cmd_cmp(input, 1, true);

						// Clear line of previous display
						if (old_cmd_len > strlen(input) + 1) partial_line_clear_i(old_cmd_len - 1);
						else partial_line_clear_i(strlen(input) - 1);

						// Print if...				... no match
						if (strcmp(cmd, "") == 0) printf("%s", input);
						else printf("%s", cmd);	//	...match!
					}
					break;
				case '\033':	// Escape character (begins arrow key sequence)
					getch();	// Discard unneeded character
					// No arrow functionality in autocomplete mode (sorry)
					if (autoc) { getch(); break; }		
					switch(getch())
					{
						// 	partial_line_clear_i() and
						// 	partial_line_clear() in utils.h	
						case 'A':	// Up Arrow

							if (li_mid_walk())	// For formatting
							partial_line_clear_i(strlen(input) - 1);
							else partial_line_clear(input);

							// 1. Move up command history list
							// 2. Store current command in input
							strcpy(input, li_walk(1));	// list_walk() in list.c
							printf("%s", input);		// 3. Print input
							break;

						case 'B':	// Down Arrow

							if (li_mid_walk())	// For formatting
							partial_line_clear_i(strlen(input) - 1);
							else partial_line_clear(input);

							// 1. Move down command history list
							// 2. Store current command in input
							strcpy(input, li_walk(0));	// list_walk() in list.c
							printf("%s", input);		// 3. Print input
							break;

						default: break;	// All other escape chars, do nothing
					} break;
				default:
					strncat(input, &c, 1);	// Add read char to input string
					
					// Print read char (when in stardard mode or no history)
					if (!autoc || li_len() == 0) printf("%c", c);
					else
					{
						// Clear line of previous display
						if (cmd == NULL || (strlen(input) - 1) > strlen(cmd)) partial_line_clear_i(strlen(input) - 1);
						else partial_line_clear(cmd);
						
						cmd = li_cmd_cmp(input, 1, true);	// Search for match

						// Print if  ... match
						if (cmd == NULL || strcmp(cmd, "") == 0) printf("%s", input);	// ... no match
						else printf("%s", cmd);
					}

					// This "char-by-char" method of printing (as opposed to
					// calling getche(), which would echo the pressed key to the
					// terminal immediately) is to avoid pushing out special
					// characters that would normally be invisible to the user,
					// such as when pressing an arrow key.
			}
		}
	}
	return 0;
}

/**
 * Batch Mode
 * 
 * 	This mode attempts to open whatever file is at the provided path and reads
 * 	commands from that file until the end of the file is reached.
 * 
 * @param path The system path to the batch file to be read
 * @returns 0 on Success
*/
int batch_mode(char* path)
{
    FILE* fp;
    char line[1024] = "";
    int index = 0;
    char c;

    fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    while ((c = fgetc(fp)) != EOF)	// Read until the end of file
	{ 
        if (c == '\n')	// New line = execute command
		{
            line[index] = '\0';		// Null-terminate the string
            parse_command(line);	// Parse and run command
            index = 0; 				// Reset index for the next line
        } 
		// Append the character
		else { if (index < (sizeof(line) - 1)) line[index++] = c; }
    }

    if (index > 0)	// If the command is in there...
	{
        line[index] = '\0'; 	// ...ensure the data is null-terminated...
        parse_command(line);	// ...and run!
    }

    fclose(fp);
	return 0;
}

/**
 * AUTOCOMPLETE SWITCH
 * 	Enters/leaves autocomplete mode
*/
void autoc_swi()
{
	autoc = !autoc;	// Switch control variable
	reset_in = true;
	return;
}