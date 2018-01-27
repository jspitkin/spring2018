#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*********************************************************************
 *
 * These C functions use patterns and functionality often found in
 * operating system code. Your job is to implement them. Of course you
 * should write test cases. However, do not hand in your test cases
 * and (especially) do not hand in a main() function since it will
 * interfere with our tester.
 *
 * Additional requirements on all functions you write:
 *
 * - you may not refer to any global variables
 *
 * - you may not call any functions except those specifically
 *   permitted in the specification
 *
 * - your code must compile successfully on CADE lab Linux
 *   machines when using:
 *
 * /usr/bin/gcc -O2 -fmessage-length=0 -pedantic-errors -std=c99 -Werror -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-include-dirs -Wno-unused-parameter -Wshadow -Wuninitialized -Wold-style-definition -c assign1.c 
 *
 * NOTE 1: Some of the specifications below are specific to 64-bit
 * machines, such as those found in the CADE lab.  If you choose to
 * develop on 32-bit machines, some quantities (the size of an
 * unsigned long and the size of a pointer) will change. Since we will
 * be grading on 64-bit machines, you must make sure your code works
 * there.
 *
 * NOTE 2: You should not need to include any additional header files,
 * but you may do so if you feel that it helps.
 *
 * HANDIN: submit your finished file, still called assign1.c, in Canvas
 *
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 *
 *********************************************************************/
unsigned long byte_sort (unsigned long arg)
{
  unsigned char bytes[8];
  // Split long into 8 bytes
  for (int i = 0; i < 8; i++) {
    bytes[i] = (arg >> (8 * i)) & 0xFF;
  }
  // Selection sort on the 8 bytes
  for (int i = 0; i < 7; i++) {
    int min_idx = i;
    for (int j = i+1; j < 8; j++) {
      if (bytes[j] < bytes[min_idx]) {
        min_idx = j;
      }
    }
    if (min_idx != i) {
      unsigned char tmp = bytes[i];
      bytes[i] = bytes[min_idx];
      bytes[min_idx] = tmp;
    }
  }
  // Arrange sorted bytes into unsigned long
  unsigned long result = 0;
  for (int i = 0; i < 8; i++) {
    unsigned long shift_bits = 0;
    shift_bits = shift_bits | bytes[i];
    shift_bits = shift_bits << (8 * i);
    result = result | shift_bits;
  }
  return result;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
  unsigned char bits[16];
  // Split long into 16 4-bit numbers
  for (int i = 0; i < 16; i++) {
    bits[i] = (arg >> (4 * i)) & 0xF;
  }
  // Selection sort on the 16 4-bit numbers
  for (int i = 0; i < 15; i++) {
    int min_idx = i;
    for (int j = i+1; j < 16; j++) {
      if (bits[j] < bits[min_idx]) {
        min_idx = j;
      }
    }
    if (min_idx != i) {
      unsigned char tmp = bits[i];
      bits[i] = bits[min_idx];
      bits[min_idx] = tmp;
    }
  }
  // Arrange sorted bytes into unsigned long
  unsigned long result = 0;
  for (int i = 0; i < 16; i++) {
    unsigned long shift_bits = 0;
    shift_bits = shift_bits | bits[i];
    shift_bits = shift_bits << (4 * i);
    result = result | shift_bits;
  }
  return result;
}

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *  
 *********************************************************************/

struct elt {
  char val;
  struct elt *link;
};

static void clean_up(struct elt *head) {
  // Given a pointer to the head of a linked list, frees the allocated memory of the list
  struct elt *cur;
  while (head != NULL) {
    cur = head;
    head = head->link;
    free(cur);
  }
}

struct elt *name_list (void)
{
  // My name is Jake
  char name[4];
  name[0] = 'J';
  name[1] = 'a';
  name[2] = 'k';
  name[3] = 'e';

  // Create a pointer to the head of the linked list
  struct elt *head = malloc(sizeof(struct elt));
  if (head == NULL) {
    return NULL;
  }
  struct elt * cur = head;
  cur->val = name[0];
  // Add the rest of the letters
  for (int i = 1; i < 4; i++) {
    struct elt *nxt = malloc(sizeof(struct elt));
    if (nxt == NULL) {
      // Something went wrong with malloc - clean up and return NULL
      clean_up(head);
      return NULL;
    }
    cur->link = nxt; 
    nxt->val = name[i];
    cur = nxt;
  }
  cur->link = NULL;
  
  return head;
}

