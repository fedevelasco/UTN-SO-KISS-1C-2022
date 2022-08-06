// CICLO DE INSTRUCCION
// Fetch -> Decode -> Fetch Operands -> Execute -> Check Interrupt

#include <cicloInstruccion.h>


t_instruction* fetch(t_pcb *  pcb){
    t_instruction* instruccion_actual = list_get(pcb->instrucciones->instructions, pcb->PC);

    return instruccion_actual;
}

t_paquete * cicloInstruccion(t_pcb * pcb) {
//    pthread_mutex_lock(&mutex_interrupcion);
//    hayInterrupcion=false;
//    pthread_mutex_unlock(&mutex_interrupcion);
    t_paquete * paquete;
    bool seguirEjecutando = true;
    t_instruction* instruccion;
    uint32_t PC_inicial = pcb->PC;
    PCB_ACTUAL=pcb->id;
    log_info(logger, "Inicia ciclo instruccion para el pcb id:%d", pcb->id);
    log_info(logger, " ");
    tablaPaginasPrimerNivelPCB = pcb->tablaDePaginas;

    while(seguirEjecutando ){
        instruccion = fetch(pcb);
        const char* identificador_instruccion;
        identificador_instruccion = instruccion->id;
        // log_info(logger, "Instrucción: %s", identificador_instruccion);
        seguirEjecutando = execute(instruccion);
        log_info(logger, " ");
        pcb->PC++;

        pthread_mutex_lock(&mutex_interrupcion);
        if (hayInterrupcion){
            hayInterrupcion=false;
            pthread_mutex_unlock(&mutex_interrupcion);
            pcb->lengthUltimaRafaga = pcb->PC - PC_inicial;
            log_info(logger, "Hay interrupcion, devuelve el pcb");

            if(string_equals_ignore_case(instruccion->id,"I/O")){

               	t_IO * io = malloc(sizeof(t_IO));
           		io->pcb = pcb;
           		io->tiempoBloqueo = ((t_parameter*) list_get(instruccion->parameters, 0))->value;
           		log_info(logger, "Ejecuto IO de: %d milisegundos", io->tiempoBloqueo);
           		paquete = armarPaqueteCon(io, PCB_EJECUTADO_IO_CPU_KERNEL);
           		free(io);
//           		log_info(logger, "Ejecuto IO, devuelve el pcb id:%d", pcb->id);
           		pthread_mutex_unlock(&mutex_interrupcion);
           		return paquete;

               } else if(string_equals_ignore_case(instruccion->id,"EXIT")){

               	paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
           		log_info(logger, "Ejecuto EXIT, devuelve el pcb id:%d", pcb->id);
           		pthread_mutex_unlock(&mutex_interrupcion);
				return paquete;

           	} else {
//           	 paquete = pcb_create_package_with_opcode(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
           	paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
           	 pthread_mutex_unlock(&mutex_interrupcion);
			 return paquete;
           	}
        } else {
        	hayInterrupcion=false;
            pthread_mutex_unlock(&mutex_interrupcion);
        }
    }

    pcb->lengthUltimaRafaga = pcb->PC - PC_inicial;
    

    if(string_equals_ignore_case(instruccion->id,"I/O")){

    	t_IO * io = malloc(sizeof(t_IO));
		io->pcb = pcb;
		io->tiempoBloqueo = ((t_parameter*) list_get(instruccion->parameters, 0))->value;
		log_info(logger, "Ejecuto IO de: %d milisegundos", io->tiempoBloqueo);
		paquete = armarPaqueteCon(io, PCB_EJECUTADO_IO_CPU_KERNEL);
		free(io);
//		log_info(logger, "Ejecuto IO, devuelve el pcb id:%d", pcb->id);

    } else if(string_equals_ignore_case(instruccion->id,"EXIT")){

    	paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
		log_info(logger, "Ejecuto EXIT, devuelve el pcb id:%d", pcb->id);

	} else {
		 log_error(logger, "No ejecuto EXIT o IO, no debe pasar por aca");
		 exit(EXIT_FAILURE);
	}


    log_info(logger, "Finaliza ciclo instruccion para pcb id:%d", pcb->id);
    return paquete;
}


