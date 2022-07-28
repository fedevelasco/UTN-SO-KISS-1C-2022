// tiene el main.h

#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/config.h>
    #include <commons/log.h>
    #include <pthread.h>

//    #include <utils/handshake_memoria.h>
    #include "../Include/conexiones.h"

    // sin static
    #include "../Include/serializacion_cpu.h"
    #include "../Include/cpu_servidor.h"
    #include "../Include/variablesGlobales.h"
    #include "../Include/cicloInstruccion.h"
    #include "../Include/servidor_abstracto.h"

    int* socket_dispatch;
    int * socket_interrupt;
    t_servidor * servidor_dispatch;
    t_servidor * servidor_interrupt;
    void iniciar_hilos(void);
    void inicializarVariablesGlobales(char * pathConfig,char * pathConfigIP);


#endif