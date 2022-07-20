#include <connection_processor.h>

bool process_init(int32_t client_socket) {

	t_process* process = malloc(sizeof(t_process));
	process = recv_process(client_socket);

	//TODO: Revisar por que falla el envio.
//	if (send_ack(client_socket, true)) {
//		log_error(logger, "Error enviando senial de ack a Kernel");
//		free(process);
//		return false;
//	}

	pthread_mutex_lock(&LOCK_ACCESS_RAM);

	bool ret_code = create_process_in_memory(process);
	int32_t pid_first_level_table_number = get_pid_first_level_table_number(
			process->pid);

	pthread_mutex_unlock(&LOCK_ACCESS_RAM);

	if (!ret_code) {
		log_error(logger, "Error iniciando proceso en Memoria");
		free(process);
		return false;
	}

	log_info(logger, "Enviando a Kernel numero de tabla: %d del pid %d", pid_first_level_table_number, process->pid );

	if (send_process_init(client_socket, pid_first_level_table_number)) {
		log_error(logger,
				"Error enviando numero de tabla del proceso a Kernel");
		free(process);
		return false;
	}
	return true;
}

