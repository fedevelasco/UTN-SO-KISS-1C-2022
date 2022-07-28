// FUNCIONES PROPIAS DEL CICLO DE INSTRUCCION
// Fetch -> Decode -> Fetch Operands -> Execute -> Check Interrupt

// Nuevas estructuras //

#include "../Include/cicloInstruccion.h"


t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->PC];
    return instruccion_actual;
}

t_paquete * cicloInstruccion(t_pcb * pcb) {
    pthread_mutex_lock(&mutex_interrupcion);
    hayInterrupcion=false;
    pthread_mutex_unlock(&mutex_interrupcion);
    t_paquete * paquete;
    bool seguirEjecutando = true;
    t_instruccion instruccion;
    uint32_t PC_inicial = pcb->PC;
    PCB_ACTUAL=pcb->id;
    log_info(logger, "Inicia ciclo instruccion para pcb id:%d", pcb->id);
    log_info(logger, " ");
    tablaPaginasPrimerNivelPCB = pcb->tablaDePaginas;

    while(seguirEjecutando ){
        instruccion = fetch(pcb);
        const char* identificador_instruccion;
        identificador_instruccion = instruccion_idAString((instruccion).identificador);
        log_info(logger, "Instrucción: %s", identificador_instruccion);
        seguirEjecutando = execute(instruccion);
        log_info(logger, " ");
        pcb->PC++;

        pthread_mutex_lock(&mutex_interrupcion);
        if (hayInterrupcion){
            hayInterrupcion=false;
            pthread_mutex_unlock(&mutex_interrupcion);
            pcb->lengthUltimaRafaga = pcb->PC - PC_inicial;
            log_info(logger, "Hay interrupcion, devulve el pcb");
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
            return paquete;
        }
        else{
            pthread_mutex_unlock(&mutex_interrupcion);
        }   
        
    }

    pcb->lengthUltimaRafaga = pcb->PC - PC_inicial;
    
    switch (instruccion.identificador){
        case IO:{
            t_IO * io = malloc(sizeof(t_IO));
            io->pcb = pcb;
            io->tiempoBloqueo = instruccion.parametro1;
            log_info(logger, "Ejecuto IO de: %d milisegundos", instruccion.parametro1);
            paquete = armarPaqueteCon(io, PCB_EJECUTADO_IO_CPU_KERNEL);
            free(io);
            //log_info(logger, "Ejecuto IO, devuelve el pcb id:%d", pcb->id);
            break;
        }
        case EXIT:{
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
            log_info(logger, "Ejecuto EXIT, devuelve el pcb id:%d", pcb->id);
            break;
        }
        default:{
           log_error(logger, "No ejecuto EXIT o IO, no debe pasar por aca");
           exit(EXIT_FAILURE);
           break;
        }
    }

    log_info(logger, "finaliza ciclo instruccion para pcb id:%d", pcb->id);
    return paquete;
}


bool execute(t_instruccion instruccion){
    switch (instruccion.identificador){
        case NO_OP:
            log_info(logger, "Ejecutado NO_OP");
            usleep(RETARDO_NOOP*1000);
            return true;
        case IO:
            //log_info(logger, "Ejecutando IO");
            return false;
        case READ: {
            uint32_t * dato = execute_read(instruccion.parametro1);
            log_info(logger, "Ejecutado READ");
            free(dato);
            //uint32_t * dato = execute_read(instruccion.parametro1);
            //log_info(logger, "Ejecutado READ valor: %d", *dato);
            return true;
        }
        case COPY:{
            //log_info(logger, "Ejecutando COPY");
            uint32_t * dato = execute_read(instruccion.parametro2);

            execute_write(instruccion.parametro1, *dato);
            free(dato);
            log_info(logger, "COPY finalizado");
            return true;
        }
        case WRITE:
            
            execute_write( instruccion.parametro1, instruccion.parametro2);
            log_info(logger, "Ejecutado Write");
            return true;
        case EXIT:
            log_info(logger, "Ejecutado Exit");
            return false;
        default:{
            log_error(logger, "IDENTIFICADOR INSTRUCCION NO CONTEMPLADO-> %d", instruccion.identificador);
            exit(EXIT_FAILURE);
            return false; 
        }
    }
}
uint32_t * execute_read(uint32_t direccion_logica){

    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica, REQ_MARCO_LECTURA_CPU_MEMORIA);
    uint32_t * dato = memoria_read(direccionFisica);
    log_info(logger, "READ dato: %d en dirección física: %d", *dato, direccionFisica);
    //leer direccion fisica en memoria y loggear dato
    return dato;
}

void execute_write(uint32_t direccion_logica, uint32_t dato){
    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica, REQ_MARCO_ESCRITURA_CPU_MEMORIA);
    memoria_write(direccionFisica, dato);
    log_info(logger, "WRITE dato: %d en dirección física: %d", dato, direccionFisica);
}

