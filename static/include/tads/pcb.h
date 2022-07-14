#ifndef PCB_H_
#define PCB_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "instructions_list.h"
#include "../serialization.h"

typedef struct {
	int32_t PID; // identificador único
    int32_t TAMANIO_PROCESO; // tamanio del proceso
    t_instructions_list* LISTA_INSTRUCCIONES; // Lista de instrucciones a ejecutar
	int32_t PC;	 // Program Counter
	int32_t TABLA_DE_PAGINAS;
	int32_t ESTIMACION_RAFAGA;
} t_PCB;



t_PCB* pcb_create();

t_PCB* pcb_create_with_size(int32_t size);

void pcb_destroy(t_PCB* pcb);

void pcb_imprimir(t_PCB* pcb, t_log* logger);

void pcb_asignar_lista_instrucciones(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_lista_tamanio(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_id_unico(t_PCB* pcb, t_log* logger);

int pcb_serializar(t_PCB* pcb, t_log* logger, char* output);

int pcb_deserializar(t_PCB* destino, char* fuente);

#endif /* PCB_H_ */
