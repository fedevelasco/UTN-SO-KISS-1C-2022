#include <connection_processor.h>

void process_init(client_socket) {

	t_process* process = recv_process(client_socket);

	send_ack(client_socket, true);

	pthread_mutex_lock(&LOCK_ACCESS_RAM);

	bool ret_code = create_process_in_memory(process);

	pthread_mutex_unlock(&LOCK_ACCESS_RAM);

	if (!ret_code) {
		log_error(logger, "Error iniciando proceso en Memoria");
		list_destroy_and_destroy_elements(posiciones, (void*) free);
		free(tareas);
		break;
	}

	//TODO: free del pcb

}

