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

#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/parameter.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/instruction.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/instructions_list.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/op_code.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/package.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/tads/buffer.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/serialization.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/serialization.h"



t_log* logger;

char* recibir_buffer(int32_t*, int32_t);

int32_t iniciar_servidor(t_log*, const char*, char*, char*);
t_instructions_list* recibir_paquete(int32_t, t_log*);
void recibir_mensaje(int32_t);
int32_t recibir_operacion(int32_t);

#endif /* UTILS_H_ */
