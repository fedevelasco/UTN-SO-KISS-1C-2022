#include <tads/memory_write_request.h>


t_memory_write_request* create_memory_write_request(){
	t_memory_write_request* memory_write_request = malloc(sizeof(t_memory_write_request));
	memory_write_request->fisical_address=-1;
	memory_write_request->data = 0;
	memory_write_request->pid = -1;

	return memory_write_request;
}

void memory_write_request_destroy(t_memory_write_request* memory_write_request){
	free(memory_write_request);
}





