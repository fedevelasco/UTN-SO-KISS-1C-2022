#include <connection_processor.h>

int32_t process_init(operation_buffer_t* operation_buffer) {

	log_info(logger, "process_init - Iniciando proceso en memoria");

	t_process* process = create_process();
	deserialize_process(process, operation_buffer->buffer);

	log_info(logger, "process_init - Creando estructuras para pid: %d", process->pid);

	int32_t pid_first_level_table_number = create_pid_memory(process);

	log_info(logger, "process_init - Creando archivo swap para pid: %d", process->pid);

	if(create_swap(process->pid, process->process_size)){
		log_error(logger, "process_init - Error creando swap");
		free(process);
		return 1;
	}

	log_info(logger, "process_init - Enviando numero de tabla del proceso a Kernel");

	if(send_process_init(operation_buffer->client_socket, pid_first_level_table_number)) {
		log_error(logger, "Error enviando numero de tabla del proceso a Kernel");
		free(process);
		return 1;
	}

	return 0;
}

