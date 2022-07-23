#ifndef RAM_CONNECTION_UTILS_H_
#define RAM_CONNECTION_UTILS_H_


#include <pthread.h>
#include <tads/op_code.h>
#include <libreriaConexiones.h>
#include <global_structures.h>
#include <protocol.h>
#include <connection_processor.h>



void process_connection(void* void_args);
int server_listen_ram(char* server_name, int server_fd);
int32_t kernel_opcode(op_code opcode);
void process_kernel_functions();
void process_cpu_functions();


#endif /* RAM_CONNECTION_UTILS_H_ */



