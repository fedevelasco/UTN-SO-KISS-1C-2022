#include "conexion.h"

// Iniciar Conexion como Servidor 

int iniciar_servidor(t_log* logger, char IP, char PUERTO)
{

	int socket_servidor;

	struct addrinfo hints, *servinfo, *p; //Ignorar errores

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	socket_servidor = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol); // Creamos el socket de escucha del servidor

	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen); // Asociamos el socket a un puerto

	listen(socket_servidor, SOMAXCONN); // Escuchamos las conexiones entrantes

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log* logger)
{

	int socket_cliente = accept(socket_servidor, NULL, NULL); // Aceptamos un nuevo cliente

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

// Iniciar Conexion como Cliente

int iniciar_cliente(char *ip, char* puerto, t_log* logger)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol); // Ahora vamos a crear el socket.

	int connection_info = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);// Ahora que tenemos el socket, vamos a conectarlo

	if(connection_info == -1){
		log_info(logger, "No se pudo conectar al servidor");
		close(socket_cliente);
		freeaddrinfo(server_info);
		return -1;
	} else {
		log_info(logger, "Connexion al servidor exitosa");
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}


