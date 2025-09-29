#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bgproc.h"

void init_job_list(job_list* jl) {
	jl->size = 0;
	jl->items = NULL;
}

void add_job(job_list* jl, unsigned int jid, pid_t pid, char* cmd) {
	jl->items = (job**) realloc(jl->items, sizeof(job*) * (jl->size + 1));

	job* new = (job*) malloc(sizeof(job));
	new->job_id = jid;
	new->pid = pid;
	//printf("%s\n", cmd);
	strncpy(new->cmd, cmd, 200);

	jl->items[jl->size] = new;
	jl->size++;
}

void remove_job(job_list* jl, int i) {
	if (i < 0 || i >= jl->size)
		return;

	free(jl->items[i]);
	
	for (int j = i; j < jl->size - 1; j++)
		jl->items[j] = jl->items[j + 1];

	jl->size--;
}

void free_job_list(job_list* jl) {
	while (jl->size > 0)
		remove_job(jl, 0);
}