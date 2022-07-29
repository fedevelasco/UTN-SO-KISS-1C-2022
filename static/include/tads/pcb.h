#ifndef PCB_H_
#define PCB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "instructions_list.h"
#include "../serialization.h"

typedef struct {
	uint32_t PID; // identificador único
    uint32_t TAMANIO_PROCESO; // tamanio del proceso
    t_instructions_list* LISTA_INSTRUCCIONES; // Lista de instrucciones a ejecutar
	uint32_t PC;	 // Program Counter
	uint32_t TABLA_DE_PAGINAS;
	uint32_t ESTIMACION_RAFAGA;
} t_PCB;



t_PCB* pcb_create();

t_PCB* pcb_create_with_size(uint32_t size);

void pcb_destroy(t_PCB* pcb);

void pcb_imprimir(t_PCB* pcb, t_log* logger);

void pcb_asignar_lista_instrucciones(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_estimacion_rafaga_inicial(t_PCB* pcb, char* ESTIMACION_DATO);

void pcb_asignar_lista_tamanio(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_id_unico(t_PCB* pcb, t_log* logger);

uint32_t pcb_convertir_float_a_int(float numero_float);

float pcb_convertir_int_a_float(uint32_t numero_int);

uint32_t pcb_serializar(t_PCB* pcb, char* output);

uint32_t pcb_deserializar(t_PCB* destino, char* fuente);

char* pcb_creat_paquete(t_PCB* pcb, uint32_t bytes);

uint32_t bytes_PCB(t_PCB* pcb);

#endif /* PCB_H_ */