/*********************************************************************
 *
 * convert()
 *
 * specification: depending on the value of "mode", print "value" as
 * octal, binary, or hexidecimal to stdout, followed by a newline
 * character
 *
 * extra requirement 1: output must be via putc()
 *
 * extra requirement 2: print nothing if "mode" is not one of OCT,
 * BIN, or HEX
 *
 * extra requirement 3: all leading/trailing zeros should be printed
 *
 * EXMPLE: convert (HEX, 0xdeadbeef) should print
 * "00000000deadbeef\n" (including the newline character but not
 * including the quotes)
 *
 *********************************************************************/

enum format_t {
  OCT = 66, BIN, HEX
};

void convert (enum format_t mode, unsigned long value)
{
  // Buffer for the HEX mode and a mapping to hex characters
  char hex_buffer[17];
  char hex_map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  // Buffer for the BIN mode
  char bin_buffer[65];
  // Buffer for the OCT mode
  char oct_buffer[23];

  switch (mode) {
    case OCT:
      for (int i = 0; i < 22; i++) {
        // Mask off 3-bits at a time
        int cur = (value >> (3 * i)) & 0x7;
        // Convert the 3 bits to a '0-7' character
        oct_buffer[21 - i] = cur + '0';
      }
      oct_buffer[22] = '\n';
      for (int i = 0; i < 23; i++) {
        // Print the result with a newline
        putc(oct_buffer[i], stdout);
      }
    break;
    case BIN:
      for (int i = 0; i < 64; i++) {
        // Mask off each bit of the input
        int cur = (value >> i & 0x1);
        // Convert to the bit to a '0' or '1' character
        bin_buffer[63 - i] = cur + '0';
      }
      bin_buffer[64] = '\n'; 
      for (int i = 0; i < 65; i++) {
        // print the result with a newline
        putc(bin_buffer[i], stdout);
      }
    break;
    case HEX:
      for (int i = 0; i < 16; i++) {
        // Mask of each hex character in the input
        int cur  = (value >> (4 * i)) & 0xF;
        // Map the hex value to the proper hex character
	hex_buffer[15 - i] = hex_map[cur];
      }
      hex_buffer[16] = '\n';
      // Print the result with a newline
      for (int i = 0; i < 17; i++) {
        putc(hex_buffer[i], stdout);
      }
    break;
    default:
      return;
  }
}

/*********************************************************************
 *
 * draw_me()
 *
 * this function creates a file called me.txt which contains an ASCII-art
 * picture of you (it does not need to be very big). the file must (pointlessly,
 * since it does not contain executable content) be marked as executable.
 * 
 * extra requirement 1: you may only call the function syscall() (type "man
 * syscall" to see what this does)
 *
 * extra requirement 2: you must ensure that every system call succeeds; if any
 * fails, you must clean up the system state (closing any open files, deleting
 * any files created in the file system, etc.) such that no trash is left
 * sitting around
 *
 * you might be wondering how to learn what system calls to use and what
 * arguments they expect. one way is to look at a web page like this one:
 * http://blog.rchapman.org/post/36801038863/linux-system-call-table-for-x86-64
 * another thing you can do is write some C code that uses standard I/O
 * functions to draw the picture and mark it as executable, compile the program
 * statically (e.g. "gcc foo.c -O -static -o foo"), and then disassemble it
 * ("objdump -d foo") and look at how the system calls are invoked, then do
 * the same thing manually using syscall()
 *
 *********************************************************************/

void draw_me (void)
{
  // Create the file with read/write/execute permissions
  int fc = syscall(85, "me.txt", S_IRWXU);
  if (fc < 0) {
    // Delete me.txt from disk
    syscall(87, "me.txt");
  }

  // Open the file
  int fd = syscall(2, "me.txt", O_WRONLY);
  if (fd < 0) {
    // Close me.txt and delete it from disk
    syscall(3, fd);
    syscall(87, "me.txt");
  }

  // Draw a little cartoon face in the file
  char buf[] = "-//////\n// o  o\n(    > )\n \\  o / nice, i'm done\n  ----\n";
  int bw = syscall(1, fd, buf, 55); 
  if (bw < 0) {
    // Close me.txt and delete it from disk
    syscall(3, fd);
    syscall(87, "me.txt");
  }
  
  // Close the file
  int fcl = syscall(3, fd);
  if (fcl < 0) {
    // Try to close me.txt again and delete it from disk
    syscall(3, fd);
    syscall(87, "me.txt");
  }
}

