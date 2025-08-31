/*

cmdformat.h

This header declares functions that format a command to get its content ready
for execution. E.g. expanding environment variables, splitting by pipes, etc.

*/

#ifndef CMDFORMAT_H
#define CMDFORMAT_H

#include "lexer.h"

// Takes in a list of tokens obtained by tokenizing direct user input.
// Splits them into separate sub-token-lists by each pipe ('|') token, such that
// the token list {"p1", "arg", "|", "p2", "param"} results in the token list
// array {{"p1", "arg"}, {"p2", "cmd"}}.
// in_toks	:	pointer to the input token list
// out_toks_arr	:	pointer to the first element in the array of output
//			token lists; is a result parameter and will be mutated.
// int* size	:	pointer to variable containing the size of out_toks_arr;
//			is a result parameter and will be mutated.
void pipe_split(tokenlist* in_toks, tokenlist* out_toks_arr, int* size);

// Frees a list of token lists obtained from pipe_split.
void free_pipe_split(tokenlist* toks_arr, int size);

// Takes in a list of tokens and expands all environment variables (tokens that
// start with '$').
void expand_env_vars(tokenlist* tokens);

// Takes in a list of tokens and, if the first one does not begin with ".",
// replaces it with the full path to the command.
void path_search(tokenlist* tokens);

// Creates an argument list suitable for execv from a token list; that is,
// strips it of redirect-related tokens and ampersands, and makes all entries
// but the first a string, separating them by a space.
char* make_arg_list(tokenlist* tokens);

// Utility function that will help us with the find-and-replace-esque nature of
// the other functions in cmdformat.h.
void replace_string(char* orig_str, char* new_str);

#endif