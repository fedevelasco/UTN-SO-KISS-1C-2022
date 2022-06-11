#include "logKernel.h"

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>


int32_t iniciar_servidor(char* ip, char* puerto, t_log* logger);

// -------------- Aceptar cliente como Servidor --------------

int32_t esperar_cliente(int32_t socket_servidor, t_log* logger);

// -------------- Iniciar Cliente --------------

int iniciar_cliente(char *ip, char* puerto, t_log* logger);

// -------------- Liberar Conexiones --------------

void liberar_conexion(int* socket_cliente);