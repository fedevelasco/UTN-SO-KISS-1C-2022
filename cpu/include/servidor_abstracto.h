#ifndef CPU_SERVIDOR_ABSTRACTO_H_
#define CPU_SERVIDOR_ABSTRACTO_H_

#include <stdlib.h>
#include <stdint.h>
#include <protocolo_transferencia.h>
#include  <libreriaConexiones.h>
#include "../include/sockets.h"
#include <global_structures.h>
#include "../include/cpu_servidor.h"

typedef struct {
	int socket;
	char nombre[10];
	void (*deserializarSegun)(t_paquete*, int);
} t_servidor;

void servidor(void * params);

t_servidor * obtenerServidor(int socket,
		void (*deserializarSegun)(t_paquete*, int), char nombre[10]);
#endif
