#include "command.h"

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
 * @author fayth quinn
*/

/**
 * EXIT ++POIGS++
 * 	Quits the shell
*/
int exit_poigs()
{
	print_exit_msg(); // in art.h
	run_swi = false;
	
	return 1;
}

/**
 * CHANGE DIRECTORY
 * 
 * 	Changes the user's current working directory to the given directory
 * 
 * @param path Path to the new directory
*/
void cd(char* path)
{
	// If a use attempts to access their home directory...
	if (strcmp(path, "~") == 0)
	{
		// ...manually construct the path and change them to it!
		int path_len = strlen("/home/") + strlen(user) + 1;
		char *new_path = malloc(path_len);

		strcpy(new_path, "/home/");
		strcat(new_path, user);

		chdir(new_path);
		free(new_path);
	}
	else chdir(path);	// Or they could be chill about it, but w/e ig....
}

/**
 * HISTORY - MANUAL
 * 
 * 	Prints the items from the history list out to the index, then waits for the
 * 	user to make a selection
 * 
*/
void history_manl()
{	
	li_print();			// Print the history; li_print() in list.h
	print_entry_bar();	// Print the input entry bar; print_entry_bar() in art.h
	char c = getch();	// Get keyboard input
	int d = c - '0';	// Convert char to int

	// If the user presses an invalid key, return; li_len() in list.h
	if (li_len() == 0 || d < 0 || d > (li_len() - 1)) return;
	else	// Otherwise, run the given command
	{
		printf("\n");
		parse_command(li_get(d));	// Run command from history
	}
}

/**
 * PARSE COMMAND
 * 
 * 	Takes a given string, parses it, and sorts the pieces into:
 * 	 1. Commands to be run
 * 	 2. Flags for those commands
 * 	 3. Modifiers to those commands ("anders")
 * 
 * 	It then passes those arrays to EXECUTE COMMAND to be run
 * 
 * 	CHANGES:
 * 	 * Combined term separation and term sorting into one loop
 * 	 * Anders are now stored as integers instead of strings
 * 
 * @param input The user or batch file input to be processed
 * @returns 0 on Success
*/
int parse_command(char* input)
{
	char** 	cmdv = malloc(1000 * sizeof(char));		// "Commands Vector"
	char*** flgv = malloc(10000 * sizeof(char));	// "Flags Vector"
	int* 	andv = malloc(100 * sizeof(int));		// "Anders Vector"
	int 	cmdc = 0;								// "Commands Count"
	int* 	flgc = malloc(100 * sizeof(int));		// "Flags Count"
	int 	andc = 0;								// "Anders Count"

	bool is_flag = false;	// Loop Control Variable
	for (int i = 0; i < strlen(input); )
	{
		char* word = malloc(strlen(input) + 1);
		word[0] = input[i];
		int j = 1;
		while (input[++i] != ' ' && input[i] != '\0' && input[i] != '\n')
		{
			if (input[i] == '\'')
			{
				while (i < strlen(input) && input[i] != '\'')
					word[j++] = input[i++];
				word[j++] = '\'';
			}
			else if (input[i] == '"')
			{
				while (i < strlen(input) && input[i] != '"')
					word[j++] = input[i++];
				word[j++] = '"';
			}
			else word[j++] = input[i];
		}
		word[j] = '\0';
		++i;

		// CHANGE: Moved this sorting into this primary loop
		// Check if anders (next term following an ander is a command)
		if (strcmp(word, ">>") == 0) { andv[andc++] = 0; is_flag = false; }
		else if (strcmp(word, ">") == 0) { andv[andc++] = 1; is_flag = false; }
		else if (strcmp(word, "<") == 0) { andv[andc++] = 2; is_flag = false; }
		else if (strcmp(word, "&") == 0) { andv[andc++] = 3; is_flag = false; }
		else if (strcmp(word, "|") == 0) { andv[andc++] = 4; is_flag = false; }
		else if (is_flag)
		{
			flgv[cmdc - 1][flgc[cmdc - 1]] = malloc(strlen(word) + 1);
			strcpy(flgv[cmdc - 1][flgc[cmdc - 1]++], word);
		}
		else	// ...or if command
		{
			// Terminate flag array of previous command
			if (cmdc > 0) flgv[cmdc - 1][flgc[cmdc - 1]] = NULL;

			// Allocate memory for new array entries
			cmdv[cmdc] 		= malloc(strlen(word) + 1);		// Next Command
			flgv[cmdc] 		= malloc(100 * sizeof(char));	// Command Flags
			flgv[cmdc][0] 	= malloc(strlen(word) + 1);		// Flag Storage

			strcpy(cmdv[cmdc], word);		// Store command
			strcpy(flgv[cmdc][0], word);	// Store command as flag
											// (needed for execvp())

			flgc[cmdc++] = 1;				// Update flag count, command count

			is_flag = true;	// Next term cannot be another command
		}
		free(word);	// Reset word
	}
	flgv[cmdc - 1][flgc[cmdc - 1]] = NULL;	// Terminate last flag array

	return execute_command(cmdv, cmdc, flgv, flgc, andv, andc);
}

