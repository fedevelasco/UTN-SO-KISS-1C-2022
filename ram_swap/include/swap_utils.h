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


uint32_t swap_init();
uint32_t swap_create(uint32_t pid, uint32_t size);
char* get_file_name(uint32_t pid);
uint32_t get_swap_size(uint32_t size);
void write_frame_in_swap(void* frame, uint32_t swap_page_id, uint32_t pid);
void free_process_swap(uint32_t pid);
void* read_page_in_swap(uint32_t swap_page_id, uint32_t pid);





#endif



