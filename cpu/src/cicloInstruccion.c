// FUNCIONES PROPIAS DEL CICLO DE INSTRUCCION
// Fetch -> Decode -> Fetch Operands -> Execute -> Check Interrupt



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

    //TODO: COMPLETAR CONEXION   
    uint32_t asdnum = 1;  
    uint32_t* asd = &asdnum;
    return asd;
}

void memoria_write(uint32_t direccion_fisica, uint32_t dato) {

    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

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
	buffer_destroy(buffer);
	package_destroy(package);

   uint32_t cod_op = recibir_operacion(socket_memoria);
   //Codigo operacion que recibo:* RESPONSE
	if(cod_op != WRITE_MEMORY_RESPONSE){
			perror("respuesta inesperada");
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

t_buffer* new_peticion_buffer(t_memory_write_request* peticion_escritura){

	t_buffer* buffer = create_buffer();

	buffer->size = 3*sizeof(uint32_t);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_memory_write_request(buffer->stream, peticion_escritura);

	log_debug(logger, "new_peticion_buffer - size: %d\n", offset);

	return buffer;
}
