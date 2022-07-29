#include <socket.h>

/* Inicializo el sevidor en modo escucha */
int iniciar_servidor(char* IP, char* PORT)
{
	int socket_servidor;
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = IPPROTO_TCP;
	
	getaddrinfo(IP, PORT, &hints, &server_info);

	socket_servidor = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	
	if(socket_servidor == -1){
		perror("Kernel - Error al crear socket");
		exit(EXIT_FAILURE);
	}
	
	int binded = bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	
	if(binded == -1){
		perror("Kernel - Error al bindear el socket");
		exit(EXIT_FAILURE);
	}
	
	int listening = listen(socket_servidor, SOMAXCONN);
	
	if(listening == -1){
		perror("Kernel - error al escuchar conexion");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);
	
	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	if(socket_cliente == -1){
		perror("error al aceptar socket_cliente");
		exit(EXIT_FAILURE);
	}
	return socket_cliente;
}

t_paquete* recibirPaquete(int server_socket){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	recv(server_socket, &(paquete->codigo_operacion), sizeof(t_op_code), 0);

	recv(server_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(server_socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
}


//int crear_conexion(char* ip, char* puerto) {
//	struct addrinfo hints;
//	struct addrinfo *server_info;
//
//	memset(&hints, 0, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = IPPROTO_TCP;
//
//	getaddrinfo(ip, puerto, &hints, &server_info);
//
//	int socket_cliente = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
//
//	if(socket_cliente == -1){
//		perror("error de creacion de socket");
//		exit(EXIT_FAILURE);
//	}
//
//	int resultado_conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
//
//	if(resultado_conexion != 0){
//		perror("error de conexión");
//		exit(EXIT_FAILURE);
//	}
//
//	freeaddrinfo(server_info);
//
//	return socket_cliente;
//}


uint32_t recibir_operacion(uint32_t socket_cliente)
{
	uint32_t cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL) > 0) //Recibo la cantidad de bytes que representan al op_code en el stream de datos
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

	read(socket_cliente, buffer_size, sizeof(uint32_t)); //Leo el tamanio del payload y lo escribo en buffer_size
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size); //Ahora leo la cantidad de payload que me indica buffer_size y o guardo en buffer que devuelvo

	return buffer;
}

t_instructions_list* recibir_paquete(uint32_t socket_cliente, t_log* logger)
{
	uint32_t buffer_size; //Creo el tamanio del buffer
	char* buffer; //Creo el buffer

	buffer = recibir_buffer(&buffer_size, socket_cliente); //leo el payload y lo asigno a buffer
	log_info(logger, "Kernel - Buffer recibido de parte de consola: %i\n", buffer_size); 

	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones

	return instructions_list;
}

t_instructions_list* recibirPaqquete_inicio(int server_socket, t_proceso* nuevo_proceso){

    uint32_t cod_op = recibir_operacion(server_socket);// Recibo el cod_op
    t_instructions_list* instructions_list; 
        switch (cod_op) {

			case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

				instructions_list = recibir_paquete(server_socket, logger); //Recibo paquete, lo deserializo y lo guardo en instruction_list
				log_info(logger, "Kernel - Valores recibidos:");
				log_info(logger, "Kernel - Process size: %d", instructions_list->process_size);
                return(instructions_list);
				break;

			case -1:

				log_error(logger, "El cliente se desconecto. Terminando conexion con el cliente."); //Codigo para finalizar conexion
				return;

			default:

				log_warning(logger,"Operacion desconocida."); //Codigo para cualquier otro op_code no mencionado arriba
				break;

		}
    log_info(logger, "Kernel - Recoleccion de informacion para generar proceso completada");
}
