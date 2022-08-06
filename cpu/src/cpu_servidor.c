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
            t_pcb * pcb = pcb_deserializar_estrucs(paquete->buffer->stream, 0); 
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
            log_info(logger, "Interrupt:recibido interrupcion");
            
            uint32_t * mensaje = deserializarUINT32_T(paquete->buffer->stream);
            free(mensaje);

            pthread_mutex_lock(&mutex_interrupcion);
            hayInterrupcion=true;
            pthread_mutex_unlock(&mutex_interrupcion);
            break; 
        }
        default:{
            log_error(logger, "Interrupt: llego un operation code erroneo: %d", paquete->codigo_operacion);
            break;
        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

void servidorInterrupt(void * socket){
    int socket_servidor = (int) socket;
    while(1){
    	char* cliente = "Kernel";
        int socket_cliente = esperar_cliente(logger, cliente, socket_servidor);
        log_info(logger, "Interrupt: se conecta Kernel");
        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarInterrupt(paquete, socket_cliente);
    }
}
