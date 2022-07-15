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


void end_connection(int32_t connection)
{
	close(connection);
}

int32_t receive_operation_code(int32_t server_socket)
{
	int32_t cod_op = recv(server_socket, &cod_op, sizeof(int32_t), MSG_WAITALL);
	return cod_op;
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
	int32_t bytes = package->buffer->size + sizeof(int32_t) + sizeof(op_code);
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

t_config* load_configuration_file(t_log* logger)
{
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Inicio");
	t_config* new_configuration;

	new_configuration = config_create("../console/console.config");

	if (new_configuration == NULL){
		log_error(logger, "Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion");
		exit(2);
	}
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Config en %s cargada ok", new_configuration->path);
	return new_configuration;
}


void end_process(int32_t connection, t_log* logger, t_config* config){

	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}

    close(connection);
}


