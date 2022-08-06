#ifndef CPU_SERVIDOR_H_
#define CPU_SERVIDOR_H_

#include <stdlib.h>
#include <stdio.h>
#include "../include/servidor_abstracto.h"
#include "../include/cicloInstruccion.h"
#include <global_structures.h>
#include "../include/mmu.h"
#include <pthread.h>
#include "../include/protocolo_transferencia.h"

t_config * config;
t_config * ips;


void deserializarDispatch(t_paquete * paquete, int socket_cliente);
void deserializarInterrupt(t_paquete * paquete, int socket_cliente);
void servidorInterrupt(void * socket);

#endif
