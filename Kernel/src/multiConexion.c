#include <pthread.h>

#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#include "logKernel.h"
#include "configKernel.h"
#include "conexion.h"

#define NUM_THREADS 2

kernel_cfg_t config_kernel;

t_config* nuevo_config = iniciar_config();
