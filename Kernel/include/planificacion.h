#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include "estructurasEnlazadas.h"
#include <stdio.h>
#include <string.h>

//const char* algoritmo_fifo = "FIFO";
//const char* algoritmo_sjf = "SRT";

// void obtener_valor_tabla_de_paginas(){}  ---------- Completar cuando se realice memoria

void planificadorLP_agregar_a_new(t_instructions_list* lista_instrucciones, t_log* logger);
void planificadorLP_agregar_a_ready(int grado_multiprogramacion, t_log* logger);
void planificadorLP_pasar_proceso_a_exit(t_PCB* pcb);

void planificadorCP_enviar_a_dispatch();

#endif /* PLANIFICACION_H_ */
