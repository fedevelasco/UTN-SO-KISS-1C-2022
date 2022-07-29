#ifndef RAM_CONNECTION_UTILS_H_
#define RAM_CONNECTION_UTILS_H_


#include <pthread.h>
#include <semaphore.h>
#include <libreriaConexiones.h>
#include <global_structures.h>
#include <protocol.h>
#include <connection_processor.h>


void server_listen_ram(char* server_name, int server_fd);
uint32_t kernel_opcode(t_op_code opcode);
void process_kernel_functions();
void process_cpu_functions();

sem_t sem_kernel_thread;
sem_t sem_cpu_thread;


#endif /* RAM_CONNECTION_UTILS_H_ */



