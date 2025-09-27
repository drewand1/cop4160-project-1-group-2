#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char** items;
    int size;
} tokenlist;

char* get_input(void);
tokenlist* new_tokenlist(void);
void add_token(tokenlist* tokens, char* item);
tokenlist* get_tokens(char* input);
void replace_token(tokenlist* tokens, int i, char* item);
void remove_token(tokenlist* tokens, int i);
void free_tokens(tokenlist* tokens);

#endif