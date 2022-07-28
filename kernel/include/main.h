#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/log.h>
    #include <commons/config.h>
    #include <commons/collections/dictionary.h>
    #include <commons/collections/list.h>
    #include <pthread.h>
    #include <socket.h>
    #include <inicializacion.h>
    #include <estructura.h>
    #include <planificador.h>
    #include <globales.h>
    #include <inicializacion.h>
    

    void deserializarSegun(t_paquete* paquete, int socket);
    t_proceso * deserializarSegunProceso(t_paquete * paquete, int socket);


#endif