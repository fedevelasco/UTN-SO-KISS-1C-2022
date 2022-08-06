#include <planificador.h>

int id_nuevo_proceso = 0;

/*
t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones, t_instruccion * instrucciones){
	t_proceso * proceso = malloc(sizeof(t_proceso));
	proceso->tamanioProceso = tamanioProceso;
	proceso->sizeInstrucciones = sizeInstrucciones;
	proceso->instrucciones = instrucciones;
	return proceso;
*/
t_pcb * iniciarPcb(t_proceso * proceso){
    t_pcb * pcb = malloc(sizeof(t_pcb));
    
    pcb->id = id_nuevo_proceso;
    id_nuevo_proceso++;
    pcb->tamanioProceso = proceso->tamanioProceso;
    pcb->sizeInstrucciones = proceso->sizeInstrucciones;
    pcb->instrucciones = proceso->instrucciones;
    pcb->lengthUltimaRafaga = 0;
    pcb->PC = 0;
    pcb->tablaDePaginas = 0;
    pcb->estimacionRafaga = ESTIMACION_INICIAL;
    free(proceso);
    return pcb;
}

// ---------- Agrego el PCB a la cola del estado NEW -------------
void ingresarANew(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void*) pcb);
    pthread_mutex_unlock(&mutex_estado_new);
    log_info(logger, "Kernel - Se agrega el PID: %d a la cola del estado NEW", pcb->id);
    //sem_post(&sem_hay_pcb_en_new);
    sem_post(&sem_hay_pcb_esperando_ready);
}

// ---------- Funcion que corre el hilo para administrar el planificador largo plazo -------------
void Aready(){
    while(1){

        sem_wait(&sem_hay_pcb_esperando_ready);

        sem_wait(&sem_multiprogramacion);
        log_info(logger, "Kernel - Grado de multiprogramación permite agregar proceso al estado READY\n");
        
        t_pcb * pcb = obtenerSiguienteAready();

        addEstadoReady(pcb);
        log_info(logger, "Kernel - Se elimino el proceso PID: %d de NEW y se agrego a READY", pcb->id);
        
        if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"SRT")){
            interrumpirPCB();
        }
        sem_post(&sem_ready);
    }
}


// ------------ Obtengo el siguiente PCB a ser agregado a la lista READY -----------------
t_pcb* obtenerSiguienteAready(){
    
    t_pcb* pcb = NULL;  
    	if(!queue_is_empty(estado_susp_ready)){
            
            pthread_mutex_lock(&mutex_estado_susp_ready);
    		pcb = queue_pop(estado_susp_ready); // Consigo un PCB de la cola de estado SUSPENDED READY
            pthread_mutex_unlock(&mutex_estado_susp_ready);
            //comunicacionMemoriaDesuspender(pcb);
            //por que hacemos esto? no sabemos que paginas necesitariamos
            //levantar del swap para colocar en memoria
            //al pasar de susp a ready no debemos mandar este mensaje
            return pcb;
    	}
        else{
            pthread_mutex_lock(&mutex_estado_new);
            pcb = queue_pop(estado_new); // Consigo un PCB de la cola de estado NEW
            comunicacionMemoriaCreacionEstructuras(pcb); // Le pregunto a memoria por el valor de tabla de paginas para el nuevo PCB, luego lo asigno
//            log_info(logger, "Kernel - Tabla de paginas primer nivel con index: %d, asignada al pcb: %d",pcb->tablaDePaginas, pcb->id);
            pthread_mutex_unlock(&mutex_estado_new);
            return pcb;
        }

}

void readyAexec(){
    //hilo de ejecucion
    while(1){
        sem_wait(&sem_ready);
        t_pcb * pcb;
        pcb = algoritmoPlanificacion();
        bool filtro(void* pcbAux){
        return ((t_pcb*) pcbAux)->id == pcb->id;
        };
        pthread_mutex_lock(&mutex_estado_ready);
        list_remove_by_condition(estado_ready, filtro);
        pthread_mutex_unlock(&mutex_estado_ready);
        id_pcb_ejecutando = pcb->id;
        ejecutarPCB(pcb);
    }
}
//pcb->estimacionRafaga = alfa*pcb->lengthUltimaRafaga + (1-alfa)*pcb->estimacionRafaga

