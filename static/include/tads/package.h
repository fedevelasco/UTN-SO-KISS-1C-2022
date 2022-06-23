#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "tads/op_code.h"
#include "tads/buffer.h"


typedef struct
{
op_code operation_code;
t_buffer* buffer;
} t_package;

/**
* @NAME: create_package
* @DESC: Crea una instancia vacia de "t_package".
*/
t_package* create_package();

/**
* @NAME: create_package
* @DESC: Crea una instancia vacia de "t_package" copiando el contenido del buffer en el package.
*/
t_package* new_package(t_buffer* buffer);

/**
* @NAME: package_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_package".
*/
void package_destroy(t_package* package);


#endif

