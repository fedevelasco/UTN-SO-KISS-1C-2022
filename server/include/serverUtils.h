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

#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>


#define IP "127.0.0.1"
#define PUERTO "4444"


t_log* logger;

char* recibir_buffer(uint32_t*, uint32_t);

uint32_t iniciar_servidor(t_log*, const char*, char*, char*);
uint32_t esperar_cliente(t_log*, const char*, uint32_t);
t_instructions_list* recibir_paquete(uint32_t, t_log*);
void recibir_mensaje(uint32_t);
uint32_t recibir_operacion(uint32_t);

#endif /* UTILS_H_ */