/**
 * EXECUTE COMMAND
 * 
 * 	Prepare the necessary setup for piping, redirecting, running in the
 * 	background, etc.
 * 
 * 	CHANGES:
 * 	 * Arrays are freed correctly after use (memory leak plugged)
 * 
 * 	OBSERVATIONS:
 * 	 * CMDV is unnecessary. Since the command is always stored in FLGV[CMDC][0],
 * 	   that can be used in place of CMDV[CMDC]. It's a holdover from a different
 * 	   build, but will be sticking around so as to not unintentionally break
 * 	   anything at the eleventh hour (Doctor Who reference).
 * 	 * My IDE will highlight anything I put in the doc comments that I make
 * 	   uppercase and place a colon after, but somehow doesn't have an automatic
 * 	   spellchecker.
 * 
 * @param cmdv An array of commands ("command vector")
 * @param cmdc The number of commands in cmdv ("command count")
 * @param flgv An array of flag arrays for cmdv ("flag vector")
 * @param flgc An array of the number of flags in each flgv array ("flag count")
 * @param andv An array of 'anders' for execution modification ("ander vector")
 * @param andc The number of anders in andv ("ander count")
 * @returns 0 on Success
*/
int execute_command(char** cmdv, int cmdc, char*** flgv, int* flgc, int* andv, int andc)
{
	// Built In: EXIT - Exit Shell
	if (strcmp(cmdv[0], "exit") == 0) return exit_poigs();
	// Built In: CHANGE DIRECTORY - Change Current Working Directory
	else if (strcmp(cmdv[0], "cd") == 0) cd(flgv[0][1]);
	// Built In: HISTORY - MANUAL - Select Command from Command History
	else if (strcmp(cmdv[0], "history") == 0) history_manl();
	else if (andc == 0) run(cmdv[0], flgv[0], false);	// Run without modifying
	else												// Run with modifiers
	{
		int stdo_fd = dup(STDOUT_FILENO);	// Store original STDOUT
		int stdi_fd = dup(STDIN_FILENO);	// Store original STDIN
		bool bckgrnd = false;				// Run command in background?

		for (int i = andc - 1; i >= 0; i--)	// Begin at end of input
		{
			// >>, > Redirect ouput
			if (andv[i] == 0 || andv[i] == 1)
			{
				dir_out(cmdv[i + 1], andv[i]);
				if (i > 0) --i;	// If there is another modifier, decrement i...
			}
			
			// < Redirect input
			if (andv[i] == 2)
			{
				dir_in(cmdv[i + 1]);
				if (i > 0) --i;	// ...so that that modifier can be caught...
			}

			// & Run command in background
			if (andv[i] == 3)
			{
				bckgrnd = true;
				if (i > 0) --i;	// ...and set up before execution
			}

			// | Pipe command output into next command input
			if (andv[i] == 4) piper(cmdv[i], cmdv[i + 1], flgv[i], flgv[i+1]);
			else run(cmdv[i], flgv[i], bckgrnd);	// Don't pipe
		}

		dup2(stdo_fd, STDOUT_FILENO);	// Reset STDOUT
		dup2(stdi_fd, STDIN_FILENO);	// Reset STDIN

		close(stdo_fd); close(stdi_fd);
	}

	// free_arr_arr_arr() in utils.h
	free_arr_arr_arr((int***) flgv, flgc, cmdc);	// Free flgv array safely

	// Since flgv[n][0] and cmdv[n] both point to the same thing, the elements
	// of cmdv are already freed, so only the outer array need be freed
	free(cmdv);										// Free cmdv array
	free(flgc);										// Free flgc array
	free(andv);										// Free andv array

	return 0;
}

/**
 * DIRECT INPUT
 * 
 * 	Modifies command to take input from given file instead of STDIN
 * 
 * @param path The filepath to direct input from
*/
void dir_in(char* path)
{
	int in = open(path, O_RDONLY);
	dup2(in, STDIN_FILENO);
	close(in);
}

/**
 * DIRECT OUTPUT
 * 
 * 	Modifies command to output to given file instead of STDOUT
 * 
 * @param path The filepath to direct output towards
 * @param mode Whether to truncate file (0) or append to it (1)
*/
void dir_out(char* path, int mode)
{
	int out = open(path, O_WRONLY | O_CREAT | (mode == 1 ? O_TRUNC : O_APPEND), 0644);
	dup2(out, STDOUT_FILENO);
	close(out);
}

/**
 * PIPER
 * 
 * 	Redirects the output stream of one command into the input stream of another
 * 	then runs the commands
 * 
 * @param cmda "Command A", the command to be executed by the child
 * @param cmdb "Command B", the command to be executed by the parent
 * @param flga "Flags A", the list of flags for Command A
 * @param flgb "Flags B", the list of flags for Command B
*/
void piper(char* cmda, char* cmdb, char** flga, char** flgb)
{
	int pipes[2];
	pid_t pid;

	pipe(pipes);
	pid = fork();

	if (pid == 0)	// Child
	{
		close(pipes[0]);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[1]);
		run(cmda, flga, false);		// Run giving command
	}
	else			// Parent
	{
		close(pipes[1]);
		dup2(pipes[0], STDIN_FILENO);
		close(pipes[0]);
		run(cmdb, flgb, false);		// Run receiving command
		waitpid(pid, NULL, 0);
	}
}

/**
 * RUN
 * 
 * 	Wraps execvp() in fork so that it doesn't kill the shell on command
 *  execution
 * 
 * @param cmd		The command to be executed
 * @param flg		The flags for that command
 * @param bckgrnd	Whether the command should be executed in the background
*/
void run(char* cmd, char** flg, bool bckgrnd)
{
	pid_t pid;
	pid = fork();

	if (pid == 0) { execvp(cmd, flg); exit(1); }	// Child
	else { if (!bckgrnd) waitpid(pid, NULL, 0); }	// Parent
}