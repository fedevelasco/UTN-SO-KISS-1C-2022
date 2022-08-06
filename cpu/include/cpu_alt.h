#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include "../include/conexiones.h"
#include <protocolo_transferencia.h>
#include "../include/cpu_servidor.h"
#include <global_structures.h>
#include "../include/cicloInstruccion.h"
#include "../include/servidor_abstracto.h"
#include <libreriaConexiones.h>

int socket_dispatch;
int socket_interrupt;
t_servidor * servidor_dispatch;
t_servidor * servidor_interrupt;
void iniciar_hilos(void);
void inicializarVariablesGlobales(char * pathConfig, char * pathConfigIP);

#endif