t_pcb* planificacionSRT(){
	t_pcb* pcbPlanificado = NULL;
	t_pcb* pcbAux = NULL;
    int i;
	int indexAPlanificar;
	float shortestJob;

	pthread_mutex_lock(&mutex_estado_ready);
	pcbAux = list_get(estado_ready,0);
	pthread_mutex_unlock(&mutex_estado_ready);

	indexAPlanificar = 0;
	shortestJob = pcbAux->estimacionRafaga;
    
	pthread_mutex_lock(&mutex_estado_ready);

    log_info(logger, "Kernel - PCBs en estado READY: %d", list_size(estado_ready));

    t_pcb* pcbFor = NULL;
    for(i=0; i<list_size(estado_ready); i++){
        pcbFor = list_get(estado_ready,i);
        log_info(logger, "PID: %d | Estimacion de rafaga: %d | Largo ultima rafaga: %d", pcbFor->id, pcbFor->estimacionRafaga, pcbFor->lengthUltimaRafaga); 
    }
    
    pthread_mutex_lock(&mutex_estado_susp_ready);
    log_info(logger, "Kernel - PCBs en estado SUSP_READY: %d\n", queue_size(estado_susp_ready));
    pthread_mutex_unlock(&mutex_estado_susp_ready);
    uint32_t _id_pcb_ejecutando = id_pcb_ejecutando;
    bool filtro(void* pcbFiltro){
        return ((t_pcb*) pcbFiltro)->id == _id_pcb_ejecutando;
    };

    t_pcb* pcb_en_ejecucion = list_find(estado_ready, filtro);
    if(pcb_en_ejecucion == NULL){
        pcbPlanificado = list_get(estado_ready, indexAPlanificar);
        pthread_mutex_unlock(&mutex_estado_ready);
	    return pcbPlanificado;
    }
    
    for(i=1;i<list_size(estado_ready);i++){
    	pcbAux = list_get(estado_ready,i);
    	
    	if(shortestJob > pcbAux->estimacionRafaga){
    		shortestJob = pcbAux->estimacionRafaga;
    		indexAPlanificar = i;
    	}
    }
        
    if(shortestJob < (pcb_en_ejecucion->estimacionRafaga)){
        log_info(logger, "Kernel - SRT: pcb en READY tiene mas prioridad que el pcb en EXEC");
        pcbPlanificado = list_get(estado_ready, indexAPlanificar);
        pthread_mutex_unlock(&mutex_estado_ready);
        return pcbPlanificado;
    }
    else{
        pthread_mutex_unlock(&mutex_estado_ready);
        return pcb_en_ejecucion;
    }

    pcbPlanificado = list_get(estado_ready, indexAPlanificar);
    pthread_mutex_unlock(&mutex_estado_ready);
    return pcbPlanificado;
}


void execAexit(t_pcb * pcb){

    bool filtro(void* consola){
        return ((t_consola*) consola)->id == pcb->id;
    };
    t_consola * consolaAnotificar;
    
    comunicacionMemoriaFinalizar(pcb);



    pthread_mutex_lock(&mutex_consolas_conectadas);
    consolaAnotificar = list_remove_by_condition(consolas_conectadas, filtro);
    pthread_mutex_unlock(&mutex_consolas_conectadas);
    
    log_info(logger, "Kernel - FIN: Finalizando consola id: %d, socket: %d", consolaAnotificar->id, *consolaAnotificar->socket);

    t_paquete * paqueteAconsola = armarPaqueteCon(&(pcb->id), EXIT_CONSOLE);
    enviarPaquete(paqueteAconsola, *consolaAnotificar->socket); 
    eliminarPaquete(paqueteAconsola);

    close(*consolaAnotificar->socket);
    free(consolaAnotificar->socket);
    free(consolaAnotificar);
    freePCB(pcb);
    sem_post(&sem_multiprogramacion);
}

