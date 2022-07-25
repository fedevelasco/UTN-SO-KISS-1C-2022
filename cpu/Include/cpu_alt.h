// tiene el main.h

#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/socket.h>
    #include <commons/config.h>
    #include <commons/log.h>
    #include <utils/inicializacion.h>

//    #include <utils/handshake_memoria.h>
    #include <conexiones.h>

    #include <serializacion/estructuras.h>
    #include <cpu_servidor.h>
    #include <pthread.h>
    #include <variables_globales.h>
    #include <cicloInstruccion.h>

    int* socket_dispatch;
    int * socket_interrupt;
    t_servidor * servidor_dispatch;
    t_servidor * servidor_interrupt;
    void iniciar_hilos(void);
    void inicializarVariablesGlobales(char * pathConfig,char * pathConfigIP);


#endif