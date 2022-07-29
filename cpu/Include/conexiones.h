#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
// #include <parser/parser.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
// #include "handshake.h"
// #include "header.h"
// #include "cliente-servidor.h"
// #include "log.h"
// #include "commonTypes.h"
#include "../Include/cpu_alt.h"
// sin static
#include "../Include/serializacion_cpu.h"

/*------------Variables Globales--------------*/
int kernel; //cpu es cliente del kernel
int memoria; //cpu es cliente de memoria
struct sockaddr_in direccionKernel;   //direccion del kernel
struct sockaddr_in direccionMemoria;	  //direccion memoria


// ***** Funciones de conexiones ***** //
int getHandshake(int cli);
void warnDebug();
void conectar_kernel();
void hacer_handshake_kernel();
void conectar_memoria();
void hacer_handshake_memoria();
void establecerConexionConMemoria();
void establecerConexionConKernel();

char* recibir_paquete(uint32_t socket_cliente);
char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente);
uint32_t recibir_operacion(uint32_t socket_cliente);
uint32_t send_package(uint32_t connection, t_package* package);
uint32_t send_to_server(uint32_t connection, t_package* package);

t_traduccion_direcciones* obtenerTraduccionDeDirecciones(int socket);



#endif     