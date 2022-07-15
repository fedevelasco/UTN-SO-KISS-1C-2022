#include "log.h"

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#define IP_KERNEL "127.0.0.1"
#define PUERTO_KERNEL "4444"

int32_t iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);

// -------------- Aceptar cliente como Servidor --------------

int32_t esperar_cliente(t_log* logger, const char* name, int32_t socket_servidor);

// -------------- Iniciar Cliente --------------

int iniciar_cliente(char *ip, char* puerto, t_log* logger);

// -------------- Liberar Conexiones --------------

void liberar_conexion(int* socket_cliente);

