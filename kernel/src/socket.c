#include <socket.h>

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
				"Kernel - El cliente se desconecto. Terminando conexion con el cliente."); //Codigo para finalizar conexion
		exit(-1);
		break;

	default:

		log_warning(logger, "Kernel - Operacion desconocida: %d", cod_op); //Codigo para cualquier otro op_code no mencionado arriba
		break;

	}

	log_info(logger, "Kernel - Recoleccion de informacion para generar proceso completada");
}


t_buffer* new_crear_proceso_buffer(t_process* proceso){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_process(buffer->stream, proceso);

	log_debug(logger, "Kernel - nuevo buffer recibido - size: %d\n", offset);

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