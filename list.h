#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"
#include "utils.h"

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
 * 	Detailed function documentation in list.c
 * 
 * @author fayth quinn
*/

typedef struct Node
{
	char* cmd;			// The input (command) being stored
	struct Node* next;	// Next node in list (higher index)
	struct Node* prev;	// Previous node in list (lower index)
	int idx;			// Index location of node in list
} Node;

int li_len();			// Returns the length of the list
void li_resetc();		// Resets the current node (cursor)
bool li_mid_walk();		// Is list mid-iteration?
void li_add(char* str);	// Adds an item
char* li_walk(int dir);	// Gets the next/prev list item
void li_print();		// Prints all list elements
char* li_get(int idx);	// Gets the list item at given index
// Finds matching command in list
char* li_cmd_cmp(char* pre, int dir, bool reset);
void li_free();			// Safely frees list memory

extern Node* head;	// Declared in list.c
extern Node* tail;	// Declared in list.c
extern Node* curr;	// Declared in list.c

#endif