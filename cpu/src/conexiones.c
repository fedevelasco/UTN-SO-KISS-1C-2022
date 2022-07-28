// Contiene los handshake con kernel y consola, traduccion de direcciones y paquetes de handshake.h del otro

#include "../Include/conexiones.h"

// ***** Funciones de conexiones ***** //

int getHandshake(int cli) {
	char* handshake = recv_nowait_ws(cli, 1);
	return charToInt(handshake);
}

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



