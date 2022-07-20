#ifndef RAM_CONNECTION_UTILS_H_
#define RAM_CONNECTION_UTILS_H_

#include <tads/op_code.h>
#include <libreriaConexiones.h>
#include <threads.h>
#include <connection_processor.h>
#include <ram_utils.h>

typedef struct {
    int fd;
    char* server_name;
} connection_args_t;

void process_connection(void* void_args);
int server_listen_ram(char* server_name, int server_fd);


#endif /* RAM_CONNECTION_UTILS_H_ */



