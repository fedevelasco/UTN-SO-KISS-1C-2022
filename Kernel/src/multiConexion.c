#include <pthread.h>

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_THREADS 2



#define IP "127.0.0.1"
#define PUERTO "4444"


// Funcion para iniciar un logger, chequea que se haya podido iniciar
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

void iniciar_conexion_servidor(){

    t_log* logger = iniciar_logger();

    int server_fd = iniciar_servidor(logger);

    log_info(logger, "Se inicio el servidor");

    int conexion_fd = esperar_cliente(server_fd, logger);


} 

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


void iniciar_conexion_cliente(){

    char* ip_server = "127.0.0.1";
    char* port_server = "4444";

    t_log* logger = iniciar_logger();

    int cliente_fd = iniciar_cliente(ip_server, port_server, logger); 


}



int main(){

    pthread_t threads[NUM_THREADS];

    while(1){

        pthread_create (&threads[0], NULL, (void*) iniciar_conexion_servidor, NULL);

        pthread_create (&threads[1], NULL, (void*) iniciar_conexion_cliente, NULL);

    }

    return EXIT_SUCCESS;


}