void hilo_block(){
    while(1){
        sem_wait(&sem_block);
        pthread_mutex_lock(&mutex_estado_blocked);
            t_IO * ultimoIO = queue_pop(estado_blocked);
        pthread_mutex_unlock(&mutex_estado_blocked);        
        uint32_t bloqueo = ultimoIO->tiempoEspera + (ultimoIO->tiempoBloqueo * 1000);

        if (bloqueo > TIEMPO_MAXIMO_BLOQUEADO){
             int ioParaSuspender = TIEMPO_MAXIMO_BLOQUEADO - ultimoIO->tiempoEspera;

               if (ioParaSuspender < 0)
            {
                log_info(logger, "Kernel - Tiempo de espera del PID: %d, son %d milisegundos", ultimoIO->pcb->id, ultimoIO->tiempoEspera / 1000);
                log_info(logger, "Kernel - PID: %d ejecutando IO de: %d milisegundos", ultimoIO->pcb->id, ultimoIO->tiempoBloqueo);
                comunicacionMemoriaSuspender(ultimoIO->pcb);
                sem_post(&sem_multiprogramacion);
                hacerIO(ultimoIO->tiempoBloqueo * 1000);
                log_info(logger, "Kernel - PID: %d excedió el tiempo maximo de bloqueo permitido de %d milisegundos", ultimoIO->pcb->id, TIEMPO_MAXIMO_BLOQUEADO / 1000);
                log_info(logger, "Kernel - Se suspende el pcb");
                addEstadoSuspReady(ultimoIO->pcb);
                sem_post(&sem_hay_pcb_esperando_ready);
            }
            else
            {
                log_info(logger, "Kernel - Tiempo de espera del PID: %d, son %d milisegundos", ultimoIO->pcb->id, ultimoIO->tiempoEspera / 1000);
                log_info(logger, "Kernel - PID: %d ejecutando IO de: %d milisegundos", ultimoIO->pcb->id, ioParaSuspender / 1000);
                hacerIO(ioParaSuspender);
                log_info(logger, "Kernel - PID: %d excedió el tiempo maximo de bloqueo permitido de %d milisegundos", ultimoIO->pcb->id, TIEMPO_MAXIMO_BLOQUEADO / 1000);
                log_info(logger, "Kernel - Se suspende el pcb");
                comunicacionMemoriaSuspender(ultimoIO->pcb);
                sem_post(&sem_multiprogramacion);
                log_info(logger, "Kernel - PID: %d ejecutando IO restante de %d milisegundos", ultimoIO->pcb->id, (ultimoIO->tiempoBloqueo - ioParaSuspender / 1000));
                hacerIO(ultimoIO->tiempoBloqueo * 1000 - ioParaSuspender); // restante
                addEstadoSuspReady(ultimoIO->pcb);
                sem_post(&sem_hay_pcb_esperando_ready);
            }

        } else {
        	log_info(logger, "Kernel - Tiempo de espera del PID: %d, son %d milisegundos", ultimoIO->pcb->id, ultimoIO->tiempoEspera / 1000);
			log_info(logger, "Kernel - PID: %d ejecutando IO de: %d milisegundos", ultimoIO->pcb->id, ultimoIO->tiempoBloqueo);
            hacerIO(ultimoIO->tiempoBloqueo * 1000);
            log_info(logger, "Kernel - Terminó el IO del PID: %d", ultimoIO->pcb->id);
            addEstadoReady(ultimoIO->pcb);
            if (string_equals_ignore_case(ALGORITMO_PLANIFICACION, "SRT"))
            {
                interrumpirPCB();
            }
            sem_post(&sem_ready);
        }

        free(ultimoIO);
      }

}//REQ_CREAR_PROCESO_KERNEL_MEMORIA
void sumarEspera(void *ultimoIO)
{
    t_IO *aModificar = (t_IO *)ultimoIO;
    aModificar->tiempoEspera += (100 * 1000);
}

void sumarEsperaSegundos(void *ultimoIO)
{
    t_IO *aModificar = (t_IO *)ultimoIO;
    aModificar->tiempoEspera += (1000000);
}

