#define STDIN_FD 0
#define STDOUT_FD 1
#define IN_OPEN_FLAGS (O_RDONLY)
#define OUT_OPEN_FLAGS (O_RDWR | O_CREAT | O_TRUNC)

#include <fcntl.h>
#include <unistd.h> // Isn't it lovely how open and close are in different headers?
#include <sys/stat.h>
#include <stdio.h>
#include "ioredir.h"

void qdup(int old_fd, int new_fd) {
	close(old_fd);
	dup(new_fd);
	close(new_fd);
}

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

			qdup((is_in) ? STDIN_FD : STDOUT_FD, fd);
		}
	}

	return SUCCESS;
}

void redir_pipes(int i, int pc_size, int old_pipe[2], int new_pipe[2]) {
	if (i != 0)
		qdup(STDIN_FD, old_pipe[0]);

	if (i < pc_size - 1)
		qdup(STDOUT_FD, new_pipe[1]);
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