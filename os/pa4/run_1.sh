gcc -O2 -Wall -Wextra -Werror problem_1.c -o problem_1 -lm -pthread
taskset -c 1 ./problem_1 99 5
