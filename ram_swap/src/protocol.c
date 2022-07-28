#include <protocol.h>


char* recv_buffer(int32_t client_socket)
{
	char* buffer;
	int32_t buffer_size;
	recv(client_socket, &buffer_size, sizeof(int32_t), 0);
	buffer = malloc(buffer_size);
	recv(client_socket, buffer, buffer_size, 0);

	return buffer;
}


int32_t send_process_init(int32_t client_socket, int32_t pid_first_level_table_number){

	t_buffer* buffer = new_process_init_buffer(pid_first_level_table_number);

	t_package* package = new_package(buffer, PROCESS_INIT_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_process_init - se envio el paquete a servidor");
	return 0;
}

int32_t send_process_suspend(int32_t client_socket){

	t_buffer* buffer = new_process_suspend_buffer();

	t_package* package = new_package(buffer, PROCESS_SUSPEND_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_process_suspend - se envio el paquete a servidor");
	return 0;
}

int32_t send_memory_config(int32_t client_socket, t_memory_config* memory_config){

	t_buffer* buffer = new_memory_config_buffer(memory_config);

	t_package* package = new_package(buffer, GET_MEMORY_CONFIG_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_memory_config - se envio el paquete a servidor");
	return 0;
}


int32_t send_to_server(int32_t connection, t_package* package)
{
	if(send_package(connection, package) == -1){
		package_destroy(package);
		return -1;
	}
	package_destroy(package);

	return 1;
}


int32_t send_package(int32_t connection, t_package* package)
{
	int32_t bytes = package->buffer->size + 2*sizeof(int32_t);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, MSG_CONFIRM ) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}

t_buffer* new_process_init_buffer(int32_t pid_first_level_table_number){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(int32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &pid_first_level_table_number);

	log_debug(logger, "new_process_init_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_process_suspend_buffer(){

	int32_t ok_signal = 1;

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(int32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &ok_signal);

	log_debug(logger, "new_process_suspend_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_memory_config_buffer(t_memory_config* memory_config){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(int32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_memory_config(buffer->stream, memory_config);

	log_debug(logger, "new_memory_config_buffer - size: %d\n", offset);

	return buffer;
}


