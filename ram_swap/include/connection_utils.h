#ifndef RAM_CONNECTION_UTILS_H_
#define RAM_CONNECTION_UTILS_H_

#include <tads/op_code.h>
#include <libreriaConexiones.h>
#include <connection_processor.h>

typedef struct {
    int fd;
    char* server_name;
} connection_args_t;

static void process_connection(void* void_args);

#endif /* RAM_CONNECTION_UTILS_H_ */



