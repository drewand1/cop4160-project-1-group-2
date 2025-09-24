#include "lexer.h"
#include "errhandling.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// it'll be alright the c fairy isn't going to haunt my dreams because I used
// one little macro for an operation that does NOT deserve an entire fn call
// stack frame
#define IS_OP(x) (x == '|' || x == '>' || x == '<' || x == '&')

char *get_input(void) {
	char *buffer = NULL;
	int bufsize = 0;
	char line[5];
	while (fgets(line, 5, stdin) != NULL)
	{
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;
		buffer = (char *)realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;
		if (newln != NULL)
			break;
	}
	buffer = (char *)realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;
	return buffer;
}

tokenlist *new_tokenlist(void) {
	tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **)malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item) {
	int i = tokens->size;

	tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *)malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

/* EMERGENCY BACKUP LEXER --- DO NOT REMOVE!
tokenlist *get_tokens(char *input) {
	char *buf = (char *)malloc(strlen(input) + 1);
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	char *tok = strtok(buf, " ");
	while (tok != NULL)
	{
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}
	free(buf);
	return tokens;
}*/

tokenlist *get_tokens(char *input) {
	// This is an attempt at a lexer that separates "operators" like |, >,
	// <, and & into separate tokens even if they're not separated by space.
	// UPDATE after not-so-rigorous testing: it succeeds! Just in case, I've
	// left the old one commented out. This one is kind of quick and dirty
	// but it works so I'm happy with it if y'all are.
	char *buf = (char *)malloc(strlen(input) + 1);
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	
	int start = 0;

	while (buf[start] != '\0') {
		while (isspace(buf[start]) && buf[start] != '\0')
			start++;

		if (buf[start] == '\0')
			break;

		if (IS_OP(buf[start])) {
			char tok[2] = {buf[start], '\0'};
			add_token(tokens, tok);
			start++;
		} else {
			int end = start;
			char next = buf[end + 1];

			while (!isspace(next) && !IS_OP(next) && next != '\0') {
				end++;
				next = buf[end + 1];
			}

			int sz = end - start + 2;
			char* tok = (char*) malloc(sizeof(char) * sz);

			for (int i = 0; i < sz - 1; i++)
				tok[i] = buf[start + i];
			
			tok[sz - 1] = '\0';

			add_token(tokens, tok);
			free(tok);
			start = end + 1;
		}
	}

	free(buf);
	return tokens;
}

void replace_token(tokenlist* tokens, int i, char* item) {
    if (i < 0 || i >= (int)tokens->size)
        return;
    
    free(tokens->items[i]);
    tokens->items[i] = malloc(strlen(item) + 1);
    assert_exit_ptr(tokens->items[i], "FATAL ERROR: malloc failed in replace_token");
    strcpy(tokens->items[i], item);
}

void remove_token(tokenlist* tokens, int i) {
	if (i < 0 || i > tokens->size - 1)
		return;

	free(tokens->items[i]);
	
	for (int j = i + 1; j <= tokens->size; j++)
		tokens->items[j - 1] = tokens->items[j];
	
	(tokens->size)--;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}
