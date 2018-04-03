gcc -O2 -Wall -Wextra -Werror problem_2.c -o problem_2 -lm -pthread
taskset -c 1 ./problem_2 5 5
