#include "cmdformat.h"
#include <stdio.h>

#define IS_PIPE(x) (x[0] != '|')

void pipe_split(tokenlist* in_toks, pipe_chain* pc) {
	// Getting size
	int pipe_count = 0, size = 0;

	for (int i = 0; i < in_toks->size; i++)
		if (in_toks->items[i][0] == '|')
			pipe_count++;
	
	size = pipe_count + 1;
	pc->size = size;

	// Initializing and filling up token lists
	pc->cmds = (tokenlist**) malloc(sizeof(tokenlist*) * size);
	int curs = 0;

	for (int i = 0; i < size; i++) {
		pc->cmds[i] = new_tokenlist();

		while (curs < in_toks->size && in_toks->items[curs][0] != '|') {
			add_token(pc->cmds[i], in_toks->items[curs]);
			curs++;
		}

		curs++;
	}
}

void free_pipe_split(pipe_chain* pc) {
	for (int i = 0; i < pc->size; i++)
		free_tokens(pc->cmds[i]);
	// pc is not necessarily dynamically allocated so we wouldn't call
	// free(pc) here.
}

void expand_env_vars(tokenlist* tokens) {
	for (int i = 0; i < tokens->size; i++) {
		if (tokens->items[i][0] != '$')
			continue;
		
		char* val = getenv(tokens->items[i] + 1); // token without begining '$'
		if (!val)
			val = ""; // My shell seems to replace null vars with a blank str.
				  // So, I'm gonna do that too.
		replace_token(tokens, i, val);
	}
}

void path_search(tokenlist* tokens) {}

char* make_arg_list(tokenlist* tokens) {}