void hacerIO(uint32_t tiempoBloqueo)
{
    log_info(logger, "Kernel - Completando IO de %d milisegundos", tiempoBloqueo / 10000);
    if (tiempoBloqueo < 1000000)
    {
        for (int i = 0; i < tiempoBloqueo; i += (100 * 1000))
        {
            usleep(100 * 1000); // 100 milisegundos
            pthread_mutex_lock(&mutex_estado_blocked);
            list_iterate(estado_blocked->elements, sumarEspera);
            pthread_mutex_unlock(&mutex_estado_blocked);
        }
    }
    else
    {
        for (int i = 0; i < tiempoBloqueo / 1000000; i++)
        {
            sleep(1); // 100 milisegundos
            pthread_mutex_lock(&mutex_estado_blocked);
            list_iterate(estado_blocked->elements, sumarEsperaSegundos);
            pthread_mutex_unlock(&mutex_estado_blocked);
        }
    }
}

// -------------- Me conecto a memoria para suspender un proceso -------------- 
void comunicacionMemoriaSuspender(t_pcb * pcb){

    int socketMemoria = iniciar_cliente(IP_MEMORIA,PUERTO_MEMORIA);
    if (socketMemoria == -1){
        log_info(logger, "Kernel - No se pudo crear la conexion con memoria para suspender el proceso");
    }

    t_process* crear_proceso = create_process();
    crear_proceso->process_size = pcb->tamanioProceso;
	crear_proceso->pid = pcb->id;

    t_buffer* buffer = new_crear_proceso_buffer(crear_proceso);

	t_package* package = new_package(buffer, PROCESS_SUSPEND_REQUEST);

    if (send_to_server(socketMemoria, package) == -1) {
		log_error(logger, "Kernel - Error al enviar paquete a memoria");
	}

    uint32_t cod_op = recibir_operacion(socketMemoria);
	if(cod_op != PROCESS_SUSPEND_RESPONSE){
		perror("respuesta inesperada");
		exit(EXIT_FAILURE);
	}
    log_info(logger, "Kernel - Memoria suspendio el proceso correctamente");
}

// --------- Le pregunto a memorio por el valor de tabla de paginas para el nuevo PCB, luego lo asigno ----------------------
void comunicacionMemoriaCreacionEstructuras(t_pcb * pcb){

    int socketMemoria = iniciar_cliente(IP_MEMORIA,PUERTO_MEMORIA);
    if (socketMemoria == -1){
        log_info(logger, "Kernel - No se pudo crear la conexion con memoria para traer las estructuras");
    }

    t_process* crear_proceso = create_process();
    crear_proceso->process_size = pcb->tamanioProceso;
	crear_proceso->pid = pcb->id;

	t_buffer* buffer = new_crear_proceso_buffer(crear_proceso);

	t_package* package = new_package(buffer, PROCESS_INIT_REQUEST);

	if (send_to_server(socketMemoria, package) == -1) {
		log_error(logger, "Kernel - Error al enviar paquete a memoria");
	}

    uint32_t cod_op = recibir_operacion(socketMemoria);
	if(cod_op != PROCESS_INIT_RESPONSE){
			perror("respuesta inesperada");
			exit(EXIT_FAILURE);
		}

	char* buffer_recibido = recibir_paquete(socketMemoria);

	uint32_t tablaPaginas1erNivel;

	deserialize_int(&tablaPaginas1erNivel, buffer_recibido);

	free(buffer_recibido);

    pcb->tablaDePaginas = tablaPaginas1erNivel;

}

// -------------- Me conecto a memoria para que termine un proceso -------------- 
void comunicacionMemoriaFinalizar(t_pcb * pcb) {

    int socketMemoria = iniciar_cliente(IP_MEMORIA,PUERTO_MEMORIA);
    if (socketMemoria == -1){
        log_info(logger, "Kernel - No se pudo crear la conexion con memoria para terminar el proceso");
    }

    t_process* crear_proceso = create_process();
    crear_proceso->process_size = pcb->tamanioProceso;
	crear_proceso->pid = pcb->id;

    t_buffer* buffer = new_crear_proceso_buffer(crear_proceso);

	t_package* package = new_package(buffer, PROCESS_KILL_REQUEST);

    if (send_to_server(socketMemoria, package) == -1) {
		log_error(logger, "Kernel - Error al enviar paquete a memoria");
	}

    log_info(logger, "Kernel - Memoria termino el proceso correctamente");

}

