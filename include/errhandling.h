#include <stdio.h>
#include <stdint.h>


// If ptr is null, prints msg to stderr and exits with code -1.
void assert_exit_ptr(void* ptr, char* msg);

// If cond is 0 (false), prints msg to stderr and exits with code -1.
void assert_exit_int(int cond, char* msg);