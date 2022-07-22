#ifndef RAM_UTILS_H_
#define RAM_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <math.h>
#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <commons/config.h>

#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>
#include <connection_utils.h>

// Mutex

pthread_mutex_t MUTEX_MEMORY;
pthread_mutex_t MUTEX_SWAP;
pthread_mutex_t MUTEX_FIRST_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_TABLES;
pthread_mutex_t MUTEX_SECOND_LEVEL_ENTRY;
pthread_mutex_t MUTEX_PROCESS_EXTRA_INFO;
pthread_mutex_t MUTEX_ID_EN_SWAP;
pthread_mutex_t MUTEX_OCCUPIED_FRAMES;


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
t_bitarray* occupied_frames_bitarray;
t_dictionary* process_extra_info;

// Listas de paginas globales
t_list* global_first_level_page_tables;
t_list* global_second_level_page_tables;





// Funciones
t_log* start_logger(void);
int32_t load_ram_configuration_file(char* config_path, char* ip_config_path);
int32_t memory_create();
void occupied_frames_vector_create();
int32_t paging_tables_create();
void close_process();
char* create_bitarray_memory_block(int32_t frames);
int32_t threads_init();
int32_t mutex_init();
int32_t aux_elements_init();




#endif /* RAM_UTILS_H_ */
