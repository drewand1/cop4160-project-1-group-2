#include "cmdinternal.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

tokenlist previous_cmds = {NULL, 0};


void change_directory(tokenlist* tokens){
    if(tokens->items[1] == NULL){
        // Just "cd" - go to home directory
        char* home = getenv("HOME");
        if(home == NULL){
            fprintf(stderr, "cd: HOME not set\n");
        } else if(chdir(home) != 0){
            perror("cd");
        }
    } else if(tokens->items[2] != NULL){
        // Too many arguments
        fprintf(stderr, "cd: too many arguments\n");
    } else {
        // "cd path" - go to specified directory
        char* path = tokens->items[1];
        
        // Check if path exists
        struct stat path_stat;
        if(stat(path, &path_stat) != 0){
            fprintf(stderr, "cd: %s: No such file or directory\n", path);
        } else if(!S_ISDIR(path_stat.st_mode)){
            fprintf(stderr, "cd: %s: Not a directory\n", path);
        } else if(chdir(path) != 0){
            perror("cd");  // This handles permission errors
        }
    }
}

void add_prev_cmd(char* cmd){
    add_token(&previous_cmds, cmd);
}

char* reconstruct_command(tokenlist* tokens) {
    if (tokens->size == 0) return NULL;
    
    // Calculate total length needed
    size_t total_length = 0;
    for (int i = 0; i < tokens->size; i++) {
        total_length += strlen(tokens->items[i]);
        if (i < tokens->size - 1) total_length += 1; // space between args
    }
    total_length += 1; // null terminator
    
    // Allocate and build the string
    char* command = malloc(total_length);
    if (!command) return NULL;
    
    strcpy(command, tokens->items[0]);
    for (int i = 1; i < tokens->size; i++) {
        strcat(command, " ");
        strcat(command, tokens->items[i]);
    }
    
    return command;
}

void handle_shell_exit(){
    printf("Command history:\n");
    if(previous_cmds.size < 1){
        printf("No previous valid commands\n");
        return;
    }
    
    if(previous_cmds.size < 3) {
        // Print only the last (most recent) command
        size_t last_index = previous_cmds.size - 1;
        printf("%zu: %s\n", last_index + 1, previous_cmds.items[last_index]);
    } else {
        // Print last 3 commands in REVERSE order (most recent first)
        for(int i = previous_cmds.size - 1; i >= (int)(previous_cmds.size - 3); i--) {
            printf("%zu: %s\n", i + 1, previous_cmds.items[i]);
        }
    }

    for(size_t i = 0; i < previous_cmds.size; i++) {
        free(previous_cmds.items[i]);  // Free each string
    }
    free(previous_cmds.items);  // Free the array of pointers
    
    exit(0);  // Actually exit the program
}

 