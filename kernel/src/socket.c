#include <socket.h>

/* Inicializo el sevidor en modo escucha */
// int iniciar_servidor(char* IP, char* PORT)
// {
// 	int socket_servidor;
// 	struct addrinfo hints, *server_info;

// 	memset(&hints, 0, sizeof(hints));
// 	hints.ai_family = PF_INET;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = IPPROTO_TCP;
// 	// hints.ai_family = AF_UNSPEC;
// 	// hints.ai_socktype = SOCK_STREAM;
// 	// hints.ai_flags = AI_PASSIVE;
	
// 	getaddrinfo(IP, PORT, &hints, &server_info);

// 	socket_servidor = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	
// 	if(socket_servidor == -1){
// 		perror("Kernel - Error al crear socket");
// 		log_error(logger,"server_listen_ram - Error en sem_post - sem_kernel_thread. (errno %i)", errno);
// 		exit(EXIT_FAILURE);
// 	}
// 	int yes = 1;
// 	setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes); // Setea el socket como reutilizable
// 	int binded = bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	
// 	if(binded == -1){
// 		perror("Kernel - Error al bindear el socket");
// 		exit(EXIT_FAILURE);
// 	}
	
// 	int listening = listen(socket_servidor, SOMAXCONN);
	
// 	if(listening == -1){
// 		perror("Kernel - error al escuchar conexion");
// 		exit(EXIT_FAILURE);
// 	}

// 	freeaddrinfo(server_info);
	
// 	return socket_servidor;
// }

// int esperar_cliente(int socket_servidor)
// {
// 	// Aceptamos un nuevo cliente
// 	int socket_cliente = accept(socket_servidor, NULL, NULL);
// 	if(socket_cliente == -1){
// 		perror("error al aceptar socket_cliente");
// 		exit(EXIT_FAILURE);
// 	}
// 	return socket_cliente;
// }

