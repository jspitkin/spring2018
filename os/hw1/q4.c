#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Original process with pid: %d\n", (int) getpid());
    int rc1 = fork();
    if (rc1 == 0) 
      printf("A new child from fork 1 with pid: %d\n", (int) getpid());
    else 
      printf("The parent %d created a process on fork 1\n", (int) getpid());
    int rc2 = fork();
    if (rc2 == 0) 
        printf("A new child from fork 2 with pid: %d\n", (int) getpid());
    else 
      printf("The parent %d created a process on fork 2\n", (int) getpid());
    int rc3 = fork();
    if (rc3 == 0) 
        printf("A new child from fork 3 with pid: %d\n", (int) getpid());
    else 
      printf("The parent %d created a process on fork 3\n", (int) getpid());
    return 1;
}
