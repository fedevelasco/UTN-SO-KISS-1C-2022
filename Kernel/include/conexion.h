#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include<pthread.h>
#include<commons/config.h>

#include "planificacion.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/parameter.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/instruction.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/instructions_list.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/op_code.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/package.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/buffer.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/serialization.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/serialization.h"

typedef struct {
	char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_CPU;
    char* PUERTO_CPU_DISPATCH;
    char* PUERTO_CPU_INTERRUPT;
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    char* ESTIMACION_INICIAL;
    char* ALFA;
    char* GRADO_MULTIPROGRAMACION;
    char* TIEMPO_MAXIMO_BLOQUEADO;
} t_config_data;

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;


t_log* logger;

// ---------------- CONFIG ------------------------

t_config* iniciar_config(void);
void terminar_config(t_config* config, t_config_data* config_data);
void cargar_config(t_config* config, t_config_data* config_data);

// ---------------- CONEXION -----------------------

char* recibir_buffer(int32_t*, int32_t);
int32_t iniciar_servidor(t_log*, const char*, char*, char*);
t_instructions_list* recibir_paquete(int32_t, t_log*);
int32_t recibir_operacion(int32_t);

// --------------- PROCESAR CONEXIONES DE CONSOLA ------------------------

static void procesar_conexion(void* void_args);
int server_escuchar(t_log* logger, char* server_name, int server_socket);

#endif /* UTILS_H_ */