bool execute(t_instruction* instruccion){

	if (string_equals_ignore_case(instruccion->id, "NO_OP")) {
		log_info(logger, "Ejecutando instruccion NO_OP");
		usleep(RETARDO_NOOP * 1000000);

		return true;
	} else if (string_equals_ignore_case(instruccion->id, "I/O")) {

		return false;
	} else if (string_equals_ignore_case(instruccion->id, "READ")) {
		execute_read( ((t_parameter*)list_get(instruccion->parameters,0))->value );
		log_info(logger, "Ejecutando instruccion READ %d", ((t_parameter*)list_get(instruccion->parameters,0))->value );

		return true;
	} else if (string_equals_ignore_case(instruccion->id, "COPY")) {
		log_info(logger, "Ejecutando instruccion COPY %d %d", ((t_parameter*)list_get(instruccion->parameters,0))->value, ((t_parameter*)list_get(instruccion->parameters,1))->value );
		uint32_t dato = execute_read( ((t_parameter*)list_get(instruccion->parameters,1))->value );

		execute_write(((t_parameter*)list_get(instruccion->parameters,0))->value, dato);
		
		log_info(logger, "COPY finalizado");
		return true;

	} else if (string_equals_ignore_case(instruccion->id, "WRITE")) {
		execute_write(((t_parameter*)list_get(instruccion->parameters,0))->value , ((t_parameter*)list_get(instruccion->parameters,1))->value);
		log_info(logger, "Ejecutado instruccion WRITE %d %d",((t_parameter*)list_get(instruccion->parameters,0))->value, ((t_parameter*)list_get(instruccion->parameters,1))->value );
		return true;
	} else if (string_equals_ignore_case(instruccion->id, "EXIT")) {
		log_info(logger, "Ejecutando instruccion Exit");
		return false;
	} else {
		log_error(logger, "IDENTIFICADOR INSTRUCCION NO CONTEMPLADO-> %s",
				instruccion->id);
		exit(EXIT_FAILURE);
		return false;
	}



}
uint32_t execute_read(uint32_t direccion_logica){

    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica, GET_FRAME_READ_REQUEST);
    uint32_t dato = memoria_read(direccionFisica);
    log_info(logger, "READ dato: %d en dirección física: %d", dato, direccionFisica);
    //leer direccion fisica en memoria y loggear dato
    return dato;
}

void execute_write(uint32_t direccion_logica, uint32_t dato){
    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica, GET_FRAME_WRITE_REQUEST);
    memoria_write(direccionFisica, dato);
    log_info(logger, "WRITE dato: %d en dirección física: %d", dato, direccionFisica);
}

uint32_t  memoria_read(uint32_t direccion_fisica) {

    uint32_t socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA);

    t_buffer* buffer = new_memoria_read_buffer(direccion_fisica);

    //Codigo operacion que envio: * REQUEST
	t_package* package = new_package(buffer, READ_MEMORY_REQUEST);

	if (send_to_server(socket_memoria, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");

	}
	
    uint32_t cod_op = recibir_operacion(socket_memoria);

   //Codigo operacion que recibo:* RESPONSE
	if(cod_op != READ_MEMORY_RESPONSE){
			perror("Respuesta inesperada");
			exit(EXIT_FAILURE);
		}

	char* buffer_recibido = recibir_paquete(socket_memoria);

	uint32_t dato;

	deserialize_int(&dato, buffer_recibido);

	if(dato == -1){
		log_error(logger, "Error al leer dato en memoria");
	}

	free(buffer_recibido);

    // uint32_t asdnum = 1;  
    // uint32_t* asd = &asdnum;

    return dato;
}
    // t_paquete * paquete = armarPaqueteCon(&direccion_fisica, REQ_READ_CPU_MEMORIA);
    // enviarPaquete(paquete,socket_memoria);
    // eliminarPaquete(paquete);

      
    


void memoria_write(uint32_t direccion_fisica, uint32_t dato) {

    uint32_t socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA);

    t_memory_write_request* peticion_escritura = create_memory_write_request();

    peticion_escritura->fisical_address = direccion_fisica;
    peticion_escritura->data = dato;
    peticion_escritura->pid = PCB_ACTUAL;

    t_buffer* buffer = new_peticion_buffer(peticion_escritura);

    //Codigo operacion que envio: * REQUEST
	t_package* package = new_package(buffer, WRITE_MEMORY_REQUEST);

	if (send_to_server(socket_memoria, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");

	}

   uint32_t cod_op = recibir_operacion(socket_memoria);
   //Codigo operacion que recibo:* RESPONSE
	if(cod_op != WRITE_MEMORY_RESPONSE){
			perror("Respuesta inesperada");
			exit(EXIT_FAILURE);
		}

	char* buffer_recibido = recibir_paquete(socket_memoria);

	uint32_t escritura_ok;

	deserialize_int(&escritura_ok, buffer_recibido);

	if(escritura_ok != 1){
		log_error(logger, "Error al escribir dato en memoria");
	}

	free(buffer_recibido);
}


