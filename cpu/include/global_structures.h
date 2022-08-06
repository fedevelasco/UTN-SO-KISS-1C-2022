#ifndef INCLUDE_GLOBAL_STRUCTURES_H_
#define INCLUDE_GLOBAL_STRUCTURES_H_

#include <commons/config.h>
#include <commons/log.h>
#include <math.h>

//Conexion con memoria
#include <tads/memory_config.h>
#include <tads/page_table_request.h>
#include <tads/memory_write_request.h>
#include <tads/buffer.h>
#include <tads/package.h>

#include <tads/instruction.h>
#include <tads/instructions_list.h>

typedef struct {
	uint32_t id;
	uint32_t tamanioProceso;
	uint32_t PC;
	uint32_t tablaDePaginas;
	uint32_t estimacionRafaga;
	uint32_t lengthUltimaRafaga;
	uint32_t sizeInstrucciones;
	t_instructions_list* instrucciones;
} t_pcb;

typedef struct {
	t_pcb * pcb;
	uint32_t tiempoBloqueo;
	uint32_t tiempoEspera;
} t_IO;

typedef struct {
	t_op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
	uint32_t longitud;
	char * texto;
} t_mensaje;

t_log* logger;

pthread_mutex_t mutex_interrupcion;
bool hayInterrupcion;

char* IP;
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* REEMPLAZO_TLB;
int RETARDO_NOOP;
int ENTRADAS_TLB;

t_memory_config* traduccion_direcciones;

#endif /* INCLUDE_GLOBAL_STRUCTURES_H_ */

