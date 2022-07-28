#include <tads/memory_config.h>


t_memory_config* create_memory_config(){
	t_memory_config* memory_config = malloc(sizeof(t_memory_config));
	memory_config->page_size = 0;
	memory_config->pages_per_table=0;
	return memory_config;
}

void memory_config_destroy(t_memory_config* memory_config){
	free(memory_config);
}





