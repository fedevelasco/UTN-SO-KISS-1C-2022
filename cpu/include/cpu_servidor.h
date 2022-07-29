// contiene los h de inicializacion.h - dispatch.h - interrupt.h

#ifndef CPU_SERVIDOR_H_
#define CPU_SERVIDOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include "../include/servidor_abstracto.h"
    #include "../include/cicloInstruccion.h"
    #include "../include/variablesGlobales.h"
    #include "../include/mmu.h"
    #include <pthread.h>
// sin static 
    #include "../include/serializacion_cpu.h"

t_config * config;
t_config * ips;
char * IP;
char * PUERTO_ESCUCHA_DISPATCH;
char * PUERTO_ESCUCHA_INTERRUPT;
char * IP_MEMORIA;
char * PUERTO_MEMORIA;
char * REEMPLAZO_TLB;
int RETARDO_NOOP;
int ENTRADAS_TLB;
bool hayInterrupcion;
pthread_mutex_t mutex_interrupcion;
t_traduccion_direcciones* traduccion_direcciones;


    void deserializarDispatch(t_paquete * paquete, int socket_cliente);
    void deserializarInterrupt(t_paquete * paquete, int socket_cliente);
    void servidorInterrupt(void * socket);

#endif
