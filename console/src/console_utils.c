#include <console_utils.h>

uint32_t create_connection(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = IPPROTO_TCP;

	getaddrinfo(ip, puerto, &hints, &server_info);

	uint32_t socket_cliente = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);

	if(socket_cliente == -1){
		perror("error de creacion de socket");
		exit(EXIT_FAILURE);
	}

	int resultado_conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	if(resultado_conexion != 0){
		perror("error de conexión");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}


void end_connection(uint32_t connection)
{
	close(connection);
}

uint32_t receive_operation_code(uint32_t server_socket)
{
	uint32_t cod_op;
	if(recv(server_socket, &cod_op, sizeof(uint32_t), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(server_socket);
		return -1;
	}
}

t_paquete* wait_package(int server_socket){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	recv(server_socket, &(paquete->codigo_operacion), sizeof(t_op_code), 0);

	recv(server_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(server_socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
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
	uint32_t bytes = package->buffer->size + sizeof(uint32_t) + sizeof(t_op_code);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, 0) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}

t_log* start_logger(void)
{
	t_log* new_logger;

	new_logger = log_create("console.log", "Console", 1, LOG_LEVEL_DEBUG);

	if(new_logger == NULL){
		printf("No fue posible crear el Logger\n");
		exit(1);
	}

	return new_logger;
}

t_config* load_configuration_file(char* ips_path, t_log* logger)
{
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Inicio");
	t_config* new_configuration;

	new_configuration = config_create(ips_path);

	if (new_configuration == NULL){
		log_error(logger, "Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion");
		exit(2);
	}
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Config en %s cargada ok", new_configuration->path);
	return new_configuration;
}


void end_process(uint32_t connection, t_log* logger, t_config* config){

	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}

    close(connection);
}

char* recibir_buffer(uint32_t socket_cliente)
{
	uint32_t buffer_size;
	char* buffer;
	if(recv(socket_cliente, &buffer_size, sizeof(uint32_t), MSG_WAITALL) == -1){
		perror("error scoket");
	}

	buffer = malloc(buffer_size);
	if(recv(socket_cliente, buffer, buffer_size, MSG_WAITALL) == -1){
			perror("error scoket");
		}


	return buffer;
}

