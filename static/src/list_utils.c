#include <list_utils.h>

int bytes_list(t_list* input, int32_t element_size){
	//Cantidad de elementos * su tamanio + 1 byte donde almaceno el tamanio
	return list_size(input)*element_size+1;
}
