/*

bgproc.h

This file declares some functions that are useful for handling background
processes.

*/

#ifndef BGPROC_H
#define BGPROC_H

#include <unistd.h>
#include <sys/types.h>

typedef struct {
	unsigned int job_id;
	pid_t pid;
	char cmd[200];
} job;

typedef struct {
	int size;
	job** items;
} job_list;

// Initializes a job list
void init_job_list(job_list* jl);

// Adds a job to the list
void add_job(job_list* jl, unsigned int jid, pid_t pid, char* cmd);

// Removes a job from the list at index i
void remove_job(job_list* jl, int i);

// Frees all elements in the job list
void free_job_list(job_list* jl);

#endif