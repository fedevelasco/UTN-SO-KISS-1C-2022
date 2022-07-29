#ifndef RAM_UTILS_H_
#define RAM_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <commons/config.h>

#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>
#include <global_structures.h>
#include <connection_utils.h>


// Funciones
t_log* start_logger(void);
uint32_t load_ram_configuration_file(char* config_path, char* ip_config_path);
uint32_t memory_create();
void occupied_frames_vector_create();
uint32_t paging_tables_create();
void close_process();
char* create_bitarray_memory_block(uint32_t frames);
uint32_t threads_init();
uint32_t mutex_init();
uint32_t aux_elements_init();


#endif /* RAM_UTILS_H_ */
