#ifndef STATIC_SOCKETS_SOCKET_H_
#define STATIC_SOCKETS_SOCKET_H_    

#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <estructura.h>
#include <globales.h>
#include <errno.h>
#include <serialization.h>

//  int iniciar_servidor(char* IP, char* PORT);
// int esperar_cliente(int socket_servidor);

//    int crear_conexion(char *ip, char* puerto);

t_paquete* recibirPaquete(int server_socket);

uint32_t recibir_operacion(uint32_t socket_cliente);
char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente);
t_proceso* recibir_paquete_proceso(uint32_t socket_cliente);
t_proceso* recibirPaqquete_inicio(int server_socket);
t_buffer* new_crear_proceso_buffer(t_process* proceso);
uint32_t send_package(uint32_t connection, t_package* package);
uint32_t send_to_server(uint32_t connection, t_package* package);
char* recibir_paquete(uint32_t socket_cliente);

#endif
