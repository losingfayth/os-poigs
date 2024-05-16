# #POIGS#
## The Iteration on the Iteration of the Iteration on the Iteration of "Pretty Okay, I Guess" Shell!
**author:** fayth quinn ([*@losingfayth*](https://github.com/losingfayth))

*30th April 2024*

This program implements a rudimentary shell for GNU/Linux systems, taking cues from Bash, among others.

The purpose of this program is to explore the ways which an operating system and its programs are constructed and handled. This program is written in C as that is the favorite language of many an OS programmer (and for good reason!)

Features
 * Run programs in the background with '&'
 * Redirect program in/out with '<', '>', '>>'
 * Link output of one program with input of another with '|'
 * Run commands from a batch file or from the input terminal
 * View command history, scroll through command history, and autocomplete commands from command history

## Context
The program was created as part of a school project for a class on Operating Systems. The first part of this project, POIGS, had no history or batch file support. The second part of this project, #POIGS# was built off of the first part (only the second part available in this repository).

## Project Time
POIGS: ~2 Days  
#POIGS#: ~2 Days  
Total: ~4 Days

## Known Issues
 * Some pipe commands, such as 'yes abc | head -10' hang, forcing external termination
 * User must make additional input before they can get to the input prompt when switching between standard user mode and history autocomplete mode
 * Memory issues, including memory leaks and invalid reads/writes (although none causing any acute issues at this time)
 * Does not send error messages some of (most of) the time for invalid paths or commands
 * Occassionally will not quit on 'exit' command, needing the command to run again

## Compilation & Running Considerations
 * The signal handler for SIGINT (CTRL-C) is modified and does not terminate the program. The program can be exited by exiting the greater terminal it runs in (Bash, etc.), using the 'exit' command (case sensitive), or via segfault.
 * The 'unique' naming scheme of the main source file makes it slightly annoying to compile. A Makefile is provided for the purpose of easy compilation (or the source file can be renamed)
 * This program allocates a fair bit of memory and, while much of it is freed correctly, caution should be taken.
 * This program utilizes a custom input loop which does not account for many edgecases.

## Feedback Recieved
On POIGS
* dc: When the user types an invalid command, there is no error message.
* dc: When attempting to change into a non-existent directory, there is no error message.
* dc: I'm not sure if this is a problem with pipes, but the following command behaves strangely:
	'yes abc | head -10'
	Other test cases involving pipes executed correctly.
	* fq: Seems to be an issue of the 'waitpid' call in the parent of the pipe not behaving as expected, likely due to the infinite nature of the 'yes' command. Not tested further due to short turn around time.
* dc: Typos are bound to occur, but doesn't your editor have a spelling checker?
	"This program writes a rudamentary shell for GNU/Linux systems, taking cues from Bash, amoung others."
	* fq: >:(
* dc: As for the code itself: to be honest, I did not bother looking at it because I've already seen that it behaves correctly and the meticulous  (as usual) documentation convinces me that there are no significant issues or problems waiting to be discovered. There are other strong students who write clear and helpful comments, but few whose documentation inspires such immediate confidence in the code.
	* fq:	:)

On #POIGS#
* dc: Basic functionality  
	  &nbsp;&nbsp;Does not work: echo "a b c" | wc (but it is an issue with quotes, not pipes, because echo abc | wc works)  
	  &nbsp;&nbsp;Minor detail: > and >> were implemented for append and overwrite respectively due to a typo on one line.
	* fq: Quote handling improved from submitted version but still shakey. Redirection fixed.
* dc: Batch mode  
	  &nbsp;&nbsp;I tested it with this file:  
	  	&nbsp;&nbsp;&nbsp;&nbsp;echo testing batch mode  
		&nbsp;&nbsp;&nbsp;&nbsp;date  
		&nbsp;&nbsp;&nbsp;&nbsp;hostname  
		&nbsp;&nbsp;&nbsp;&nbsp;ls /bin/log* -l | sort -r  
		&nbsp;&nbsp;&nbsp;&nbsp;echo testing complete  
	  &nbsp;&nbsp;Everything worked except for the penultimate command. The reason why it did not work previously is that my batch file had been created in windows, which terminates lines with /r/n (vs. just /n in Linux). Not your fault. As for the penultimate command, it gives this error:  
	  	&nbsp;&nbsp;&nbsp;&nbsp;ls: cannot access '/bin/log*': No such fule or directory  
	  &nbsp;&nbsp;When I try to run just ls /bin/log* in interactive mode, it gives the same error message. At this point, looking at the code and seeing that batch mode mostly works, let's move on and just say it works. The same command produced the same error in batch mode for another student with a high-quality solution, although when I ran the command in his shell in interactive mode, there was no problem. Mysteries.
	* fq: The shell does not currently function with the wildcare character, '*'
* dc: Command history  
	  &nbsp;&nbsp;You noted that the fact that history never appears in the history is by design. That's fine (although Bash does record history as part of the history).
	* fq: Creative license
* dc: Auto-Completion  
	  &nbsp;&nbsp;Good.  
	  fq: :)