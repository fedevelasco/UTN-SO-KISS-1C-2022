#include "logKernel.h"

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>


int iniciar_servidor(t_log* logger, char IP, char PUERTO);


int esperar_cliente(int socket_servidor, t_log* logger);


int iniciar_cliente(char *ip, char* puerto, t_log* logger);

