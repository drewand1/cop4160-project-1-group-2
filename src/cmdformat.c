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
    if (!pc)
        return;
    
    if (pc->cmds) {
		// Free each command's tokens
        for (int i = 0; i < pc->size; i++) {
            if (pc->cmds[i])
                free_tokens(pc->cmds[i]);
        }
		// Free cmds array
        free(pc->cmds);
    }
    
	// Make cmds NULL and reset the size to 0
    pc->cmds = NULL;
    pc->size = 0;
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

		// NOTE: initial version expanded the tilde but replaced the
		// whole token, chopping off a potential path after the tilde.
		// The following bit includes it. It's a lil rough but it works.
		int home_len = strlen(val);
		int path_len = strlen(tokens->items[i]);
		int new_len = home_len + path_len;
		char* new_tok = (char*) malloc(sizeof(char) * (new_len + 1));
		// safety check that prevents your shell from crashing if memory allocation fails
		assert_exit_ptr(new_tok, "FATAL ERROR: malloc failed in expand_tilde");

		strcpy(new_tok, val);

		if (path_len > 1) // Works only if there's something after the '~'
			strcpy(new_tok + home_len, tokens->items[i] + 1);		

		replace_token(tokens, i, new_tok);
		free(new_tok);
	}
}

int path_search(tokenlist* tokens) {
    // Return early if first already has path to command
    if (strchr(tokens->items[0], '/') != NULL)
        return 0; // Success - has path

    char* command_token = tokens->items[0];
    unsigned int num_paths = 0;
    char** paths;
    char* path_token;
    char* path_str = getenv("PATH");
    assert_exit_ptr(path_str, "FATAL ERROR: PATH environment variable not defined.");

    char* path_copy = strdup(path_str); // Copy path so strtok doesn't modify original

    // First pass to get number of paths
    path_token = strtok(path_copy, ":");
    while (path_token != NULL) {
        num_paths++;
        path_token = strtok(NULL, ":");
    }

    free(path_copy);

    // Second pass to store tokens in array
    paths = malloc(num_paths * sizeof(char*));
    path_copy = strdup(path_str);
    path_token = strtok(path_copy, ":");
    int i = 0;

    while (path_token != NULL) {
        paths[i] = strdup(path_token);
        i++;
        path_token = strtok(NULL, ":");
    }

    free(path_copy);

    // Search for executable in paths
    for (int j = 0; j < num_paths; j++) {
        int path_len = strlen(paths[j]) + strlen(command_token) + 2;
        char* full_path = malloc(path_len);
        snprintf(full_path, path_len, "%s/%s", paths[j], command_token);

        // Check if binary exists and is executable
        if (access(full_path, X_OK) == 0) {
            free(tokens->items[0]);
            tokens->items[0] = full_path; // Set command to full path
            for (int k = 0; k < num_paths; k++) free(paths[k]);
            free(paths);
            return 0; // Success
        }
        free(full_path);
    }

    // Command not found
    printf("Command not found: %s\n", command_token);
    for (int k = 0; k < num_paths; k++) free(paths[k]);
    free(paths);
    return -1; // Failure
} 


// Makes a list of arguments to use with execv from a list of tokens.
// It skips special symbols like >, <, and &.
// Tokens : the list of words typed by the user
// Returns : a new array of strings ready for execv
char** make_arg_list(tokenlist* tokens) {
    // Count how many tokens we'll keep (excluding redirection operators and files)
    int arg_count = 0;
    for (int i = 0; i < tokens->size; i++) {
        char* token = tokens->items[i];
        if (token[0] == '>' || token[0] == '<') {
            i++;
            continue;
        }
        
        // Skip & for background processes
        if (token[0] == '&') {
            continue;
        }
        
        arg_count++;
    }
    
    // Create null-terminated array for execv
    char** arg_list = (char**) malloc((arg_count + 1) * sizeof(char*));
    
    // Fill the argument list
    int arg_index = 0;
    for (int i = 0; i < tokens->size; i++) {
        char* token = tokens->items[i];
        
        // Skip redirection operators and their arguments
        if (token[0] == '>' || token[0] == '<') {
            i++; // Skip the next token (the filename) as well
            continue;
        }
        
        // Skip & for background processes
        if (token[0] == '&') {
            continue;
        }
        
        // Copy the token to the argument list
        arg_list[arg_index] = strdup(token);
        arg_index++;
    }
    
    arg_list[arg_count] = NULL;
    return arg_list;
}

// Helper function to free the argument list created by make_arg_list
void free_arg_list(char** arg_list) {
    if (!arg_list) return;
    
    for (int i = 0; arg_list[i] != NULL; i++) {
        free(arg_list[i]);
    }
    free(arg_list);
}

bool tklist_contains(tokenlist* toks, char* tok) {
	for (int i = 0; i < toks->size; i++)
		if (strcmp(toks->items[i], tok) == 0)
			return true;

	return false;
}

bool pc_contains(pipe_chain* pc, char* tok) {
	for (int i = 0; i < pc->size; i++)
		if (tklist_contains(pc->cmds[i], tok))
			return true;
	
	return false;
}