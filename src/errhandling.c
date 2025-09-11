#include <stdint.h>
#include <stdlib.h>
#include "errhandling.h"

void assert_exit_ptr(void* ptr, char* msg) {
	if (!ptr) {
		fprintf(stderr, "%s\n", msg);
		exit(-1);
	}
}

void assert_exit_int(int cond, char* msg) {
	if (!cond) {
		fprintf(stderr, "%s\n", msg);
		exit(-1);
	}
}