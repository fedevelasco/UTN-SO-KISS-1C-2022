#include "logKernel.h"

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>


#define IP "127.0.0.1"
#define PUERTO "4444"


int iniciar_servidor(t_log* logger);


int esperar_cliente(int socket_servidor, t_log* logger);


int iniciar_cliente(char *ip, char* puerto, t_log* logger);

