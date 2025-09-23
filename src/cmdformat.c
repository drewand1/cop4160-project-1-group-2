#define _GNU_SOURCE 
#include "cmdformat.h"
#include "errhandling.h"
#include <stdio.h>  
#include <string.h>    
#include <unistd.h>   
#include <stdlib.h> 


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

void expand_tilde(tokenlist* tokens){
	for (int i = 0; i < tokens->size; i++) {
		if (tokens->items[i][0] != '~')
		continue;

		char* val = getenv("HOME");
		if (!val)
			val = "";
		replace_token(tokens, i, val);
	}
}

void path_search(tokenlist* tokens) {
	// Return early if first already has path to command
	if(strchr(tokens->items[0], '/') != NULL)
	return;

	char* command_token = tokens->items[0];
	unsigned int num_paths = 0;
	char** paths;
	char* path_token;
	char* path_str = getenv("PATH");
	assert_exit_ptr(path_str, "FATAL ERROR: PATH environment variable not defined.");

	char* path_copy = strdup(path_str); // Copy path so strtok doesnt modify the user's original path

	// First pass to get number of paths
	path_token = strtok(path_copy,":");
	while(path_token != NULL){
		num_paths++;
		path_token = strtok(NULL, ":");
	}

	free(path_copy);

	// Second pass to store tokens in str array
	paths = malloc(num_paths * sizeof(char*));

	path_copy = strdup(path_str);
	path_token = strtok(path_copy, ":");
	int i = 0;

	while(path_token != NULL){
		paths[i] = strdup(path_token);
		i++;
		path_token = strtok(NULL, ":");
	}

	free(path_copy);

	// Now we have paths array containing pointers to the individual path strings 

	for(int j = 0; j < num_paths; j++){
		int path_len = strlen(paths[j]) + strlen(command_token) + 2;
		char* full_path = malloc(path_len);
		snprintf(full_path, path_len, "%s/%s", paths[j], command_token);  

		// Check if there is a binary found on potential path
		if (access(full_path, X_OK) == 0){

			free(tokens->items[0]);

			// Set command token to full path string
			tokens->items[0] = full_path;
			for(int k = 0; k < num_paths; k++) free(paths[k]);
            free(paths);
			return;
		}
		free(full_path);
	}

	// If path was never found print error and clear allocations
	printf("Command not found: %s\n", command_token);
	for(int k = 0; k < num_paths; k++) free(paths[k]);
    free(paths);

	
}

char* make_arg_list(tokenlist* tokens) {}