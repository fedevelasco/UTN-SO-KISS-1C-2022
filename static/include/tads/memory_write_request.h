#ifndef MEMORY_WRITE_REQUEST_H_
#define MEMORY_WRITE_REQUEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct{
		uint32_t fisical_address;
		uint32_t data;
		uint32_t pid;
	}t_memory_write_request;

/**
* @NAME: create_memory_write_request
* @DESC: Crea una instancia vacia de "t_memory_write_request".
*/
t_memory_write_request* create_memory_write_request();



/**
* @NAME: memory_write_request_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_memory_write_request
*/
void memory_write_request_destroy(t_memory_write_request* memory_write_request);


#endif
