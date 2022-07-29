#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/log.h>
#include "../list_utils.h"

#include "parameter.h"

   typedef struct {
		char* id;
		t_list* parameters;
	} t_instruction;

	/**
	* @NAME: create_instruction
	* @DESC: Crea una instancia vacia de "t_instruction".
	*/
	t_instruction* create_instruction();

    /**
    * @NAME: new_instruction
    * @DESC: Crea una instancia de "t_instruction" seteando el id y la lista de parametros.
    */
	t_instruction* new_instruction(char* id, t_list* parameters);

	/**
	* @NAME: instruction_destroy
	* @DESC: Libera la memoria ocupada por una instancia de "t_instruction".
	*/
	void instruction_destroy(t_instruction* instruction);

	/**
	* @NAME: bytes_instruction
	* @DESC: Devuelve la cantidad de bytes ocupados por una instancia de "t_instruction".
	*/
	uint32_t bytes_instruction(t_instruction* instruction);


#endif
