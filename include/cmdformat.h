/*

cmdformat.h

This header declares functions that format a command to get its content ready
for execution. E.g. expanding environment variables, splitting by pipes, etc.

*/

#ifndef CMDFORMAT_H
#define CMDFORMAT_H

#include <stdbool.h>
#include "lexer.h"
#include "unistd.h"

typedef struct {
	int size;
	tokenlist** cmds;
} pipe_chain;

// Takes in a list of tokens obtained by tokenizing direct user input.
// Splits them into separate sub-token-lists by each pipe ('|') token, such that
// the token list {"p1", "arg", "|", "p2", "param"} results in the token list
// array {{"p1", "arg"}, {"p2", "cmd"}}.
// in_toks	:	pointer to the input token list
// out_toks_arr	:	pointer to the first element in the array of pointers to
//			output token lists; is a result parameter and will be
//			mutated.
// int* size	:	pointer to variable containing the size of out_toks_arr;
//			is a result parameter and will be mutated.
void pipe_split(tokenlist* in_toks, pipe_chain* pc);

// Frees a list of token lists obtained from pipe_split.
void free_pipe_split(pipe_chain* pc);

// Takes in a list of tokens and expands all environment variables (tokens that
// start with '$').
void expand_env_vars(tokenlist* tokens);

// Takes in a list of tokens and, if the first one does not begin with ".",
// replaces it with the full path to the command.
int path_search(tokenlist* tokens);

// Creates an argument list suitable for execv from a token list; that is,
// strips it of redirect-related tokens and ampersands, and makes all entries
// but the first a string, separating them by a space.
char** make_arg_list(tokenlist* tokens);

// Takes in list of tokens and, if a token is equal to '~' or begins with '~',
// Replaces it with contents of $HOME environment variable
void expand_tilde(tokenlist* tokens);

// Returns true if token list contains the given token.
bool tklist_contains(tokenlist* toks, char* tok);

// Returns true if a pipe chain contains the given token.
bool pc_contains(pipe_chain* pc, char* tok);

// This is the the argument list to free; all strings in the array
// as well as the array itself will be deallocated.
void free_arg_list(char** arg_list);

#endif