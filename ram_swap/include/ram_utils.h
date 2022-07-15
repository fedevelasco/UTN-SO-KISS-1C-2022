#ifndef RAM_UTILS_H_
#define RAM_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/collections/list.h>
#include<commons/config.h>

#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>

typedef struct custom_config {
	int32_t puerto_escucha;
	int32_t tam_memoria;
	int32_t tam_pagina;
	int32_t entradas_por_tabla;
	int32_t retardo_memoria;
	char* 	algoritmo_reemplazo;
	int32_t marcos_por_proceso;
	int32_t retardo_swap;
	char*   path_swap;

} custom_config_t;

typedef struct global_page_lists {

	//Primer Nivel
	t_list* first_level_page_tables;

	//Segundo Nivel
	t_list* second_level_page_tables;

} global_page_lists_t;

typedef struct first_level_page_table {

	t_dictionary* table;

} first_level_page_table_t;


typedef struct second_level_page_table {
    uint32_t page_number;
    uint32_t frame_number;
    union {
        uint32_t bit_U;     // bit de uso
        uint32_t TUR;       // tiempo de ultima referencia
    };
    bool bit_P;             // bit de presencia
} second_level_page_table_t;

// Variables globales

t_log* logger;
custom_config_t config;
int32_t retardo_memoria;
t_list* last_memory_position_used;
int32_t memory_size;
void* memory;
int32_t* occupied_frames_vector;
global_page_lists_t global_page_lists;

// Funciones
t_log* start_logger(void);
t_config* load_configuration_file(t_log* logger);
void close_process();



#endif /* RAM_UTILS_H_ */
