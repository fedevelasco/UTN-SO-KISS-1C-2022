#include "consoleUtils.h"



void* serialize_package(t_package* package, int32_t bytes)
{
	void * to_send = malloc(bytes);
	int32_t offset = 0;

	memcpy(to_send + offset, &(package->operation_code), sizeof(int32_t));
	offset+= sizeof(int32_t);
	memcpy(to_send + offset, &(package->buffer->size), sizeof(int32_t));
	offset+= sizeof(int32_t);
	memcpy(to_send + offset, package->buffer->stream, package->buffer->size);
	offset+= package->buffer->size;

	return to_send;
}

int32_t create_connection(t_log* logger, const char* server_name, char *ip, char* port)
{
	struct addrinfo infoSocket, *server_info; //Declaramos las estructuras

	memset(&infoSocket, 0, sizeof(infoSocket)); //Seteamos el valor de la informacion de la direccion a 0
	//Asignamos los valores de info de la conexion
	infoSocket.ai_family = AF_UNSPEC;//Indica a getaddrinfo(3) que la direccion va a ser de tipo IPv4 o IPv6
	infoSocket.ai_socktype = SOCK_STREAM; //Socket de tipo TCP/IP
	infoSocket.ai_flags = AI_PASSIVE; //Solo para cuando se quiere utilizar el socket para un servidor

	//Obtenemos la direccion y los datos del socket y los mete en server_info
		if (getaddrinfo(ip, port, &infoSocket, &server_info) != 0){
			perror("No se pudo obtener la direccion correctamente.");
			return -1;
		}

	// Ahora vamos a crear el socket.
	int32_t server_socket = 0;
	server_socket = socket(server_info->ai_family,
		                    server_info->ai_socktype,
		                    server_info->ai_protocol);

	// Fallo en crear el socket
	if(server_socket == -1) {
		freeaddrinfo(server_info);
		log_error(logger, "Error creando el socket para %s:%s", ip, port);
		return -1;
	}

	// Ahora que tenemos el socket, vamos a conectarlo

	// Error conectando
	    if(connect(server_socket, server_info->ai_addr, server_info->ai_addrlen) == -1) {
	        close(server_socket);
	        freeaddrinfo(server_info);
	        log_error(logger, "Error al conectar (a %s)\n", server_name);
	        return -1;
	    } else
	        log_info(logger, "Cliente conectado en %s:%s (a %s)\n", ip, port, server_name);

	freeaddrinfo(server_info);
	return server_socket;
}


void create_buffer(t_package* package, t_instructions_list* instructions_list)
{
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->size = 0;
	package->buffer->stream = NULL;
}


t_package* create_package(t_instructions_list* instructions_list)
{
	t_package* package = malloc(sizeof(t_package));
	package->operation_code = INSTRUCTIONS;
	create_buffer(package, instructions_list);
	return package;
}


int send_package(int32_t connection, t_package* package)
{
	int32_t bytes = package->buffer->size + 2*sizeof(int32_t);
	void* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, 0) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}

void free_package(t_package* package)
{
	free(package->buffer->stream);
	free(package->buffer);
	free(package);
}

void end_connection(int32_t connection)
{
	close(connection);
}

int32_t receive_operation_code(int32_t server_socket)
{
	int32_t cod_op = recv(server_socket, &cod_op, sizeof(int32_t), MSG_WAITALL);
	return cod_op;
}


t_buffer* create_instruction_buffer(t_instructions_list* instructions_list){

	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->stream = NULL;

	buffer->size = sizeof(instructions_list);

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &instructions_list->process_size, sizeof(int32_t));
	offset += sizeof(int32_t);

	t_list_iterator* list_iterator = list_iterator_create(instructions_list->instructions);

	while(list_iterator_has_next(list_iterator)){
		t_instruction* instruction = list_iterator_next(list_iterator);

		memcpy(stream + offset, &instruction->id_length, sizeof(int32_t));
		offset += sizeof(int32_t);

		memcpy(stream + offset, &instruction->id, strlen(instruction->id) + 1);
		offset += strlen(instruction->id) + 1;

		memcpy(stream + offset, &instruction->parameters, sizeof(instruction->parameters));
		offset += sizeof(instruction->parameters);

		memcpy(stream + offset, &instruction->cantParameters, sizeof(int32_t));

		buffer->stream = stream;
	}

	list_iterator_destroy(list_iterator);

	return buffer;
}


t_package* create_instructions_package(t_buffer* instructions_buffer){

	t_package* package = malloc(sizeof(t_package));

	package->operation_code = INSTRUCTIONS;
	package->buffer = instructions_buffer;

    return package;
}


