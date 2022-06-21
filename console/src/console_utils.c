#include <console_utils.h>

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
			log_error(logger, "Conexion a Kernel - getaddrinfo - No se pudo obtener la direccion correctamente de %s:%s", ip, port);
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
		log_error(logger, "Conexion a Kernel - socket - Error creando el socket para %s:%s", ip, port);
		return -1;
	}

	//Seteo socket como reusable

	int yes = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);


	// Ahora que tenemos el socket, vamos a conectarlo

	// Error conectando
	    if(connect(server_socket, server_info->ai_addr, server_info->ai_addrlen) == -1) {
	        close(server_socket);
	        freeaddrinfo(server_info);
	        log_error(logger, "Conexion a Kernel - connect - Error al conectar a %s\n", server_name);
	        return -1;
	    } else
	        log_info(logger, "Conexion a Kernel - Consola conectada en %s:%s a %s\n", ip, port, server_name);

	freeaddrinfo(server_info);
	return server_socket;
}


int32_t send_package(int32_t connection, t_package* package)
{
	int32_t bytes = package->buffer->size + sizeof(int32_t) + sizeof(op_code);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, 0) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}


char* serialize_package(t_package* package, int32_t bytes)
{
	char * tosend  = malloc(bytes);
	memset(tosend, 0, bytes);

	int32_t offset = 0;

	memcpy(tosend + offset, &package->operation_code, sizeof(int32_t));
	offset+= sizeof(int32_t);
	memcpy(tosend + offset, &package->buffer->size, sizeof(int32_t));
	offset+= sizeof(int32_t);
	memcpy(tosend + offset, package->buffer->stream, package->buffer->size);
	offset+= package->buffer->size;

	return tosend;
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


t_buffer* create_instruction_buffer(t_instructions_list* instructions_list, t_log* logger){

	for(int i=0; i<list_size(instructions_list->instructions); i++){
			log_debug(logger, "create_instruction_buffer - Instruction %s\n", ((t_instruction*) list_get(instructions_list->instructions,i))->id );
		}

	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->stream = NULL;

	buffer->size = bytes_instructions_list(instructions_list);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_instructions_list(buffer->stream, instructions_list);

	instructions_list_destroy(instructions_list);
	log_debug(logger, "create_instruction_buffer - offset: %d\n", offset);

	return buffer;
}

t_package* create_instructions_package(t_buffer* instructions_buffer){

	t_package* package = malloc(sizeof(t_package));
	int32_t buffer_total_size = instructions_buffer->size+sizeof(int32_t);

	package->operation_code = INSTRUCTIONS;
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->stream = malloc(instructions_buffer->size);
	int32_t buffer_size = instructions_buffer->size;
	package->buffer->size = buffer_size;
	memmove(package->buffer->stream, instructions_buffer->stream, instructions_buffer->size);

	free(instructions_buffer->stream);
	free(instructions_buffer);


    return package;
}

int32_t send_to_server(int32_t connection, t_package* package)
{

	if(send_package(connection, package) == -1){
		free_package(package);
		return -1;
	} else {
//		free_package(package);
	}
	return 1;
}


