/*

ioredir.h

This header declares functions that help with IO redirection.

*/

#include "lexer.h"

typedef enum {
	SUCCESS,
	OPEN_FAIL,
	NO_ARG
} redir_result;

// Helper function that shortens dup stuff. Similar to dup2 but idk if we're
// allowed to use that for this project.
// WARNING: closes both old_fd and new_fd.
void qdup(int old_fd, int new_fd);

// Takes in a token list, searches for IO redirection, redirects the IO for the
// current process.
redir_result redir_io(tokenlist* tokens);

// Redirects pipes in a piped command.
// i:		the index of the command in pipe chain.
// pc_size:	size of the pipe chain.
// old_pipe:	old pipe file descriptors.
// new_pipe:	new pipe file descriptors.
void redir_pipes(int i, int pc_size, int old_pipe[2], int new_pipe[2]);

// Handles the result of a redirect.
// WARNING: can exit the process.
void handle_redir_err(redir_result res);