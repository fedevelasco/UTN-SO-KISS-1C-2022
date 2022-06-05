#include "conexion.h"

// Estructuras para recibir la informacion 

typedef struct {
		int32_t process_size;
		t_list *instructions;
	} t_instructions_list;

typedef enum
{
	INSTRUCTIONS,
	EXIT
}op_code;

typedef struct
{
	int32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code operation_code;
	t_buffer* buffer;
} t_package;



// Iniciar y operar conexion como Servidor 

int32_t iniciar_servidor(t_log* logger, char IP, char PUERTO)
{

	int32_t socket_servidor;

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
	log_trace(logger, "Conexion con Consola - Listo para escuchar a mi cliente");

	return socket_servidor;
}

int32_t esperar_cliente(int32_t socket_servidor, t_log* logger)
{

	int32_t socket_cliente = accept(socket_servidor, NULL, NULL); // Aceptamos un nuevo cliente

	log_info(logger, "Conexion con Consola - Se conecto un cliente");

	return socket_cliente;
}

int32_t recibir_operacion(int32_t socket_cliente, t_log* logger)
{
	int32_t cod_op; // Aca se almacena lo que se recive
	if(recv(socket_cliente, &cod_op, sizeof(int32_t), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		log_info(logger, "Conexion con Consola - Error al recibir el mensaje");
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int32_t* size, int32_t socket_cliente, t_log* logger)
{
	void * buffer;

	log_info(logger, "Conexion con Consola - Error al recibir el mensaje");
	recv(socket_cliente, size, sizeof(int32_t), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

t_package* recibir_package(int32_t socket_cliente)
{
	int32_t size;
	int desplazamiento = 0;
	void * buffer;
	t_package* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int32_t));
		desplazamiento+=sizeof(int32_t);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}







// Iniciar y operar conexion como Cliente

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




// Funciones para los threads

void conectar_consola(){

	t_log* logger = iniciar_logger();

	char IP = '127.0.0.1';
	char PUERTO = '8000'; //nose si esta hardcodeado

	int server_fd = iniciar_servidor(logger, IP, PUERTO);

	int cliente_fd = esperar_cliente(server_fd, logger);

	//resto de cosas que tiene que hacer kernel-consola

	terminar_logger(logger);

}

void conectar_memoria(){

	t_log* logger = iniciar_logger();

	t_config* kernel_cfg_init = iniciar_config();

	kernel_cfg_t* kernel_cfg_done = leer_config_file(kernel_cfg_init);

	int conexion = iniciar_cliente(kernel_cfg_done->IP_MEMORIA, kernel_cfg_done->PUERTO_MEMORIA, logger);

	//resto de cosas que tiene que hacer Kernel-memoria

	terminar_config(kernel_cfg_init);
	terminar_logger(logger);

}

void conectar_cpu_dispatch(){

	t_log* logger = iniciar_logger();

	t_config* kernel_cfg_init = iniciar_config();

	kernel_cfg_t* kernel_cfg_done = leer_config_file(kernel_cfg_init);

	int conexion = iniciar_cliente(kernel_cfg_done->IP_CPU, kernel_cfg_done->PUERTO_CPU_DISPATCH, logger);

	//resto de cosas que tiene que hacer kernel-cpu(dispatch)

	terminar_config(kernel_cfg_init);
	terminar_logger(logger);

}

void conectar_cpu_interrupt(){

	t_log* logger = iniciar_logger();

	t_config* kernel_cfg_init = iniciar_config();

	kernel_cfg_t* kernel_cfg_done = leer_config_file(kernel_cfg_init);

	int conexion = iniciar_cliente(kernel_cfg_done->IP_CPU, kernel_cfg_done->PUERTO_CPU_INTERRUPT, logger);

	//resto de cosas que tiene que hacer kernel-cpu(interrupt)

	terminar_config(kernel_cfg_init);
	terminar_logger(logger);

}