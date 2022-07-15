#include <connection_utils.h>

static void process_connection(void* void_args) {
	connection_args_t* args = (connection_args_t*) void_args;
	int client_socket = args->fd;
	char* server_name = args->server_name;
	free(args);

	op_code cop;
	while (client_socket != -1) {

		if (recv(client_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
			log_info(logger, "process_connection - Error recibiendo conexion");
			return;
		}

		switch (cop) {
            case DEBUGGING:
                log_info(logger, "Im a debug message");
                break;

		case PROCESS_INIT: {
			process_init(client_socket);
			break;
		}

		case PROCESS_SUSPEND: {
			process_suspend(client_socket);
			break;
		}

		case PROCESS_KILL: {
			process_kill(client_socket);
			break;
		}
		case GET_SECOND_LEVEL_TABLE: {
			get_second_level_table(client_socket);
			break;
		}
		case GET_FRAME: {
			get_frame(client_socket);
			break;
		}
		case READ_MEMORY: {
			read_memory(client_socket);
			break;
		}
		case WRITE_MEMORY: {
			write_memory(client_socket);
			break;
		}

			// Errors
		case -1:
			log_error(logger,
					"process_connection - Cliente desconectado de RAM.");
			return;
		default:
			log_error(logger, "process_connection - Error en server");
			return;
		}
	}

	log_warning(logger, "El cliente se desconecto de %s server", server_name);
	return;
}

int server_listen_ram(char* server_name, int server_fd, t_log* logger) {
	int client_socket = esperar_cliente(logger, server_name, server_fd);

	if (client_socket != -1) {
		pthread_t thread;
		connection_args_t* args = malloc(sizeof(connection_args_t));
		args->fd = client_socket;
		args->server_name = server_name;
		thread_create_with_parameter(&thread, NULL, (void*) process_connection,
				(void*) args);
//        pthread_detach(thread); Probar, ya creo el thread como detachable. Cual uso?
		return 1;
	}
	return 0;
}

