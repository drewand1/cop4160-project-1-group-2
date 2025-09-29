# Shell

This is a very rudimentary shell. It does everything it's supposed to do, plus
the extra credit.

It has colors :)

## Group Members
- **Andrew Bradley**: aeb23b@fsu.edu
  **Ryan Kurfirst**:  rsk22a@fsu.edu
  **Daniel Halterman**: drh22a@fsu.edu

## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Displays a prompt to the user containing their username, machine name, and pwd.
- **Assigned to**: 
    Daniel Halterman
    Andrew Bradley
### Part 2: Environment Variables
- **Responsibilities**: Expands env vars in commands.
- **Assigned to**: 
    Andrew Bradley
    Ryan Kurfirst
### Part 3: Tilde Expansion
- **Responsibilities**: Expands tildes in commands.
- **Assigned to**: 
    Ryan Kurfirst
    Daniel Halterman
### Part 4: $PATH Search
- **Responsibilities**: Searches $PATH to find commands.
- **Assigned to**: 
    Ryan Kurfirst
    Daniel Halterman
### Part 5: External Command Execution
- **Responsibilities**: Executes external programs specified by commands.
- **Assigned to**: 
    Andrew Bradley
    Ryan Kurfirst
### Part 6: I/O Redirection
- **Responsibilities**: Redirect I/O as specified by commands.
- **Assigned to**: 
    Daniel Halterman
    Andrew Bradley
### Part 7: Piping
- **Responsibilities**: Allow for piping between commands (it's unlimited 😎).
- **Assigned to**: 
    Andrew Bradley
    Ryan Kurfirst
### Part 8: Background Processing
- **Responsibilities**: Allow for commands to be executed in the background.
- **Assigned to**: 
    Daniel Halterman 
    Andrew Bradley
### Part 9: Internal Command Execution
- **Responsibilities**: Implement exit, cd, and jobs.
- **Assigned to**: 
    Ryan Kurfirst
    Daniel Halterman
### Part 10: External Timeout Executable
- **Responsibilities**: This task isn't in the project rubric. We have no clue what this means.
- **Assigned to**: 

### Extra Credit
- **Responsibilities**: Executing the shell from within the shell, infinite piping, and piping + io redirection.
- **Assigned to**: 
Andrew Bradley

## File Listing
```
shell/
│
├── src/
│ ├── main.c
│ ├── lexer.c
│ ├── errhandling.c
│ ├── bgproc.c
│ ├── ioredir.c
│ ├── cmdinternal.c
│ └── cmdformat.c
├── include/
│ ├── cmdformat.h
│ ├── errhandling.h
│ ├── lexer.h
│ ├── bgproc.h
│ ├── cmdinternal.h
│ └── ioredir.h
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc

### Compilation
To build, run the following:
```bash
make
```
This will make the executable in [project folder]/bin/shell. You can directly
execute this executable with ./bin/shell

## Development Log
Each member records their contributions here.

### [Andrew Bradley]

| Date       | Work Completed / Notes                                          |
|------------|-----------------------------------------------------------------|
| 2025-08-30 | Preliminary planning                                            |
| 2025-08-31 | Added replace_token                                             |
| 2025-09-03 | Added remove_token and made lexer treat |, <, >, and & as separate tokens regardless of spacing.|
| 2025-09-10 | Added pipe_split and free_pipe_split fns, made shell split command up by pipes. Got prompt and environment variable expansion working. Added some assert functions for error handling for convenience.|
| 2025-09-11 | Made direct-path commands work. Path search still not implemented. Also made the shell have colors!!|
| 2025-09-22 | Miscellaneous fix-ups. |
| 2025-09-24 | Made IO redirection. Fun fact: this change was pushed using our shell. 😎 |
| 2025-09-26 | Made pipes. |
| 2025-09-28 | Made background processing. Made exit wait for background processes. Made jobs internal command. Also fixed up some readme stuff. |

### [Ryan Kurfirst]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2025-09-22 | Added tilde expansion and $PATH search  |
| 2025-09-26 | Added exit and cd internal commands |


### [Daniel Halterman]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2025-09-22 | Makefile fixes, prompt fixes, tweaks on external commands. |
| 2025-09-24 | Fixed replace_token, free_pipe_split, and expand_tilde for memory safety, tweaks on path_search to fix extra error msg bug. |
| 2025-09-27 | Added missing include guard in lexer.h, added missing library, added new io redirection function. |
| 2025-09-29 | Final tests to make sure program is working properly. |



## Meetings
Document in-person meetings, their purpose, and what was discussed.

We had no in-person meetings for this project. Everything occurred over email
and text.


## Bugs
- **Errors In Pipes**: Unexpected behavior may occur when a command in a pipe chain errors.
- **2 commands**: If only 2 commands were run in the shell process's lifespan, the command log displayed when exit is run will only show the second.

## Extra Credit
- **Extra Credit 1**: Unlimited pipes - this shell supports an unlimited number of pipes in one command.
- **Extra Credit 2**: Shell-ception - you can execute this shell using this shell as many times as you want.
- **Extra Credit 3**: Piping and I/O redirection - you can redirect input on the first command and redirect output on the last command in a pipe chain. This shell does not, however, support I/O redirection on commands that are not either the first or last in the pipe chain.

**We have completed all 3 extra credit tasks.**

## Considerations
Note that when combining IO redirection and pipes, you may only redirect input
to the first command in the pipe chain, and you may only redirect output from
the last command in the pipe chain.

This project was finished at 9/28/2025 at 9:17 PM, a little over 24 hours before
the due date. We sincerely apologize if there are any bugs we didn't catch. But
bugs make for more fun, don't they?
