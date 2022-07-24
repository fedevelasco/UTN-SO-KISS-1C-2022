#include <connection_utils.h>

int server_listen_ram(char* server_name, int server_socket) {



	while(1){

	int32_t client_socket = esperar_cliente(logger, server_name, server_socket);

	if (client_socket != -1) {

		op_code opcode;
		if (recv(client_socket, &opcode, sizeof(op_code), 0) != sizeof(op_code)) {
			log_error(logger, "server_listen_ram - Error recibiendo op_code");
			return 1;
		}

		int32_t buffer_size;
		char* buffer;

		buffer = recv_buffer(&buffer_size, client_socket);

		operation_buffer_t* operation_buffer = malloc(sizeof(operation_buffer_t));
		operation_buffer->opcode = opcode;
		operation_buffer->buffer = buffer;
		operation_buffer->client_socket = client_socket;

		if(kernel_opcode(opcode)){
			log_info(logger, "El opcode es de Kernel");
			pthread_mutex_lock(&MUTEX_KERNEL_QUEUE);
			queue_push(kernel_queue, operation_buffer);
			pthread_mutex_unlock(&MUTEX_KERNEL_QUEUE);

			if(sem_post(&sem_kernel_thread)){
				log_error(logger, "server_listen_ram - Error en sem_post - sem_kernel_thread. (errno %i)", errno);
			}

		} else {
			log_info(logger, "El opcode es de cpu");
			pthread_mutex_lock(&MUTEX_CPU_QUEUE);
			queue_push(cpu_queue, operation_buffer);
			pthread_mutex_unlock(&MUTEX_CPU_QUEUE);
			if(sem_post(&sem_cpu_thread)){
				log_error(logger, "server_listen_ram - Error en sem_post - sem_cpu_thread. (errno %i)", errno);
			}
		}

		}

	}
	return 0;
}

int32_t kernel_opcode(op_code opcode){

	if(
		opcode == PROCESS_INIT_REQUEST ||
		opcode == PROCESS_INIT_SEND ||
		opcode == PROCESS_SUSPEND_REQUEST ||
		opcode == PROCESS_SUSPEND_SEND ||
		opcode == PROCESS_KILL_REQUEST ||
		opcode == PROCESS_KILL_SEND
		){
		log_info(logger, "kernel_opcode - El opcode es de Kernel");
		return 1;
	}

	return 0;
}

void process_kernel_functions(){
	while(true){
		 	log_info(logger, "KERNEL_THREAD - process_kernel_functions - Procesando funcion de kernel");

	        if(sem_wait(&sem_kernel_thread)){
				log_error(logger, "KERNEL_THREAD - process_kernel_functions - Error en sem_wait - sem_kernel_thread. (errno %i)", errno);
			}
		 	log_info(logger, "KERNEL_THREAD - process_kernel_functions - Procesando funcion de kernel despues de semaforo");

	        pthread_mutex_lock(&MUTEX_KERNEL_QUEUE);
	        operation_buffer_t* operation_buffer = queue_pop(kernel_queue);
	        pthread_mutex_unlock(&MUTEX_KERNEL_QUEUE);

		 	log_info(logger, "KERNEL_THREAD - process_kernel_functions - se saco de queue socket: %d", operation_buffer->client_socket);


	        switch (operation_buffer->opcode) {
	        		case DEBUGGING: {
	        			log_info(logger, "Im a debug message");
	        			break;
	        		}
	        		case PROCESS_INIT_REQUEST: {
	        			process_init(operation_buffer);
	        			break;
	        		}

	        		case PROCESS_SUSPEND_REQUEST: {
	        //			process_suspend(operation_buffer->buffer);
	        			break;
	        		}

	        		case PROCESS_KILL_REQUEST: {
	        //			process_kill(operation_buffer->buffer);
	        			break;
	        		}

	        		default:
	        			log_error(logger, "KERNEL_THREAD - process_kernel_functions - Error en server");
	        			return;
	        		}

	        free(operation_buffer->buffer);
	        free(operation_buffer);
	    }

}

void process_cpu_functions(){
	while(true){
		        sem_wait(&sem_cpu_thread);
		        pthread_mutex_lock(&MUTEX_CPU_QUEUE);
		        operation_buffer_t* operation_buffer = queue_pop(cpu_queue);
		        pthread_mutex_unlock(&MUTEX_KERNEL_QUEUE);

		        switch (operation_buffer->opcode) {
		        		case DEBUGGING: {
		        			log_info(logger, "Im a debug message");
		        			break;
		        		}
		        		case GET_SECOND_LEVEL_TABLE_REQUEST: {
		        //			get_second_level_table(operation_buffer->buffer);
		        			break;
		        		}
		        		case GET_FRAME_REQUEST: {
		        //			get_frame(operation_buffer->buffer);
		        			break;
		        		}
		        		case READ_MEMORY_REQUEST: {
		        //			read_memory(operation_buffer->buffer);
		        			break;
		        		}
		        		case WRITE_MEMORY_REQUEST: {
		        //			write_memory(operation_buffer->buffer);
		        			break;
		        		}

		        		default:
		        			log_error(logger, "process_cpu_functions - Error en server");
		        			return;
		        		}

		        free(operation_buffer->buffer);
		        free(operation_buffer);
		    }

}

