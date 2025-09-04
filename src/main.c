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

Another note about the lexer --- that was provided to us, but it just splits by
space. We have two options:
	1. Pray to God that none of Wang's test commands will throw some
		curveball shit at us like "p1 > out.txt" vs. "p1 >out.txt"
		or "p1 | p2" vs. "p1|p2".
	2. Redesign it to handle that and treat '>', '|' and '&'
		as separate tokens from the text they might prefix/suffix.
We should make this decision early, because whether we do this will determine
how we interpret our tokens in our system.
	UPDATE ON THIS: I went ahead and made the get_tokens function handle
	"operators" like |, >, <, and & as separate tokens regardless of whether
	there's a space separating them or not. Better safe than sorry!

*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexer.h"

int main() {
	bool should_run = true;

	while (should_run) {
		printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);

		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

		if (tokens->size == 0)
			continue;

		// exit built-in fn
		if (strcmp(tokens->items[0], "exit") == 0)
			should_run = false;

		free(input);
		free_tokens(tokens);
	}

	return 0;
}