t_pcb * algoritmoPlanificacion(){
    if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"FIFO")){
        log_info(logger, "Kernel - Se planifica FIFO");
        t_pcb * pcb = planificacionFIFO();
        log_info(logger, "Kernel - FIFO elige el PID: %d y se pasa a EXEC",pcb->id);
        return pcb;
    }
    else if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"SRT")){
        log_info(logger, "Kernel - Se planifica SRT");
        t_pcb * pcb = planificacionSRT();
        log_info(logger, "Kernel - SRT elige el PID: %d y se pasa a EXEC",pcb->id);
        return pcb;
    }
    else{
        char * error = string_new();
        string_append_with_format(&error,"Algoritmo de planificacion %s no soportado", ALGORITMO_PLANIFICACION);
        perror(error);
        exit(-1);
    }

}

/* Creo los hilos encargados de mover los procesos entre estados y ejecutar la logica */
void inicializarPlanificacion(){

    pthread_t hilo_Aready;
    pthread_create(&hilo_Aready, NULL, (void*) Aready, NULL);
    pthread_detach(hilo_Aready);

    pthread_t hilo_readyAexec;
    pthread_create(&hilo_readyAexec, NULL, (void*) readyAexec, NULL);
    pthread_detach(hilo_readyAexec);

    pthread_t hiloblock;
    pthread_create(&hiloblock, NULL, (void*) hilo_block, NULL);
    pthread_detach(hiloblock);

    log_info(logger, "Kernel - Listo para recibir servidores");
}

t_pcb* planificacionFIFO(){
    t_pcb* pcb;
    pthread_mutex_lock(&mutex_estado_ready);
    pcb = (t_pcb *) list_remove(estado_ready, 0);
    pthread_mutex_unlock(&mutex_estado_ready);

    return pcb;
}


// t_pcb * desalojarYEjecutar(t_pcb * pcb){
//     int socket_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
//     t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
//     enviarPaquete(paquete, socket_interrupt);

//     bool filtro(void* pcbFiltro){
//         return ((t_pcb*) pcbFiltro)->id == pcb->id;
//     };
    
//     pthread_mutex_lock(&mutex_estado_ready);
//     t_pcb * pcb = list_remove_by_condition(lista_ready, filtro);
//     pthread_mutex_unlock(&mutex_estado_ready);
//     return pcb;
// }
void interrumpirPCB(){
    log_info(logger, "Kernel - Interrumpiendo proceso");
    int socketInterrupt = iniciar_cliente(IP_CPU, PUERTO_CPU_INTERRUPT);
    int numero = 1;
    t_paquete * paquete = armarPaqueteCon(&numero, REQ_INTERRUPCION_KERNEL_CPU);
    enviarPaquete(paquete, socketInterrupt);
    eliminarPaquete(paquete);
    //close(socketInterrupt);
}

