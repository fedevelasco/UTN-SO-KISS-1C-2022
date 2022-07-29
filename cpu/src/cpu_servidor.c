// constiene los .c de inicializacion.c - dispatch.c - interrupt.c

// INTEGRO OPERACIONES //

#include "../include/cpu_servidor.h"

// INICIALIZACION //

void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

// DISPATCH //

void deserializarDispatch(t_paquete * paquete, int socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
            t_pcb * pcb = deserializarPCB(paquete->buffer->stream, 0);
            log_info(logger, "recibido pcb id:%d para ejecutar", pcb->id);
            vaciarTLB(pcb->id);
            
            t_paquete* resPaquete = cicloInstruccion(pcb);
            
            enviarPaquete(resPaquete, socket_cliente);
            eliminarPaquete(resPaquete);
            freePCB(pcb);
            break;
        }
        default:{

        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

// INTERRUPCIONES //

void deserializarInterrupt(t_paquete * paquete, int socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_INTERRUPCION_KERNEL_CPU:{
            log_info(logger, "interrupt:recibido interrupcion");
            
            uint32_t * mensaje = deserializarUINT32_T(paquete->buffer->stream);
            free(mensaje);

            pthread_mutex_lock(&mutex_interrupcion);
            hayInterrupcion=true;
            pthread_mutex_unlock(&mutex_interrupcion);
            break; 
        }
        default:{
            log_error(logger, "interrupt: no es por aca.");
            break;
        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

void servidorInterrupt(void * socket){
    int * socket_servidor = (int * )socket;
    while(1){
        int socket_cliente = esperar_cliente(*socket_servidor);
        log_info(logger, "interrupt: se conecta cliente");
        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarInterrupt(paquete, socket_cliente);
    }
}
