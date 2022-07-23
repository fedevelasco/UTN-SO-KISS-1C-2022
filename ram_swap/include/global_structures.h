#ifndef INCLUDE_GLOBAL_STRUCTURES_H_
#define INCLUDE_GLOBAL_STRUCTURES_H_

#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <commons/collections/queue.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>

// Tabla de paginas primer nivel
typedef struct first_level_page_table {

	int32_t pid;
	t_list* first_level_entries; //first_level_entries_t

} first_level_page_table_t;

typedef struct first_level_entries {
	int32_t pid;
	int32_t second_level_table_id;
} first_level_entries_t;

// Tabla de paginas segundo nivel
typedef struct second_level_page_table{

	t_list* pages; // page_t
} second_level_page_table_t;

typedef struct page {
    int32_t pid;
    int32_t frame_number;
    bool bit_U;     // bit de uso
    bool bit_P;     // bit de presencia
    bool bit_M;     // bit de modificado
    int32_t swap_page_id;
} page_t;

typedef struct process_state{
	int32_t frames_used;
	int32_t clock_pointer;
	int32_t first_level_page_table_id;
} process_state_t;

typedef struct operation_buffer{
	op_code opcode;
	char* buffer;
	int32_t client_socket;
} operation_buffer_t;

// Mutex

pthread_mutex_t MUTEX_MEMORY;
pthread_mutex_t MUTEX_SWAP;
pthread_mutex_t MUTEX_FIRST_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_ENTRY;
pthread_mutex_t MUTEX_PROCESS_EXTRA_INFO;
pthread_mutex_t MUTEX_ID_EN_SWAP;
pthread_mutex_t MUTEX_OCCUPIED_FRAMES;

// Threads

pthread_t kernel_thread;
pthread_t cpu_thread;

// Variables globales
char* puerto_escucha;
int32_t tam_memoria;
int32_t tam_pagina;
int32_t entradas_por_tabla;
int32_t retardo_memoria;
char* 	algoritmo_reemplazo;
int32_t marcos_por_proceso;
int32_t retardo_swap;
char*   path_swap;
char* ip;
int32_t cantidad_frames;

t_log* logger;

t_list* last_memory_position_used;

void* memory;
void* swap_area;
t_bitarray* occupied_frames_bitarray;
t_dictionary* process_extra_info;

// Listas de paginas globales
t_list* global_first_level_page_tables;
t_list* global_second_level_page_tables;

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



#endif /* INCLUDE_GLOBAL_STRUCTURES_H_ */
