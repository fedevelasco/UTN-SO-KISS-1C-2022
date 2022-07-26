#include <connection_processor.h>

int32_t process_init(operation_buffer_t* operation_buffer) {

	log_info(logger, "process_init - Iniciando proceso en memoria - Inicio");

	t_process* process = create_process();
	deserialize_process(process, operation_buffer->buffer);

	log_info(logger, "process_init - Creando estructuras para pid: %d", process->pid);

	int32_t pid_first_level_table_number = create_pid_memory(process);
	if(pid_first_level_table_number == -1){
		log_error(logger, "Error creando proceso en memoria. Ya existe");
		free(process);
		return 1;
	}

	log_info(logger, "process_init - Creando archivo swap para pid: %d", process->pid);

	if(swap_create(process->pid, process->process_size)){
		log_error(logger, "process_init - Error creando swap");
		free(process);
		return 1;
	}

	log_info(logger, "process_init - Enviando numero de tabla del proceso a Kernel: %d", pid_first_level_table_number);


	if(send_process_init(operation_buffer->client_socket, pid_first_level_table_number)) {
		log_error(logger, "Error enviando numero de tabla del proceso a Kernel");
		free(process);
		return 1;
	}

	log_debug(logger, "process_init - se envio el paquete");

	free(process);
	free(operation_buffer->buffer);
	free(operation_buffer);

	log_info(logger, "process_init - Iniciando proceso en memoria - Fin");

	return 0;
}

int32_t process_suspend(operation_buffer_t* operation_buffer){

	log_info(logger, "process_suspend - Suspendiendo proceso - Inicio");

	t_process_suspend* process = create_process_suspend();
	deserialize_process_suspend(process, operation_buffer->buffer);

	log_info(logger, "process_suspend - Se suspende el proceso: %d", process->pid);

	if(suspend_pid(process)){
		log_error(logger, "Error suspendiendo proceso");
		free(process);
		return 1;
	}

	free_memory(process->first_level_table_number);


	if(send_process_suspend(operation_buffer->client_socket)) {
			log_error(logger, "Error enviando confirmacion de suspension de proceso a Kernel");
			free(process);
			return 1;
		}


	free(process);
	free(operation_buffer->buffer);
	free(operation_buffer);

	log_info(logger, "process_suspend - Suspendiendo proceso - Fin");

	return 0;
}

