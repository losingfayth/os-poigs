#include "utils.h"

/**
 * UTILS
 * 
 *  This is the implementation file for UTILS.
 * 
 * 	These files hold small functions primarily used for cleanup and handling
 * 	intricacies.
 * 
 * 	Also implemented here is character reading functionality utilizing the
 *  terminos library. Please see utils.h to view the authors of this
 *  code and the code's original source.
 * 
 * @author fayth quinn, et al.
*/

static struct termios old, current;

void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */

  if (echo) current.c_lflag |= ECHO; /* set echo mode */
  else current.c_lflag &= ~ECHO; /* set no echo mode */

  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}
/* Restore old terminal i/o settings */
void resetTermios(void) { tcsetattr(0, TCSANOW, &old); }

char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char getch(void) { return getch_(0); }/* Read 1 character without echo */
char getche(void) { return getch_(1); }	/* Read 1 character with echo */

/**
 * BACKSPACE
 * 
 *  Allows for the ability to backspace when using a custom input loop by:
 *    1. Deleting the last character of the input storage string
 *    2. Clearing the output of one character
 * 
 * @param str The input storage string for the custom input loop
*/
void backspace(char* str)
{
  if (strlen(str) != 0) // Only delete from input
  {
    str[strlen(str) - 1] = '\0';  // Move string terminator up one space
    partial_line_clear_i(1);      // Clear output one space
  }
}

/**
 * FULL LINE CLEAR
 *  Removes all characters from the current input/output line
*/
void full_line_clear() { printf("\33[2K\r"); }

/**
 * PARTIAL LINE CLEAR - INDEX
 * 
 *  Removes the given number of characters from the line by moving backwards
 *  that many characters, then clearing everything in front of it.
 * 
 * @param idx The number of characters to clear
*/
void partial_line_clear_i(int idx) { printf("\033[%liD\033[K", (long) idx); }

/**
 * PARTIAL LINE CLEAR
 * 
 *  Removes a number of characters from the line equal to the number of
 *  characters in a given string. Is primarily used for clearing the line when
 *  cylcing through the list.
 * 
 * @param line The line to be cleared
*/
void partial_line_clear(char* line) { partial_line_clear_i(strlen(line)); }

/**
 * FREE ARRAY OF ARRAYS
 * 
 *  Safely frees all nested arrays in a 2D array before freeing the 2D array
 * 
 * @param arr The 2D array (array of arrays) to free
 * @param n   The number of nested arrays
*/
void free_arr_arr(int** arr, int n)
{
	for (int i = 0; i < n; i++) free(arr[i]);
	free(arr);
}

/**
 * FREE ARRAY OF ARRAYS OF ARRAYS
 * 
 *  Safely frees all 2D arrays nested in a 3D array before freeing the 3D array
 * 
 * @param arr The 3D array (array of arrays of arrays) to free
 * @param ns  Array of Ns such that n[x] = the number of nested arrays in arr[x]
 * @param k   The number of nested 2D arrays in arr
*/
void free_arr_arr_arr(int*** arr, int* ns, int k)
{ for (int i = 0; i < k; i++)  free_arr_arr(arr[i], ns[i]); }