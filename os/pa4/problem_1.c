/* author: jake pitkin
 * last edit: march 27 2018
 * assignment 4 - problem 1
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }
    long threads, seconds;
    threads = strtol(argv[1], NULL, 10);
    seconds = strtol(argv[2], NULL, 10);
    if (threads == 0) {
        fprintf(stderr, "Invalid input for threads. Expected [1, 99].\n");
        return -1;
    }
    if (seconds == 0) {
        fprintf(stderr, "Invalid input for seconds. Expected positive integer.\n");
        return -1;
    }
    return 0;
}
