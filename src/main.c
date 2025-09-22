/*

I've done a little preliminary work like trying to rub my 2.5 braincells
together to create some starting point of a plan for how we might tackle this,
and created a couple of function defs to match. Note that since this is a group
project and everyone else is going to have to live with the consequences of the
plan that is made from the beginning, please PLEASE do not feel obligated to
blindly go along with it if you don't like it. Let's discuss it --- point out
the pitfalls, call me on my shit, and if we really want to we can come up with
something entirely different. I've just tried to create something to get the
ball rolling --- if it works, it works; if it doesn't, we'll change it.

-- Andrew

The command pipeline:
	1. Prompt; receive input.

	[FORMAT STAGE; stuff for this is in cmdformat.h; no fn defs yet]
	2. Tokenize input.
	3. Split by pipes into separate commands/token lists. For each,
	4. Expand env vars and tildes.
	5. Resolve command (will either be a built in fn, relative to
		pwd or in $PATH; probably best to check in that order).

	[PRE-EXECUTION STAGE]
	6. Fork
	7. Resolve stdin; will either be redirect with '<', result of
		pipe, or default. If redirect is present, redirect fd.
	8. Resolve stdout; will either be redirect with '>', result of
		pipe, or default. If redirect is present, redirect fd.
	9. Determine whether background or foreground (based on presence
		of '&' token). Presumably all that changes is that if it's
		background we don't call wait?
	10. Make command line args (all tokens which are not redirects or
		ampersands in the command; ends with null. Check execv manpage
		for better explanation).

	[EXECUTION STAGE]
	11. Execute w/ command args

Now there are some holes in that:
	- We need to identify potential errors --- a LOT of things can go wrong
		in user input to a shell and we have to catch all of those edge
		cases and report them.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "cmdformat.h"
#include "errhandling.h"
#include "lexer.h"

int main() {
	bool should_run = true;

	while (should_run) {
		// PROMPT
		char* user = getenv("USER");
		char* machine = getenv("HOSTNAME");
		if (!machine) {
			machine = getenv("MACHINE");
		}
		char pwd_buffer[1024];
		char* pwd = getcwd(pwd_buffer, sizeof(pwd_buffer));

		assert_exit_ptr(user, "FATAL ERROR: USER environment variable not defined.");
		assert_exit_ptr(pwd, "FATAL ERROR: PWD environment variable not defined.");

		if (machine)
			printf("\e[0;31m%s@%s\e[0m:\e[0;36m%s\e[0m> ", user, machine, pwd);
		else
			printf("\e[0;31m%s\e[0m:\e[0;36m%s\e[0m> ", user, pwd);

		// GETTING AND PROCESSING INPUT
		char *input = get_input();
		//printf("whole input: %s\n", input);

		tokenlist* init_tokens = get_tokens(input);
		expand_env_vars(init_tokens);
		expand_tilde(init_tokens);
		pipe_chain pc;
		pipe_split(init_tokens, &pc);

		free_tokens(init_tokens);
		free(input);

		for (int i = 0; i < pc.size; i++) {
			//printf("[[ CMD %d ]]\n", i);

			tokenlist* tokens = pc.cmds[i];			
			/*for (int j = 0; j < tokens->size; j++) {
				printf("token %d: (%s)\n", j, tokens->items[j]);
			}*/

			if (tokens->size == 0)
				continue;

			// exit built-in fn
			if (strcmp(tokens->items[0], "exit") == 0)
				should_run = false;

			// Running external commands
			if (fork() == 0) {
				// Add null terminator for execv
				char** args = malloc((tokens->size + 1) * sizeof(char*));
				for (int j = 0; j < tokens->size; j++) {
					args[j] = tokens->items[j];
				}
				args[tokens->size] = NULL;
				
				// Try to execute directly first
				execv(tokens->items[0], args);
				
				// If that fails, try common paths
				char full_path[256];
				snprintf(full_path, sizeof(full_path), "/bin/%s", tokens->items[0]);
				execv(full_path, args);
				
				snprintf(full_path, sizeof(full_path), "/usr/bin/%s", tokens->items[0]);
				execv(full_path, args);
				
				// If we get here, command wasn't found
				printf("Error: Command not found. %s\n", tokens->items[0]);
				exit(1);
			} else {
				int status;
				waitpid(-1, &status, 0);
			}
		}
		
		free_pipe_split(&pc);
	}

	return 0;
}