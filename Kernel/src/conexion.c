#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

// BORRAR
t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("kernel.log","Kernel", 1, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL){
		printf("Error al abrir el kernel.log\n");
		exit(1);
	}

	return nuevo_logger;
}

// Funcion que finaliza el logger
void terminar_logger(t_log* logger)
{
	log_destroy(logger);
}
//BORRAR


int iniciar_servidor(t_log* logger)
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

int main(){
	
	t_log* logger = iniciar_logger();

	log_info(logger, "Logger iniciado");

	int init_kernel = iniciar_servidor(logger);

	log_info(logger, "Servidor listo para recibir a un cliente");

	int	conexion_kernel = esperar_cliente(init_kernel, logger);

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

}