uint32_t * memoria_read(uint32_t direccion_fisica) {

    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_paquete * paquete = armarPaqueteCon(&direccion_fisica, REQ_READ_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    eliminarPaquete(paquete);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
    if(paqueteRespuesta->codigo_operacion != RES_READ_MEMORIA_CPU){
        perror("respuesta inesperada");
        exit(EXIT_FAILURE);
    }
    //TODO: ELIMINAR PAQUETE??
    uint32_t * dato = deserializarUINT32_T(paqueteRespuesta->buffer->stream);
    eliminarPaquete(paqueteRespuesta);
    return dato;
}

void memoria_write(uint32_t direccion_fisica, uint32_t dato) {

    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_peticionEscritura * peticion = malloc(sizeof(t_peticionEscritura));
    peticion->direccionFisica = direccion_fisica;
    peticion->dato = dato;
    peticion->id = PCB_ACTUAL;

    t_paquete * paquete = armarPaqueteCon(peticion, REQ_WRITE_CPU_MEMORIA); // agregar el dato al paquete
    enviarPaquete(paquete,socket_memoria);
    eliminarPaquete(paquete);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
    if(paqueteRespuesta->codigo_operacion != RES_WRITE_CPU_MEMORIA){
        perror("respuesta inesperada");
        eliminarPaquete(paqueteRespuesta);
        exit(EXIT_FAILURE);
    }
    eliminarPaquete(paqueteRespuesta);
    free(peticion);
}


                // ----------- INTEGRACION CON OTROS ARCHIVOS ------------ //

// #include "cpu.h"      // DEBERIA REESTRUCTURAR EL .h PARA TENER UN cicloInstruccion.h // 


// // FETCH - buscar la próxima instrucción a ejecutar// 

// /*----- Operaciones sobre el PC y avisos por quantum -----*/


// void setearPC(t_PCB* pcb, t_puntero_instruccion pc) {
// 	if(!CU_is_test_running()){
// 		log_info(activeLogger, "Actualizando PC de |%d| a |%d|.", pcb->PC, (int)pc);
// 	}
// 	pcb->PC = (int)pc;
// }

// void incrementarPC(t_PCB* pcb) {
// 	setearPC(pcb, (t_puntero_instruccion)((pcb->PC) + 1));
// }

// void loggearFinDePrimitiva(char* primitiva) {
// 	log_debug(debugLogger, ANSI_COLOR_GREEN "La primitiva |%s| finalizó OK." ANSI_COLOR_RESET, primitiva);
// }

// void desalojarProceso() {
// 	log_info(activeLogger, "Desalojando proceso...");

// 	enviarPCB();
// 	ejecutando = false;
// 	log_info(activeLogger, "Proceso desalojado.");
// }

// // void pass(){} //no borrar

// void obtenerPCB() {		//recibo el pcb que me manda kernel
// 	if(pcbActual!=NULL){
// 		pcb_destroy(pcbActual);
// 	}
// 	ejecutando = true;

// 	pcbActual=malloc(sizeof(t_PCB));
// 	//log_debug(debugLogger, "Recibiendo PCB...");
// 	char* serialPCB = leerLargoYMensaje(kernel);
// 	log_debug(debugLogger, "PCB recibido!");
// 	deserializar_PCB(pcbActual, serialPCB);
// 	enviarPID();
// 	cantidadPaginasCodigo = pcbActual->cantidad_paginas;
// 	stack = pcbActual->SP;
// 	free(serialPCB);
// }

// void enviarPCB() {
// 	//log_debug(debugLogger, "Enviando PCB a kernel...");
// 	int bytes = bytes_PCB(pcbActual);
// 	//imprimir_PCB(pcbActual);
// 	char* serialPCB = malloc(bytes);
// 	serializar_PCB(serialPCB, pcbActual);

// 	enviarLargoYSerial(kernel, bytes, serialPCB);
// 	log_debug(debugLogger, "PCB Enviado!");
// 	free(serialPCB);
// }

// /**
//  * Llamo a la funcion analizadorLinea del parser y logeo
//  */
// void parsear(char* const sentencia) {
// 	log_info(activeLogger, "Ejecutando la sentencia |%s|...", sentencia);
// 	pcbActual->PC++; 			//si desp el parser lo setea en otro lado mediante una primitiva, es tema suyo.
// 								//lo incremento antes asi no se desfasa.

// 	if(noEsEnd(sentencia)){
// 		analizadorLinea(sentencia, &funciones, &funcionesKernel); // chequear el funcionesKernel con juli
// 		if(flagMeSalteoTodoConGoto){return;}

// 		log_info(activeLogger, "PC actualizado a |%d|",pcbActual->PC);
// 		enviarHeader(kernel,headerTermineInstruccion);
// 		log_debug(debugLogger,"Informé a kernel del fin de una instrucción");
// 	}
// 	else{
// 		finalizar_proceso(true);
// 	}
// }


