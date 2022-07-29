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

#include <planificacion.h>
#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>
#include <libreriaConexiones.h>

t_log* logger;

//char* recibir_buffer(uint32_t*, uint32_t);

//t_instructions_list* recibir_paquete(uint32_t, t_log*);
//uint32_t recibir_operacion(uint32_t);


static void procesar_conexion(void* void_args);
int server_escuchar(t_log* logger, char* server_name, int server_socket);

#endif /* UTILS_H_ */
