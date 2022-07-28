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

t_traduccion_direcciones* obtenerTraduccionDeDirecciones(int socket);



#endif     