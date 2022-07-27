#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int32_t pid;
	int32_t process_size;
	int32_t first_level_table_number;
} t_process;


/**
* @NAME: create_process
* @DESC: Crea una instancia vacia de "t_process".
*/
t_process* create_process();



/**
* @NAME: process_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_process".
*/
void process_destroy(t_process* process);


#endif
