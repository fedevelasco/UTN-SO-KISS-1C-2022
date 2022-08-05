// Este file es exclusivamente utilizado para tener como libreria de conexion entre modulos

#include <libreriaConexiones.h>


// -------------- Iniciar Servidor --------------

int32_t iniciar_servidor(t_log* logger, char* name, char* ip, char* puerto)
{
	int32_t socket_servidor; //Declaramos el descriptor

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
	log_info(logger, "Escuchando en %s:%s (%s)\n", ip, puerto, name);

	freeaddrinfo(server_info);

	return socket_servidor;
}


// -------------- Aceptar Cliente --------------

int32_t esperar_cliente(t_log* logger, char* name, int32_t socket_servidor)
{
	// Aceptamos un nuevo cliente
	struct sockaddr_in dir_cliente; // Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc
	socklen_t addrlenght  = sizeof(dir_cliente);


	int32_t socket_cliente = accept(socket_servidor, (struct sockaddr *) &dir_cliente, &addrlenght ); // Aceptamos un nuevo cliente

	if(socket_cliente == -1){
			perror("Error al aceptar socket_cliente");
			exit(EXIT_FAILURE);
		}

	log_info(logger, "socket: %i", socket_cliente);

	log_info(logger, "Se conecto %s:%d al servidor %s\n", inet_ntoa(dir_cliente.sin_addr), (int) ntohs(dir_cliente.sin_port), name);

	return socket_cliente;
}

// -------------- Iniciar Cliente --------------

//int iniciar_cliente(char *ip, char* puerto, t_log* logger){ //viejo
int iniciar_cliente(char *ip, char* puerto){
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
		perror("No se pudo conectar al servidor");
		close(socket_cliente);
		freeaddrinfo(server_info);
		return -1;
	} else {
//		perror("Connexion al servidor exitosa");
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

// -------------- Liberar Conexiones --------------

void liberar_conexion(int socket_cliente) {
    close(socket_cliente);
    socket_cliente = -1;
}
