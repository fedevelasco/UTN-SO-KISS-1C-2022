#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdlib.h>
#include <stdio.h>




typedef struct {
	int32_t pid;
	int32_t process_size;
} t_process;

typedef struct {
	int32_t pid;
	int32_t first_level_table_number;
} t_process_suspend;


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

/**
* @NAME: create_process_suspend
* @DESC: Crea una instancia vacia de "t_process_suspend".
*/
t_process_suspend* create_process_suspend();



/**
* @NAME: process_suspend_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_process_suspend".
*/
void process_suspend_destroy(t_process_suspend* process);


#endif
