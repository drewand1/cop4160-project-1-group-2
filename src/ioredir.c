#define STDIN_FD 0
#define STDOUT_FD 1
#define IN_OPEN_FLAGS (O_RDONLY)
#define OUT_OPEN_FLAGS (O_RDWR | O_CREAT | O_TRUNC)

#include <fcntl.h>
#include <unistd.h> // Isn't it lovely how open and close are in different headers?
#include <sys/stat.h>
#include <stdio.h>
#include "ioredir.h"

redir_result redir_io(tokenlist* tokens) {
	for (int i = 0; i < tokens->size; i++) {
		char c = tokens->items[i][0];
		if (c == '<' || c == '>') {
			bool is_in = (c == '<');

			if (i >= tokens->size - 1)
				return NO_ARG;
			
			int fd = open(
				tokens->items[i + 1],
				(is_in) ? IN_OPEN_FLAGS : OUT_OPEN_FLAGS,
				S_IRUSR | S_IWUSR
			);
			if (fd == -1)
				return OPEN_FAIL;

			close((is_in) ? STDIN_FD : STDOUT_FD);
			dup(fd);
			close(fd);
		}
	}

	return SUCCESS;
}

void handle_redir_err(redir_result res) {
	switch(res) {
	case NO_ARG:
		fprintf(
			stderr,
			"\e[41;97mERROR:\e[0m redirect arg not found.\n"
		);
		exit(1);
				
	case OPEN_FAIL:
		fprintf(
			stderr,
			"\e[41;97mERROR:\e[0m could not open file."
		);
		exit(1);

	case SUCCESS:	// Just to shut the compiler up.
		break;
	}
}