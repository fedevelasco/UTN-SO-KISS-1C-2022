// Contiene los handshake con kernel y consola, traduccion de direcciones y paquetes de handshake.h del otro

#include "../Include/conexiones.h"

// ***** ELIMINAR COMENTADOS  YA QUE SON VIEJOS ***** //

// int getHandshake(int cli) {
// 	char* handshake = recv_nowait_ws(cli, 1);
// 	return charToInt(handshake);
// }

// void warnDebug() {
// 	log_warning(activeLogger, "--- CORRIENDO EN MODO DEBUG!!! ---", getpid());
// 	log_warning(activeLogger, "--- CORRIENDO EN MODO DEBUG!!! ---", getpid());
// 	log_warning(activeLogger, "--- CORRIENDO EN MODO DEBUG!!! ---", getpid());
// 	log_warning(activeLogger, "--- CORRIENDO EN MODO DEBUG!!! ---", getpid());
// }

// -------- KERNEL -------- //

// void conectar_kernel() {
// 	direccionKernel = crearDireccionParaCliente(config.puertoKernel,
// 			config.ipKernel);
// 	kernel = socket_w();
// 	connect_w(kernel, &direccionKernel); //conecto cpu a la direccion 'direccionKernel'
// 	log_info(activeLogger, "Conectado a Kernel!");
// }

// void hacer_handshake_kernel() {
// 	char* hand = string_from_format("%c%c", HeaderHandshake, SOYCPU);
// 	send_w(kernel, hand, 2);

// 	if (getHandshake(kernel) != SOYKERNEL) {
// 		perror("Se esperaba que CPU se conecte con Kernel.");
// 	} else {
// 		log_info(bgLogger, "Exito al hacer handshake con Kernel.");
// 	}
// }

// -------- MEMORIA -------- //

// void conectar_memoria() {
// 	direccionMemoria = crearDireccionParaCliente(config.puertoMemoria, config.ipMemoria);
// 	memoria = socket_w();
// 	connect_w(memoria, &direccionMemoria); //conecto memoria a la direccion 'direccionMemoria'
// 	log_info(activeLogger, "Conectado a Memoria!");
// }

// void hacer_handshake_memoria() {
// 	char *hand = string_from_format("%c%c", HeaderHandshake, SOYCPU);
// 	send_w(memoria, hand, 2);

// 	if (getHandshake(memoria) != SOYMEMORIA) {
// 		perror("Se esperaba que CPU se conecte con Memoria.");
// 	} else {
// 		log_info(bgLogger, "Exito al hacer handshake con Memoria.");
// 	}
// }


t_traduccion_direcciones* obtenerTraduccionDeDirecciones(int socket){
    t_mensaje * mensaje = malloc(sizeof(t_mensaje));
    mensaje->texto=string_new();
    string_append(&mensaje->texto,"hola");
    mensaje->longitud=strlen(mensaje->texto)+1;
    t_paquete * paquete = armarPaqueteCon(mensaje,REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA);
    enviarPaquete(paquete,socket);
    eliminarPaquete(paquete);
    
    free(mensaje->texto);
    free(mensaje);
    

    //respuesta
    paquete = recibirPaquete(socket);
    if(paquete->codigo_operacion!=RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU){
        perror("No se recibio el codigo de operacion esperado para traduccion de direcciones");
        exit(EXIT_FAILURE);
    }
    t_traduccion_direcciones* traduccion_direcciones = deserializarTraduccionDirecciones(paquete->buffer->stream);

    eliminarPaquete(paquete);

    return traduccion_direcciones;


}

char* recibir_paquete(uint32_t socket_cliente)
{
	uint32_t buffer_size;
	char* buffer;

	buffer = recibir_buffer(&buffer_size, socket_cliente);
	
	return buffer;
}

char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente)
{
	char* buffer;

	read(socket_cliente, buffer_size, sizeof(uint32_t));
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size);

	return buffer;
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

uint32_t send_to_server(uint32_t connection, t_package* package)
{
	if(send_package(connection, package) == -1){
		package_destroy(package);
		return -1;
	}
	package_destroy(package);

	return 1;
}


uint32_t send_package(uint32_t connection, t_package* package)
{
	uint32_t bytes = package->buffer->size + 2*sizeof(uint32_t);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, MSG_CONFIRM ) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}

t_buffer* new_page_table_request_buffer(t_page_table_request* request){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_page_table_request(buffer->stream, request);

	log_debug(logger, "new_page_table_request_buffer - size: %d\n", offset);

	return buffer;
}


// -------- ESTABLECER CONEXIONES CON LOS OTROS MODULOS -------- //

// void establecerConexionConMemoria() {
// 	if (!config.DEBUG_IGNORE_MEMORIA) {
// 		conectar_memoria();
// 		log_info(activeLogger,"Estoy handshakeando");
// 		hacer_handshake_memoria();
// 	} else {
// 		warnDebug();
// 	}

// 	pedir_tamanio_paginas();
// }
// void establecerConexionConKernel() {
// 	if(!config.DEBUG_IGNORE_KERNEL){
// 		conectar_kernel();
// 		hacer_handshake_kernel();
// 	}else{
// 		warnDebug();
// 	}
// }



