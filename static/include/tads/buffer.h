#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/string.h>



typedef struct {
	int32_t size;
	char* stream;
} t_buffer;


/**
* @NAME: create_buffer
* @DESC: Crea una instancia vacia de "t_buffer".
*/
t_buffer* create_buffer();



/**
* @NAME: buffer_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_buffer".
*/
void buffer_destroy(t_buffer* buffer);


#endif
