#include <stdio.h>
#include <assert.h>

unsigned long byte_sort(unsigned long args);
unsigned long nibble_sort(unsigned long args);
struct elt *name_list(void);

struct elt {
  char val;
  struct elt *link;
};

enum format_t {
  OCT = 66, BIN, HEX
};
void convert (enum format_t mode, unsigned long value);

int main(void) {
  printf("Testing byte_sort\n");
  unsigned long sorted_bytes = byte_sort(0x0403deadbeef0201);
  unsigned long expected_sort = 0xefdebead04030201;
  if (sorted_bytes != expected_sort) {
   printf("byte_sort incorrect\n");
  } else {
    printf("byte_sort correct\n"); 
  }
  printf("Expected: %lx\n", expected_sort);
  printf("Actual: %lx\n", sorted_bytes);

  printf("\n");
  printf("Testing nibble_sort\n");
  unsigned long sorted_nibble = nibble_sort(0x0403deadbeef0201); 
  unsigned long expected_sorted_nibble = 0xfeeeddba43210000;
  if (sorted_nibble != expected_sorted_nibble) {
    printf("nibble_sort incorrect\n");
  } else {
    printf("nibble_sort correct\n");
  }
  printf("Expected: %lx\n", expected_sorted_nibble);
  printf("Actual: %lx\n", sorted_nibble);

  printf("\n");
  printf("Testing name_list");
  struct elt *head = name_list();
  struct elt *cur = head;
  printf("\n");
  while (cur != NULL) {
    char letter = cur->val;
    printf("%c", letter);
    cur = cur->link;
  }
  printf("\n"); 
  if (cur == NULL) {
    printf("End of linked list is NULL as expected.\n");
  } else {
    printf("End of linked list is not NULL!\n");
  }

  printf("\n");
  printf("Testing convert\n");
  convert(HEX, 0xdeadbeef);
  convert(BIN, 0x7);
  convert(OCT, 0x492492);
  convert(66, 18446744073709551615lu);
  convert(0, 0xdeadbeef);
}