t_paquete* recibirPaquete(int server_socket){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer_new));

	uint32_t op_code;

	recv(server_socket, &op_code, sizeof(uint32_t), 0);

	paquete->codigo_operacion=op_code;

	recv(server_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(server_socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
}


//  int crear_conexion(char* ip, char* puerto) {

//  	struct addrinfo hints;
//  	struct addrinfo *server_info;

//  	memset(&hints, 0, sizeof(hints));
//  	hints.ai_family = AF_INET;
//  	hints.ai_socktype = SOCK_STREAM;
//  	hints.ai_flags = IPPROTO_TCP;
//  	// hints.ai_family = AF_UNSPEC;
// 	// hints.ai_socktype = SOCK_STREAM;
// 	// hints.ai_flags = AI_PASSIVE;

//  	getaddrinfo(ip, puerto, &hints, &server_info);

//  	int socket_cliente = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	
//  	if(socket_cliente == -1){
//  		perror("Error de creacion de socket");
// 		log_error(logger,"server_listen_ram - Error en sem_post - sem_kernel_thread. (errno %i)",errno);
//  		exit(EXIT_FAILURE);
//  	}

//  	int resultado_conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	
//  	if(resultado_conexion != 0){
//  		perror("Error de conexión");
//  		exit(EXIT_FAILURE);
//  	}

//  	freeaddrinfo(server_info);


//  	return socket_cliente;
//  }

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

char* recibir_paquete(uint32_t socket_cliente)
{
	uint32_t buffer_size;
	char* buffer;

	buffer = recibir_buffer(&buffer_size, socket_cliente);

	return buffer;
}

t_proceso* recibir_paquete_proceso(uint32_t socket_cliente)
{
	uint32_t buffer_size; //Creo el tamanio del buffer
	char* buffer; //Creo el buffer

	buffer = recibir_buffer(&buffer_size, socket_cliente); //leo el payload y lo asigno a buffer
	//log_info(logger, "Kernel - Buffer recibido de parte de consola: %i\n", buffer_size); 
	
	t_proceso* nuevo_proceso = malloc(sizeof(t_proceso));
	nuevo_proceso->sizeInstrucciones = buffer_size;
	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones
	nuevo_proceso->instrucciones = instructions_list;
	return nuevo_proceso;
}

t_proceso* recibirPaqquete_inicio(int server_socket){

	t_proceso* nuevo_proceso = malloc(sizeof(t_proceso));

	t_op_code cod_op = recibir_operacion(server_socket); // Recibo el cod_op

	switch (cod_op) {

	case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

		nuevo_proceso = recibir_paquete_proceso(server_socket); //Recibo paquete, lo deserializo y lo guardo en instruction_list
		nuevo_proceso->tamanioProceso = nuevo_proceso->instrucciones->process_size;
		log_info(logger, "Kernel - Paquete recibido");
		log_info(logger, "Kernel - Tamanio lista de instrucciones: %i", nuevo_proceso->sizeInstrucciones);
		log_info(logger, "Kernel - Tamanio de proceso: %i ", nuevo_proceso->tamanioProceso);
		// TODO: no funciona.... imprimir_lista_instrucciones(nuevo_proceso->instrucciones);

		return nuevo_proceso;
		break;

	case UNDEFINED:

		log_error(logger,
				"El cliente se desconecto. Terminando conexion con el cliente."); //Codigo para finalizar conexion
		exit(-1);
		break;

	default:

		log_warning(logger, "Operacion desconocida."); //Codigo para cualquier otro op_code no mencionado arriba
		break;

	}

	log_info(logger, "Kernel - Recoleccion de informacion para generar proceso completada");
}


t_buffer* new_crear_proceso_buffer(t_process* proceso){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_process(buffer->stream, proceso);

	log_debug(logger, "new_crear_proceso_buffer - size: %d\n", offset);

	return buffer;
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

uint32_t send_to_server(uint32_t connection, t_package* package)
{
	if(send_package(connection, package) == -1){
		package_destroy(package);
		return -1;
	} 
	package_destroy(package);
	
	return 1;
}








// copia por si se rompe algo reemplazar estas funciones

// uint32_t recibir_operacion(uint32_t socket_cliente)
// {
// 	uint32_t cod_op;
// 	if(recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL) > 0) //Recibo la cantidad de bytes que representan al op_code en el stream de datos
// 		return cod_op;
// 	else
// 	{
// 		close(socket_cliente);
// 		return -1;
// 	}
// }

// char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente) 
// {
// 	char* buffer;

// 	read(socket_cliente, buffer_size, sizeof(uint32_t)); //Leo el tamanio del payload y lo escribo en buffer_size
// 	buffer = malloc(*buffer_size);
// 	read(socket_cliente, buffer, *buffer_size); //Ahora leo la cantidad de payload que me indica buffer_size y o guardo en buffer que devuelvo

// 	return buffer;
// }

// t_instructions_list* recibir_paquete(uint32_t socket_cliente, t_log* logger)
// {
// 	uint32_t buffer_size; //Creo el tamanio del buffer
// 	char* buffer; //Creo el buffer

// 	buffer = recibir_buffer(&buffer_size, socket_cliente); //leo el payload y lo asigno a buffer
// 	log_info(logger, "Kernel - Buffer recibido de parte de consola: %i\n", buffer_size); 

// 	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

// 	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones

// 	return instructions_list;
// }

// t_instructions_list* recibirPaqquete_inicio(int server_socket, t_proceso* nuevo_proceso){

//     uint32_t cod_op = recibir_operacion(server_socket);// Recibo el cod_op
//     t_instructions_list* instructions_list; 
//         switch (cod_op) {

// 			case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

// 				instructions_list = recibir_paquete(server_socket, logger); //Recibo paquete, lo deserializo y lo guardo en instruction_list
// 				log_info(logger, "Kernel - Valores recibidos:");
// 				log_info(logger, "Kernel - Process size: %d", instructions_list->process_size);
//                 return(instructions_list);
// 				break;

// 			case -1:

// 				log_error(logger, "El cliente se desconecto. Terminando conexion con el cliente."); //Codigo para finalizar conexion
// 				return;

// 			default:

// 				log_warning(logger,"Operacion desconocida."); //Codigo para cualquier otro op_code no mencionado arriba
// 				break;

// 		}
//     log_info(logger, "Kernel - Recoleccion de informacion para generar proceso completada");
// }
