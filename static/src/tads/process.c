#include <tads/process.h>


t_process* create_process(){
	t_process* process = malloc(sizeof(t_process));
	process->pid = -1;
	process->process_size = -1;
	process->first_level_table_number = -1;

	return process;
}

void process_destroy(t_process* process){
	free(process);
}





