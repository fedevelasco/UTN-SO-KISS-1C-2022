#include <list_utils.h>

uint32_t bytes_list(t_list* input, uint32_t element_size){
	//Cantidad de elementos * su tamanio + 1 byte donde almaceno el tamanio
	return list_size(input)*element_size+1;
}
