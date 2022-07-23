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


int32_t process_init(operation_buffer_t* operation_buffer);

#endif /* RAM_CONNECTION_PROCESSOR_H_ */


