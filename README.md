# Shell

[Description]

## Group Members
- **Andrew Bradley**: aeb23b@fsu.edu
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 2: Environment Variables
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 3: Tilde Expansion
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 4: $PATH Search
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 5: External Command Execution
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 6: I/O Redirection
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 7: Piping
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 8: Background Processing
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 9: Internal Command Execution
- **Responsibilities**: [Description]
- **Assigned to**: 

### Part 10: External Timeout Executable
- **Responsibilities**: [Description]
- **Assigned to**: 

### Extra Credit
- **Responsibilities**: [Description]
- **Assigned to**: 

## File Listing
```
shell/
│
├── src/
│ ├── main.c
│ └── shell.c
│
├── include/
│ └── shell.h
│
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: e.g., `gcc` for C/C++, `rustc` for Rust.
- **Dependencies**: List any libraries or frameworks necessary (rust only).

### Compilation
For a C/C++ example:
```bash
make
```
This will build the executable in ...
### Execution
```bash
make run
```
This will run the program ...

## Development Log
Each member records their contributions here.

### [Andrew Bradley]

| Date       | Work Completed / Notes                                          |
|------------|-----------------------------------------------------------------|
| 2025-08-30 | Preliminary planning                                            |
| 2025-08-31 | Added replace_token                                             |
| 2025-09-03 | Added remove_token and made lexer treat |, <, >, and & as separate tokens regardless of spacing.|
| 2025-09-10 | Added pipe_split and free_pipe_split fns, made shell split command
up by pipes. Got prompt and environment variable expansion working. Added some
assert functions for error handling for convenience.|
| 2025-09-11 | Made direct-path commands work. Path search still not implemented. Also made the shell have colors!!|

### [Member 2]

| Date       | Work Completed / Notes |
|------------|------------------------|
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |


### [Member 3]

| Date       | Work Completed / Notes |
|------------|------------------------|
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| YYYY-MM-DD | [Names]              | [Agenda items]   | [Actions/Next steps]  |
| YYYY-MM-DD | [Names]              | [Agenda items]   | [Actions/Next steps]  |



## Bugs
- **Bug 1**: This is bug 1.
- **Bug 2**: This is bug 2.
- **Bug 3**: This is bug 3.

## Extra Credit
- **Extra Credit 1**: [Extra Credit Option]
- **Extra Credit 2**: [Extra Credit Option]
- **Extra Credit 3**: [Extra Credit Option]

## Considerations
[Description]
