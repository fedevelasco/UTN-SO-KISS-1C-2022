#include <protocol.h>


char* recv_buffer(int32_t client_socket)
{
	char* buffer;
	uint32_t buffer_size;
	recv(client_socket, &buffer_size, sizeof(uint32_t), 0);
	buffer = malloc(buffer_size);
	recv(client_socket, buffer, buffer_size, 0);

	return buffer;
}


uint32_t send_process_init(uint32_t client_socket, uint32_t pid_first_level_table_number){

	t_buffer* buffer = new_process_init_buffer(pid_first_level_table_number);

	t_package* package = new_package(buffer, PROCESS_INIT_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_process_init - se envio el paquete a servidor");
	return 0;
}

uint32_t send_process_suspend(uint32_t client_socket){

	t_buffer* buffer = new_process_suspend_buffer();

	t_package* package = new_package(buffer, PROCESS_SUSPEND_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_process_suspend - se envio el paquete a servidor");
	return 0;
}

uint32_t send_memory_config(uint32_t client_socket, t_memory_config* memory_config){

	t_buffer* buffer = new_memory_config_buffer(memory_config);

	t_package* package = new_package(buffer, GET_MEMORY_CONFIG_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_memory_config - se envio el paquete a servidor");
	return 0;
}

uint32_t send_second_level_table_id(uint32_t client_socket, uint32_t second_level_table_id){

	t_buffer* buffer = new_second_level_table_buffer(second_level_table_id);

	t_package* package = new_package(buffer, GET_SECOND_LEVEL_TABLE_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_second_level_table_id - se envio el paquete a servidor");
	return 0;
}

uint32_t send_frame_number_read(uint32_t client_socket, uint32_t frame_number){

	t_buffer* buffer = new_frame_number_buffer(frame_number);

	t_package* package = new_package(buffer, GET_FRAME_READ_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_frame_number - se envio el paquete a servidor");
	return 0;
}

uint32_t send_frame_number_write(uint32_t client_socket, uint32_t frame_number){

	t_buffer* buffer = new_frame_number_buffer(frame_number);

	t_package* package = new_package(buffer, GET_FRAME_WRITE_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_frame_number - se envio el paquete a servidor");
	return 0;
}


uint32_t send_memory_data(uint32_t client_socket, uint32_t memory_data){

	t_buffer* buffer = new_memory_data_buffer(memory_data);

	t_package* package = new_package(buffer, READ_MEMORY_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_memory_data - se envio el paquete a servidor");
	return 0;
}

uint32_t send_write_memory(uint32_t client_socket){

	t_buffer* buffer = new_write_memory_buffer();

	t_package* package = new_package(buffer, WRITE_MEMORY_RESPONSE);

	if (send_to_server(client_socket, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

	log_debug(logger, "send_write_memory - se envio el paquete a servidor");
	return 0;
}


uint32_t send_to_server(uint32_t connection, t_package* package)
{
	if(send_package(connection, package) == -1){
		package_destroy(package);
		return -1;
	}
	package_destroy(package);

	return 1;
}


uint32_t send_package(uint32_t connection, t_package* package)
{
	uint32_t bytes = package->buffer->size + 2*sizeof(uint32_t);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, MSG_CONFIRM ) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}

t_buffer* new_process_init_buffer(uint32_t pid_first_level_table_number){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &pid_first_level_table_number);

	log_debug(logger, "new_process_init_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_second_level_table_buffer(uint32_t second_level_table_id){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &second_level_table_id);

	log_debug(logger, "new_second_level_table_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_frame_number_buffer(uint32_t frame_number){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &frame_number);

	log_debug(logger, "new_frame_number_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_memory_data_buffer(uint32_t memory_data){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &memory_data);

	log_debug(logger, "new_memory_data_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_process_suspend_buffer(){

	uint32_t ok_signal = 1;

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &ok_signal);

	log_debug(logger, "new_process_suspend_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_write_memory_buffer(){

	uint32_t ok_signal = 1;

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &ok_signal);

	log_debug(logger, "new_write_memory_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_memory_config_buffer(t_memory_config* memory_config){

	t_buffer* buffer = create_buffer();

	buffer->size = 2*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_memory_config(buffer->stream, memory_config);

	log_debug(logger, "new_memory_config_buffer - size: %d\n", offset);

	return buffer;
}


