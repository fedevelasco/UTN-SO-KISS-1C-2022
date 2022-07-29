#ifndef PROCESO_H_
#define PROCESO_H_

#include "pcb.h"

typedef struct t_proceso {

	int consola; // Indice de socketCliente
	int cpu; // Indice de socketCliente, legible solo cuando estado sea EXEC
	// t_proceso_estado estado;
	t_PCB* pcb; 
	uint32_t socketConsola; // Socket de consola asociado al proceso
	uint32_t socketCPU; // Socket de CPU asociado al proceso
	int rafagas; 
	bool suspendido;

} t_proceso;

t_proceso* proceso_crear();

void proceso_liberar(t_proceso* proceso);

void proceso_asignar_pcb(t_proceso* proceso, t_PCB* pcb);

void proceso_asignar_socket_de_cliente(t_proceso* proceso, uint32_t cliente_socket);

void proceso_asignar_socket_de_cpu(t_proceso* proceso, uint32_t cpu_socket);


#endif /* PROCESO_H_ */
