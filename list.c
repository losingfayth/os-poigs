#include "list.h"

/**
 * LIST
 * 
 * 	These files outline and create a doubly-linked list for the purpose of
 * 	storing the input history.
 * 
 * 	This is a doubly-linked list, able to be navigated from either end and
 * 	move either direction from the center. Each node is indexed for searching
 * 	and determining list length.
 * 
 * 	This list does not support any operations other than adding to the end of
 *  the list, iterating through the list, and finding a node in the list. That
 * 	is to say, some basic functionality such as removing an element from the
 * 	list, sorting the list, and inserting into the middle of the list is not
 * 	present. These functions are not relevant to the list's purpose as a history
 * 	ledger.
 * 
 * @author fayth quinn
*/

Node* head = NULL;	// The first item in the list
Node* tail = NULL;	// The last item in the list
Node* curr = NULL;	// The current item (cursor; for iteration)

/**
 * LENGTH
 * 
 * @returns The number of items in the list
*/
int li_len() { if (tail == NULL) return 0; else return tail->idx + 1; }

/**
 * RESET CURSOR
 * 	Sets the cursor back to null
*/
void li_resetc() { curr = NULL; }

/**
 * MID WALK ?
 * 
 * @returns Whether the list is currently being iterated through
*/
bool li_mid_walk() { return curr == NULL; }

/**
 * ADD
 * 
 * 	Adds a node to the list and stores the given string there
 * 
 * @param str The string to be stored in the list
*/
void li_add(char* str)
{
	// Do not add "history" command or empty inputs to the list
	if (strcmp(str, "history") == 0 || strcmp(str, "") == 0) return;

	Node* n = malloc(sizeof(Node) + strlen(str) + 1);

	n->cmd = malloc(strlen(str) + 1);
	strcpy(n->cmd, str);			// Store string

	if (head == NULL)	// Empty list
	{
		n->next = NULL;
		n->prev = NULL;
		n->idx = 0;

		head = n;
		tail = n;
	}
	else
	{
		tail->next = n;
		n->prev = tail;
		n->next = NULL;
		n->idx = n->prev->idx + 1;	// Store index number

		tail = n;
	}
}

/**
 * WALK
 * 
 * 	Move a single element through the list in the given direction and give that
 * 	new element's stored input.
 * 
 * 	The return value of this function is intended to be read into the input
 * 	loop's 'input' variable so that it can be easily parsed and executed.
 * 	As such, the return value for a failure of any kind is an empty string (""),
 * 	not disrupting any further user input.
 * 
 * @param dir Whether to walk up the list (1) or down the list (0)
 * @return The string value of the walked-to node
*/
char* li_walk(int dir)
{
	if (tail == NULL || (dir == 0 && curr == NULL)) return "";	// Empty list

	if (curr == NULL) curr = tail;	// Start at the bottom of the list
	else
	{
		/*
		 * This section here mimcs the way command histories tend to work in
		 * other shells. There are two main pieces:
		 * 
		 * 	1. If you reach the beginning of the list, the cursor returns that
		 * 	   first element and doesn't move. Continuing to walk up the list
		 * 	   will do nothing.
		 * 	 Why: If the user continues to press the up arrow, they won't
		 * 	   fall out of the list, preventing overshooting. Also forces the
		 * 	   user to walk out the bottom of the list to exit.
		 * 
		 * 	2. If you are at the bottom of the list and walk down, the cursor
		 * 	   the cursor resets and an empty string is returned.
		 * 	 Why: Allows the user to exit the list and return to an empty
		 * 	   command prompt for custom input.
		 */
		if (dir == 1 && curr->prev != NULL) curr = curr->prev;	// Walk up
		else if (dir == 0)										// Walk down
		{
			curr = curr->next;
			if (curr == NULL) return "";	// Fall out of list
		}
	}
	
	return curr->cmd;
}

/**
 * PRINT
 * 	Iterates through the list from top to bottom and prints each element
*/
void li_print()
{
	if (head == NULL) return;

	curr = head;

	while (curr != NULL) 
	{
		printf("[%i] %s\n", curr->idx, curr->cmd);
		curr = curr->next;
	}
}

/**
 * GET
 * 
 * 	Gets the node at the given index in the list
 * 
 * @param idx The index to locate
 * @return The string value of the node at that index
*/
char* li_get(int idx)
{
	// Empty list or bad index
	if (head == NULL || idx < 0 || idx > tail->idx) return "";

	// This is pretty much completely unnecessary. I don't think the list will
	// ever reach the size where there will be a noticable performance hit from
	// starting on one end or the other, but why not?
	if (idx < tail->idx - idx)	// Start at the top!
	{
		curr = head;
		while (curr->idx < idx) curr = curr->next;
	}
	else						// Start at the bottom!
	{
		curr = tail;
		while (curr->idx > idx) curr = curr->prev;
	}

	return curr->cmd;	// Send out the stored command for that index
}

/**
 * COMMAND COMPARE
 * 
 * 	Checks the given string against each item on the list, stopping once it has
 * 	found a match. Used for autocompletion, where a partial input will be
 * 	searched for and a command that it matches returns.
 * 
 * 	Takes a string (as opposed to a char) so that the user can enter more
 * 	characters and get a result that more accurately reflects their query.
 * 
 * 	Has the option of being reset (having the cursor start at the bottom) or not
 * 	(starting from the last node). This allows for the ability to scroll through
 * 	matching commands using the arrow keys.
 * 
 * 	It will return an empty string if a match is not found.
 * 
 * @param pre	'Prefix', the partial string to be searched for
 * @param dir	Whether to walk up or down the list
 * @param reset Whether or not to reset the cursor before starting the search
*/
char* li_cmd_cmp(char* pre, int dir, bool reset)
{
	int ci;	// 'Cursor Index'
	char* out = malloc(sizeof(char*));
	if (reset) li_resetc();				// Resets cursor
	else ci = curr->idx;				// Stores cursor position before walk

	// Executes once at least in case it's at the head of the list, since the
	// list can't be exited from the top (see li_walk())
	do {
		out = li_walk(dir);	// Get next string
		// Return string if it's a match
		if (strncmp(out, pre, strlen(pre)) == 0) return out;
	} while (curr->idx > 0 && curr->idx <= tail->idx);

	// If the list has not been reset, then the cursor likely initially pointed
	// towards a match. Since no new match has been found, return the old one
	if (!reset) return li_get(ci);
	return "";	// No match found
}

/**
 * FREE
 * 	Safely frees the list and contents of the nodes
*/
void li_free()
{
	if (head == NULL) return; 	// List is empty

	if (head == tail)			// List is one item
	{
		free(head->cmd);
		free(head);
		return;
	}

	curr = head->next;		// Go to second node
	while (curr->next != NULL)	// While not at last node...
	{
		free(curr->prev->cmd);	// 1. Free command string
		free(curr->prev);		// 2. Free node
		curr = curr->next;		// 3. Move to next node
	}
	free(curr->cmd);
	free(curr);			// Free last node
}