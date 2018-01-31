#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

long string_to_pos_long(char *s);
void hailstone_sequence(long start);

int main(int argc, char *argv[]) {
  // Check for a single command line arg
  if (argc != 2) {
    fprintf(stderr, "Invalid input. Please enter the starting number for the Collatz conjecture.\n");
    exit(1);
  }

  // Verify the input is a positive integer
  long start = string_to_pos_long(argv[1]);
  if (start < 1) {
    fprintf(stderr, "Invalid input. Please enter a positive integer as the starting point.\n");
    exit(1);
  }

  // Fork a new process
  int rc;
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // Child process computes the hailstone sequence from 'start'
    hailstone_sequence(start);
  } else {
    // Wait for the child to finish
    int wc = wait(NULL);
    if (wc < 0) {
      fprintf(stderr, "wait failed\n");
      exit(1);
    }
  }
  return 0;
}

/* Prints to stdout the hailstone sequence starting at start
 *
 * @param - the starting number for the sequence
 */ 
void hailstone_sequence(long start) {
  long cur = start;
  while (1) {
    // Base case
    if (cur == 1) {
      printf("%ld\n", cur);
      break;
    }
    
    if (cur % 2 == 0) {
      printf("%ld, ", cur);
      cur = cur / 2;
    } else {
      printf("%ld, ", cur);
      cur = cur * 3 + 1;
    }
  }
  return;
}
 
/* Converts a string to a positive integer.
 *
 * @param - the input to be converted.
 *
 * @return - the positive integer or -1 if the string isn't a positive integer.
 */
long string_to_pos_long(char *s) {
  // Verify each character is a digit
  for (size_t i = 0; i < strlen(s); i++) {
    if (!isdigit(s[i]))
      return -1;
  }
  // Convert string to a long and return
  char *ptr;
  long ret = strtol(s, &ptr, 10);
  return ret;
}
