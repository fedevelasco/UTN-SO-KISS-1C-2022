#ifndef LIST_UTILS_H_
#define BUFFER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commons/collections/list.h>


/**
* @NAME: bytes_list
* @DESC: Retorna la cantidad de bytes ocupados por una lista con elementos de tamanio element_size.
*/
int32_t bytes_list(t_list* input, int32_t element_size);


#endif
