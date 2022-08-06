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
#include <tads/package.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <libreriaConexiones.h>
#include <protocolo_transferencia.h>
#include <serialization.h>
#include <tads/memory_config.h>

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
t_buffer* new_page_table_request_buffer(t_page_table_request* request);
t_buffer* new_memoria_read_buffer(uint32_t direccion);
t_buffer* new_peticion_buffer(t_memory_write_request* peticion_escritura);
t_buffer* new_memory_config_buffer();

t_memory_config* obtenerTraduccionDeDirecciones();

#endif     
