/*

ioredir.h

This header declares functions that help with IO redirection.

*/

#include "lexer.h"

typedef enum {
	SUCCESS,
	FILE_NOT_FOUND,
	NO_ARG
} redir_result;

redir_result redir_io(tokenlist* tokens);