#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <commons/collections/list.h>


/**
* @NAME: bytes_list
* @DESC: Retorna la cantidad de bytes ocupados por una lista con elementos de tamanio element_size.
*/
uint32_t bytes_list(t_list* input, uint32_t element_size);


#endif
