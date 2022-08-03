#include <tads/process.h>


t_process* create_process(){
	t_process* process = malloc(sizeof(t_process));
	process->pid = 0;
	process->process_size = 0;
	process->first_level_table_number = 0;

	return process;
}

void process_destroy(t_process* process){
	free(process);
}





