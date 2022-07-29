#ifndef MEMORY_CONFIG_H_
#define MEMORY_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct memory_config {
	uint32_t page_size;
	uint32_t pages_per_table;
} t_memory_config;


/**
* @NAME: create_memory_config
* @DESC: Crea una instancia vacia de "t_memory_config".
*/
t_memory_config* create_memory_config();



/**
* @NAME: memory_config_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_memory_config".
*/
void memory_config_destroy(t_memory_config* memory_config);


#endif
