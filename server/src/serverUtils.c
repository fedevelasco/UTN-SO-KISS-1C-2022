#include <serverUtils.h>

uint32_t iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto)
{
	uint32_t socket_servidor; //Declaramos el descriptor

	struct addrinfo infoSocket, *server_info; //Declaramos las estructuras

	memset(&infoSocket, 0, sizeof(infoSocket)); //Seteamos el valor de la informacion de la direccion a 0
	//Asignamos los valores de info de la conexion
	infoSocket.ai_family = AF_UNSPEC; //Indica a getaddrinfo(3) que la direccion va a ser de tipo IPv4 o IPv6
	infoSocket.ai_socktype = SOCK_STREAM; //Socket de tipo TCP/IP
	infoSocket.ai_flags = AI_PASSIVE; //Solo para cuando se quiere utilizar el socket para un servidor

	//Obtenemos la direccion y los datos del socket y los mete en server_info
	if (getaddrinfo(ip, puerto, &infoSocket, &server_info) != 0){
		perror("No se pudo obtener la direccion correctamente.");
		return -1;
	}

	bool conecto = false;

	// Itera por cada addrinfo devuelto
	for (struct addrinfo *p = server_info; p != NULL; p = p->ai_next) {
		// Creamos el socket de escucha del servidor
		socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (socket_servidor == -1) // fallo de crear socket
			continue;

		int yes = 1;
		setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

		// Asociamos el socket a un puerto
		if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
			// Si entra aca fallo el bind
			close(socket_servidor);
			continue;
		}
		// Ni bien conecta uno nos vamos del for
		conecto = true;
		break;
	}

	if(!conecto) {
		free(server_info);
		perror("No se pudo crear el socket");
		return -1;
	}

	// Escuchamos las conexiones entrantes

	listen(socket_servidor, SOMAXCONN);

	log_trace(logger, "Listo para escuchar a mi cliente");
	log_info(logger, "Escuchando en %s:%s (%s)\n", IP, PUERTO, name);

	freeaddrinfo(server_info);

	return socket_servidor;
}

uint32_t esperar_cliente(t_log* logger, const char* name, uint32_t socket_servidor)
{
	// Aceptamos un nuevo cliente
	struct sockaddr_in dir_cliente; // Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc
	socklen_t addrlenght  = sizeof(dir_cliente);

	uint32_t socket_cliente = accept(socket_servidor, (struct sockaddr *) &dir_cliente, &addrlenght );

	log_info(logger, "Cliente conectado (a %s)\n", name);

	return socket_cliente;
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

char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente)
{
	char* buffer;

	read(socket_cliente, buffer_size, sizeof(uint32_t));
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size);

	return buffer;
}

void recibir_mensaje(uint32_t socket_cliente)
{
	uint32_t size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_instructions_list* recibir_paquete(uint32_t socket_cliente, t_log* logger)
{
	uint32_t buffer_size;
	char* buffer;

	buffer = recibir_buffer(&buffer_size, socket_cliente);
	log_info(logger, "buffer_size:%i\n", buffer_size);

	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size);


	deserialize_instructions_list(instructions_list, buffer);

	return instructions_list;
}




