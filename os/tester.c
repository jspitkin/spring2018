#include <stdio.h>
#include <assert.h>

int main() {
  printf("Testing byte_sort\n");
  assert(byte_sort(0x0403deadbeef0201) == 0xefdebead04030201);
  printf("byte_sort correct\n"); 
}
