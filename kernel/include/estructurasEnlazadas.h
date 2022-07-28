#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/pcb.h>

t_queue* COLA_NEW;
t_list* LISTA_READY;
t_queue* COLA_EXIT;
t_queue* COLA_SUSPENDED_READY;
t_list* LISTA_SUSPENDED_BLOCKED;
t_list*  LISTA_BLOCKED;

// SEMAFOROS


// Funciones de cola NEW 

void push_cola_new(t_PCB* lista); // Agregar nuevo elemento a la cola NEW

t_PCB* pop_cola_new(); // Quitar y devolver un elemento a la cola NEW

int largo_cola_new(); // Devuelve el tamanio de la cola

t_PCB* buscar_cola_new(unsigned long tid); // Busca y retorna un elemento de la cola NEW

void* remover_cola_new(unsigned long tid); // Elimina un elemento de la cola NEW

// Funciones de lista READY

void push_lista_ready(t_PCB* pcb);

int largo_lista_ready();

// Funciones de cola EXIT

void push_cola_exit(t_PCB* pcb);


#endif /* ESTRUCTURAS_H_ */
