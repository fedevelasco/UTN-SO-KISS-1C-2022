#include "../include/planificacion.h"

/*
Planificador de Largo Plazo
Al conectarse una consola al kernel, deberá generarse la estructura PCB detallada anteriormente y asignarse este proceso al estado NEW.

En caso de que el grado de multiprogramación lo permita, los procesos pasarán al estado READY, enviando un mensaje al módulo Memoria para 
que inicialice sus estructuras necesarias y obtener el valor de la tabla de páginas que deberá estar almacenado en nuestro PCB.

Cuando se reciba un PCB con motivo de finalizar el mismo, se deberá pasar al proceso al estado EXIT y dar aviso al módulo Memoria 
para que éste libere sus estructuras. Una vez liberadas, se dará aviso a la Consola de la finalización del proceso.
*/

// void obtener_valor_tabla_de_paginas(){}  ---------- Completar cuando se realice memoria

void planificadorLP_agregar_a_new(t_instructions_list* lista_instrucciones, t_log* logger){
    
    log_info(logger, "Planificador Largo Plazo -> Creando estructura de PCB\n");
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb = pcb_create();
	pcb_asignar_lista_instrucciones(pcb, lista_instrucciones);
    pcb_asignar_id_unico(pcb, logger);
    pcb_asignar_lista_tamanio(pcb, lista_instrucciones);
    pcb_imprimir(pcb, logger);

    log_info(logger, "Planificador Largo Plazo -> Asignando PCB a cola de estado NEW\n");
    push_cola_new(pcb);
    int temp = largo_cola_new();
    log_info(logger, "Planificador Largo Plazo -> Largo cola NEW = %i", temp);

}

void planificadorLP_agregar_a_ready(int grado_multiprogramacion, t_log* logger){

    if (largo_cola_new() <= grado_multiprogramacion){
        log_info(logger, "Planificador Largo Plazo -> Asignando PCB a lista READY");
        t_PCB* temp_pcb = malloc(sizeof(t_PCB));
        temp_pcb = pop_cola_new();
        log_info(logger, "Planificador Largo Plazo -> PID: %i", temp_pcb->PID);
        push_cola_ready(temp_pcb);
        int largo = largo_cola_ready();
        log_info(logger, "Largo lista READY: %i", largo);
        free(temp_pcb);

    }

}


/*
Planificador de Corto Plazo
Los procesos que estén en estado READY serán planificados mediante los siguientes algoritmos:
FIFO: First in First out
SRT: Shortest Remaining Time (también visto como SJF con desalojo)

En el caso de los algoritmos con desalojo, cuando un proceso llegue a la cola READY se deberá enviar una Interrupción al proceso CPU 
a través de la conexión de interrupt para indicarle que deberá desalojar al proceso que se encuentra actualmente en ejecución.

Al recibir el PCB del proceso en ejecución, se calcularán las estimaciones correspondientes, según indique el algoritmo, para seleccionar el siguiente a ejecutar.

Cabe aclarar que en todos los casos el PCB será recibido a través de la conexión de dispatch, 
quedando la conexión de interrupt dedicada solamente a enviar mensajes de interrupción.
*/