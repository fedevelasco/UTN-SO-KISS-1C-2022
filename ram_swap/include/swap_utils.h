#ifndef SWAP_UTILS_H_
#define SWAP_UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <global_structures.h>





pthread_mutex_t MUTEX_SWAP;


int32_t swap_init();
int32_t swap_create(int32_t pid, int32_t size);
char* get_file_name(int32_t pid);
int32_t get_swap_size(int32_t size);




#endif



