#include <ram_utils.h>

t_log* start_logger(void)
{
	t_log* new_logger;

	new_logger = log_create("console.log", "Console", 1, LOG_LEVEL_DEBUG);

	if(new_logger == NULL){
		printf("No fue posible crear el Logger\n");
		exit(1);
	}

	return new_logger;
}

int32_t load_configuration_file(t_config* config_to_load, t_log* logger)
{
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Inicio");
	t_config* new_configuration;

	new_configuration = config_create("../ram_swap/ram_swap.config");

	if (new_configuration == NULL){
		log_error(logger, "Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion");
		return 0;
	}

	//Almaceno valores de config en custom_config_t
	config.puerto_escucha = config_get_int_value(config_to_load, "PUERTO_ESCUCHA");
	config.tam_memoria = config_get_int_value(config_to_load, "TAM_MEMORIA");
	config.tam_pagina = config_get_int_value(config_to_load, "TAM_PAGINA");
	config.entradas_por_tabla = config_get_int_value(config_to_load, "ENTRADAS_POR_TABLA");
	config.retardo_memoria = config_get_int_value(config_to_load, "RETARDO_MEMORIA");
	config.algoritmo_reemplazo = config_get_string_value(config_to_load, "ALGORITMO_REEMPLAZO");
	config.marcos_por_proceso = config_get_int_value(config_to_load, "MARCOS_POR_PROCESO");
	config.retardo_swap = config_get_int_value(config_to_load, "RETARDO_SWAP");
	config.path_swap = config_get_string_value(config_to_load, "PATH_SWAP");

	config_destroy(config_to_load);

	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Config en %s cargada ok", new_configuration->path);

	return 1;
}

int32_t memory_create(custom_config_t config, t_log* logger){

	//Creo memoria y la relleno
	memory_size= config.tam_memoria;
	memory = malloc(memory_size);
	if (memory == NULL) {
	        log_error(logger, "Fallo en el malloc de memory");
	        return 0;
	    }
	memset(memory,'\0',memory_size);

	retardo_memoria = config.retardo_memoria;

	last_memory_position_used = list_create();

	occupied_frames_vector_create(config, logger);

	log_info(logger,"Creada la memoria");

	return 1;

}

void occupied_frames_vector_create(custom_config_t config, t_log* logger){

	int32_t frames = config.tam_memoria / config.tam_pagina;

	//Creo vector de marcos ocupados y lo relleno
	occupied_frames_vector = malloc(sizeof(int32_t) * frames);

	memset(occupied_frames_vector,0,sizeof(int32_t) * frames);

}

void paging_create(custom_config_t config, t_log* logger){

	global_page_lists = malloc(sizeof(global_page_lists_t));

	global_page_lists.first_level_page_tables = list_create();
	global_page_lists.second_level_page_tables = list_create();

}


void close_process() {
	//TODO: REVISAR
//	destruirLogs();
//	log_destroy(dump);
//	list_destroy(listaTablasPaginas);
//	free(tlb);
//	free(memoria);
//	free(vectorClientes);
//	free(vectorMarcosOcupados);
//	pthread_mutex_destroy(&lock_accesoMarcosOcupados);
//	pthread_mutex_destroy(&lock_accesoMemoria);
//	pthread_mutex_destroy(&lock_accesoTabla);
//	pthread_mutex_destroy(&lock_accesoTlb);
//	pthread_mutex_destroy(&lock_accesoUltimaPos);
}




