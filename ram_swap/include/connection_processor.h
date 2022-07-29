#ifndef RAM_CONNECTION_PROCESSOR_H_
#define RAM_CONNECTION_PROCESSOR_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <tads/process.h>
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
#include <serialization.h>
#include <libreriaConexiones.h>
#include <global_structures.h>
#include <ram_functions.h>
#include <swap_utils.h>
#include <protocol.h>

//Kernel
void process_init(operation_buffer_t* operation_buffer);
void process_suspend(operation_buffer_t* operation_buffer);
void process_kill(operation_buffer_t* operation_buffer);

//Cpu
void get_memory_config(operation_buffer_t* operation_buffer);
void get_second_level_table(operation_buffer_t* operation_buffer);
void get_frame_read(operation_buffer_t* operation_buffer);
void get_frame_write(operation_buffer_t* operation_buffer);
void read_memory(operation_buffer_t* operation_buffer);
void write_memory(operation_buffer_t* operation_buffer);



#endif /* RAM_CONNECTION_PROCESSOR_H_ */


