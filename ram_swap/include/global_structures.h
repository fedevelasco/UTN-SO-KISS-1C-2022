#ifndef INCLUDE_GLOBAL_STRUCTURES_H_
#define INCLUDE_GLOBAL_STRUCTURES_H_

#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <tads/memory_config.h>
#include <tads/page_table_request.h>
#include <tads/memory_write_request.h>


// Tabla de paginas primer nivel
typedef struct first_level_page_table {

	uint32_t pid;
	t_list* first_level_entries; //first_level_entries_t

} first_level_page_table_t;

typedef struct first_level_entries {
	uint32_t pid;
	uint32_t second_level_table_id;
} first_level_entries_t;

// Tabla de paginas segundo nivel
typedef struct second_level_page_table{

	t_list* pages; // page_t
} second_level_page_table_t;

typedef struct page {
    uint32_t pid;
    uint32_t frame_number;
    bool bit_U;     // bit de uso
    bool bit_P;     // bit de presencia
    bool bit_M;     // bit de modificado
    uint32_t swap_page_id;
} page_t;

typedef struct process_state{
	uint32_t frames_used;
	uint32_t clock_pointer;
	uint32_t first_level_page_table_id;
} process_state_t;

typedef struct operation_buffer{
	t_op_code opcode;
	char* buffer;
	int32_t client_socket;
} operation_buffer_t;

typedef struct swap_file {
	int32_t pid;
	int32_t swap_file;
	int32_t swap_size;
	void* swap_mmap;
} t_swap_file;


// Mutex

pthread_mutex_t MUTEX_MEMORY;
pthread_mutex_t MUTEX_SWAP;
pthread_mutex_t MUTEX_FIRST_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_ENTRY;
pthread_mutex_t MUTEX_PROCESS_EXTRA_INFO;
pthread_mutex_t MUTEX_ID_EN_SWAP;
pthread_mutex_t MUTEX_OCCUPIED_FRAMES;
pthread_mutex_t MUTEX_SWAP_FILES;

// Threads



// Variables globales
char* puerto_escucha;
uint32_t tam_memoria;
uint32_t tam_pagina;
uint32_t entradas_por_tabla;
uint32_t retardo_memoria;
char* 	algoritmo_reemplazo;
uint32_t marcos_por_proceso;
uint32_t retardo_swap;
char*   path_swap;
char* ip;
uint32_t cantidad_frames;

t_log* logger;

t_list* last_memory_position_used;

void* memory;
//void* swap_area;
t_bitarray* occupied_frames_bitarray;
t_dictionary* process_extra_info;

// Listas de paginas globales
t_list* global_first_level_page_tables;
t_list* global_second_level_page_tables;

//Lista de archivos swap
t_list* swap_files_list;

typedef struct {
    int socket;
    char* server_name;
} connection_args_t;


pthread_mutex_t MUTEX_KERNEL_QUEUE;
pthread_mutex_t MUTEX_CPU_QUEUE;

t_queue* kernel_queue;
t_queue* cpu_queue;



#endif /* INCLUDE_GLOBAL_STRUCTURES_H_ */
