/* author: jake pitkin
 * last edit: april 25 2018
 * assignment 5 - problem 2
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Validate input
    if (argc != 2) {
	fprintf(stderr, "Invalid number of arguments.\n");
    }
    dir_path = argv[1];
    return 0;
}
