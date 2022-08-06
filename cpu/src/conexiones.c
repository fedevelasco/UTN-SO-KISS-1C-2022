#include <conexiones.h>

t_memory_config* obtenerTraduccionDeDirecciones(){

	 int socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA);

	 t_buffer* buffer = new_memory_config_buffer();

	t_package* package = new_package(buffer, GET_MEMORY_CONFIG_REQUEST);

	if (send_to_server(socket_memoria, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");

	}

	uint32_t cod_op = recibir_operacion(socket_memoria);
	if(cod_op != GET_MEMORY_CONFIG_RESPONSE){
		perror("respuesta inesperada");
		exit(EXIT_FAILURE);
	}

	char* buffer_recibido = recibir_paquete(socket_memoria);

	t_memory_config* traduccion_direcciones = create_memory_config();

	deserialize_memory_config(traduccion_direcciones, buffer_recibido);

	free(buffer_recibido);
	close(socket_memoria);

    return traduccion_direcciones;
}

char* recibir_paquete(uint32_t socket_cliente)
{
	uint32_t buffer_size;
	char* buffer;

	buffer = recibir_buffer(&buffer_size, socket_cliente);
	
	return buffer;
}

char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente)
{
	char* buffer;

	read(socket_cliente, buffer_size, sizeof(uint32_t));
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size);

	return buffer;
}

uint32_t recibir_operacion(uint32_t socket_cliente)
{
	uint32_t cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
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

t_buffer* new_page_table_request_buffer(t_page_table_request* request){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_page_table_request(buffer->stream, request);

	log_debug(logger, "new_page_table_request_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_memoria_read_buffer(uint32_t direccion){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &direccion);

	log_debug(logger, "new_memoria_read_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_peticion_buffer(t_memory_write_request* peticion_escritura){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_memory_write_request(buffer->stream, peticion_escritura);

	log_debug(logger, "new_peticion_buffer - size: %d\n", offset);

	return buffer;
}

t_buffer* new_memory_config_buffer(){

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(uint32_t);

	uint32_t get_memory_config_signal = 1;

	buffer->stream = malloc(buffer->size);
	int offset = serialize_int(buffer->stream, &get_memory_config_signal);

	log_debug(logger, "new_memoria_config_buffer - size: %d\n", offset);

	return buffer;
}





// handshake con consola y traduccion de direcciones