/* TODO: A implementar con nuestras cosas   
(a puerto dipatch)
REQ_PCB_A_EJECUTAR_KERNEL_CPU,  ejecutar pcb

PCB_EJECUTADO_IO_CPU_KERNEL, pcb necesita IO

PCB_EJECUTADO_EXIT_CPU_KERNEL, pcb ejecutado, puede finalizarce

PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL, el pcb que me devolvieron fue interrumpido

(a puerto interrupt)
REQ_INTERRUPCION_KERNEL_CPU, Interrumpir el pcb actual, le mando solo un opcode
*/
void ejecutarPCB(t_pcb * pcb){
    int socketDispatch = iniciar_cliente(IP_CPU, PUERTO_CPU_DISPATCH);
    t_paquete* paquete = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU); 
    enviarPaquete(paquete, socketDispatch);
    eliminarPaquete(paquete);
    freePCB(pcb);
    
    t_paquete * paqueteRespuesta = recibirPaquete(socketDispatch);
    switch (paqueteRespuesta->codigo_operacion){
        case PCB_EJECUTADO_IO_CPU_KERNEL:{
            
            t_IO *io = deserializarIO(paqueteRespuesta->buffer->stream);
            io->pcb->estimacionRafaga = ALFA * (io->pcb->lengthUltimaRafaga) + (1 - ALFA) * (io->pcb->estimacionRafaga);
            log_info(logger, "Kernel - Vuelve para hacer su IO: id: %d | Estimacion de refaga: %d | Largo ultima rafaga: %d", io->pcb->id, io->pcb->estimacionRafaga, io->pcb->lengthUltimaRafaga);
            t_IO *pcbBloqueado = malloc(sizeof(t_IO));
            pcbBloqueado->pcb = io->pcb;
            pcbBloqueado->tiempoBloqueo = io->tiempoBloqueo;
            pcbBloqueado->tiempoEspera = 0;
            addEstadoBlocked(pcbBloqueado);
            sem_post(&sem_block);
            free(io);
            // t_pcb * pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
            // pcbActualizado->estimacionRafaga = ALFA*(io->pcb->lengthUltimaRafaga) + (1-ALFA)*(io->pcb->estimacionRafaga);
            // log_info(logger, "estimacionRafaga: %d, id: %d, lengthUltimaRafaga: %d,PC: %d, sizeInstrucciones: %d, tablaDePaginas: %d, tamanioProceso: %d",
            //         pcbActualizado->estimacionRafaga, pcbActualizado->id, pcbActualizado->lengthUltimaRafaga, pcbActualizado->PC, pcbActualizado->sizeInstrucciones, pcbActualizado->tablaDePaginas, pcbActualizado->tamanioProceso);
            // addEstadoBlocked(pcbActualizado);

            // el hilo de bloqueados es el que se bloquea con usleep(io->tiempoBloqueo)
        break;
        }
        case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
            log_info(logger, "Kernel - Entró un pcb a exit");
            t_pcb * pcbActualizado = pcb_deserializar_estrucs(paqueteRespuesta->buffer->stream, 0);
            execAexit(pcbActualizado);
          
            break;
        }
        case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
            t_pcb* pcbActualizado = pcb_deserializar_estrucs(paqueteRespuesta->buffer->stream, 0);
            pcbActualizado->estimacionRafaga -= pcbActualizado->lengthUltimaRafaga; 
            log_info(logger, "Kernel - Entró un pcb desalojado por interrupción ID: id: %d | Estimacion de rafaga: %d | Largo ultima rafaga: %d", pcbActualizado->id, pcbActualizado->estimacionRafaga, pcbActualizado->lengthUltimaRafaga);
            addEstadoReady(pcbActualizado);
            sem_post(&sem_ready);
            // pthread_mutex_lock(&mutex_pcb_ejecutando);
            // pcbEjecutando = pcbActualizado;
            // pthread_mutex_unlock(&mutex_pcb_ejecutando)            
            //sem_post(&sem_no_suspended_ready);;

            break; 
        }   
        default:{
            eliminarPaquete(paqueteRespuesta);
            log_info(logger, "Kernel - cod op erroneo: %d", paqueteRespuesta->codigo_operacion);

            char * error = string_new();
            string_append_with_format(&error,"Kernel - Respuesta de CPU %s no soportada",codOPtoString(paqueteRespuesta->codigo_operacion));

            perror(error);
            exit(EXIT_FAILURE);
            break;
        }
    }
    eliminarPaquete(paqueteRespuesta);
}


void addEstadoReady(t_pcb* pcb){
//    log_info(logger, "se agrega pcb: %d a ready",pcb->id);
    pthread_mutex_lock(&mutex_estado_ready);
    list_add(estado_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_ready); 
}

void addEstadoBlocked(t_IO* io){
    log_info(logger, "Kernel - SUSPENSION: se agrega PID: %d a Blocked",io->pcb->id);
    pthread_mutex_lock(&mutex_estado_blocked);
    queue_push(estado_blocked, (void *) io);
    pthread_mutex_unlock(&mutex_estado_blocked);
}

void addEstadoSuspReady(t_pcb* pcb){
    log_info(logger, "Kernel - Se agrega PID: %d a SUSP_READY",pcb->id);
    pthread_mutex_lock(&mutex_estado_susp_ready);
    queue_push(estado_susp_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_susp_ready); 
}

