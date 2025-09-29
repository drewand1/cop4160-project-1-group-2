#ifndef CMDINTERNAL_H
#define CMDINTERNAL_H

#include "bgproc.h"
#include "lexer.h"
#include "unistd.h"
#include "cmdformat.h"

extern tokenlist previous_cmds;

char* reconstruct_command(tokenlist* tokens);

void add_prev_cmd(char* cmd);

void handle_shell_exit(void);

void change_directory(tokenlist* tokens);

void list_jobs(job_list* jl);

#endif