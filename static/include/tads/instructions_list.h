#ifndef INSTRUCTIONS_LIST_H_
#define INSTRUCTIONS_LIST_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "instruction.h"

  typedef struct {
		t_list *instructions;
		uint32_t process_size;
	} t_instructions_list;



	/**
	* @NAME: create_instructions_list
	* @DESC: Crea una instancia vacia de "t_instructions_list".
	*/
	t_instructions_list* create_instructions_list();

	/**
	* @NAME: create_instructions_list_with_size
	* @DESC: Crea una instancia vacia de "t_instructions_list" especificando un tamanio para el malloc.
	*/
	t_instructions_list* create_instructions_list_with_size(uint32_t size);

    /**
    * @NAME: new_instructions_list
    * @DESC: Crea una instancia de "t_instructions_list".
    */
	t_instructions_list* new_instructions_list(t_list* instructions, uint32_t process_size);

	/**
	* @NAME: instructions_list_destroy
	* @DESC: Libera la memoria ocupada por una instancia de "t_instructions_list".
	*/
	void instructions_list_destroy(t_instructions_list* instructions_list);

	/**
	* @NAME: bytes_instructions_list
	* @DESC: Devuelve la cantidad de bytes ocupados por una instancia de "t_instructions_list".
	*/
	uint32_t bytes_instructions_list(t_instructions_list* instructions_list);

	void imprimir_lista_instrucciones(t_instructions_list* instructions_list);

#endif
