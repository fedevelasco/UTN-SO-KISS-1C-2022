#ifndef RAM_CONNECTION_UTILS_H_
#define RAM_CONNECTION_UTILS_H_

#include <commons/collections/queue.h>
#include <semaphore.h>
#include <tads/op_code.h>
#include <libreriaConexiones.h>
#include <connection_processor.h>
#include <ram_utils.h>

typedef struct {
    int socket;
    char* server_name;
} connection_args_t;

pthread_mutex_t MUTEX_KERNEL_QUEUE;
pthread_mutex_t MUTEX_CPU_QUEUE;

sem_t sem_kernel_thread;
sem_t sem_cpu_thread;



t_queue* kernel_queue;
t_queue* cpu_queue;

void process_connection(void* void_args);
int server_listen_ram(char* server_name, int server_fd);
int32_t kernel_opcode(op_code opcode);
void process_kernel_functions();
void process_cpu_functions();


#endif /* RAM_CONNECTION_UTILS_H_ */



