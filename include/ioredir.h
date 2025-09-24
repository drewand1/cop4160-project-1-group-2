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

// Takes in a token list, searches for IO redirection, redirects the IO for the
// current process.
redir_result redir_io(tokenlist* tokens);

// Handles the result of a redirect.
// WARNING: can exit the process.
void handle_redir_err(redir_result res);