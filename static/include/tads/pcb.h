#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "instructions_list.h"

typedef struct {
	int PID; // identificador único
    int32_t TAMANIO_PROCESO; // tamanio del proceso
    t_instructions_list* LISTA_INSTRUCCIONES; // Lista de instrucciones a ejecutar
	int PC;	 // Program Counter
	// Tabla de paginas A DEFINIR
	short int ESTIMACION_RAFAGA;
} t_PCB;



t_PCB* pcb_create();

void pcb_destroy(t_PCB* pcb);

void pcb_imprimir(t_PCB* pcb, t_log* logger);

void pcb_asignar_lista_instrucciones(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_lista_tamanio(t_PCB* pcb, t_instructions_list* lista_instrucciones);

void pcb_asignar_id_unico(t_PCB* pcb, t_log* logger);