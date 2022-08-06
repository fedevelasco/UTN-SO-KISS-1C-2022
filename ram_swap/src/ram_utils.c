#include <ram_utils.h>

t_log* start_logger(void) {
	t_log* new_logger;

	new_logger = log_create("ram_swap.log", "Ram_Swap", 1, LOG_LEVEL_INFO);

	if (new_logger == NULL) {
		printf("No fue posible crear el Logger\n");
		exit(1);
	}

	return new_logger;
}

uint32_t load_ram_configuration_file(char* config_path, char* ip_config_path) {
	log_info(logger,"Carga de archivo de configuracion - load_configuration_file - Inicio");
	t_config* config_to_load;
	t_config* ip_config_to_load;


	config_to_load = config_create(config_path); // "../ram_swap/ram_swap.config"
	if (config_to_load == NULL) {
		log_error(logger,"Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion - path: %s",config_path);
		return 1;
	}

	ip_config_to_load = config_create(ip_config_path); // "../ram_swap/ips.config"
	if (ip_config_to_load == NULL) {
		log_error(logger,"Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion de ips - path: %s",ip_config_path);
		return 1;
	}

	//Almaceno valores de config en custom_config_t
	tam_memoria = config_get_int_value(config_to_load, "TAM_MEMORIA");
	tam_pagina = config_get_int_value(config_to_load, "TAM_PAGINA");
	entradas_por_tabla = config_get_int_value(config_to_load, "ENTRADAS_POR_TABLA");
	retardo_memoria = config_get_int_value(config_to_load, "RETARDO_MEMORIA");
	algoritmo_reemplazo = config_get_string_value(config_to_load, "ALGORITMO_REEMPLAZO");
	marcos_por_proceso = config_get_int_value(config_to_load, "MARCOS_POR_PROCESO");
	retardo_swap = config_get_int_value(config_to_load, "RETARDO_SWAP");
	path_swap = config_get_string_value(config_to_load, "PATH_SWAP");
	ip = config_get_string_value(ip_config_to_load, "IP_RAM");
	puerto_escucha = config_get_string_value(ip_config_to_load, "PUERTO_ESCUCHA_RAM");
	cantidad_frames = tam_memoria / tam_pagina;

	log_info(logger,"Carga de archivo de configuracion - load_configuration_file - Config en %s y en %s cargadas ok", config_path, ip_config_path);

	return 0;
}

uint32_t memory_create() {

	//Creo memoria y la relleno
	memory = malloc(tam_memoria);
	if (memory == NULL) {
		log_error(logger, "memory_create - Fallo en el malloc de memory");
		return 1;
	}
	memset(memory, '\0', tam_memoria);

	//Creo bitarray para saber frames libres

	uint32_t frames = cantidad_frames / 8;
	char *bitarray_memory_block = create_bitarray_memory_block(cantidad_frames);
    occupied_frames_bitarray = bitarray_create_with_mode(bitarray_memory_block, frames, LSB_FIRST);

	for (int i = 0; i < cantidad_frames; i++){
		bitarray_clean_bit(occupied_frames_bitarray, i);
	}

	last_memory_position_used = list_create();

	process_extra_info = dictionary_create();

	log_info(logger, "memory_create - Memoria creada con exito");

	return 0;
}

char* create_bitarray_memory_block(uint32_t frames){

    // Si cantidad frames menor a 8, ocupa 1 byte. Si es mayor ocupa, el entero superior.
    uint32_t size = frames < 8 ? 1 : (uint32_t) ceil( (double)frames / 8.0 );

    char* memory_block = malloc(size);
    memset(memory_block, 0, size);

    return memory_block;


}


uint32_t paging_tables_create() {

	global_first_level_page_tables = list_create();
	global_second_level_page_tables = list_create();

	swap_files_list = list_create();

	log_info(logger, "paging_tables_create - Tablas de paginas globales creadas con exito");

	return 0;

}

uint32_t mutex_init(){


	if (
	pthread_mutex_init(&MUTEX_MEMORY, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_SWAP, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_FIRST_LEVEL_TABLES, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_SECOND_LEVEL_TABLES, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_SECOND_LEVEL_ENTRY, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_PROCESS_EXTRA_INFO, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_ID_EN_SWAP, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_OCCUPIED_FRAMES, (void *)NULL) ||
	pthread_mutex_init(&MUTEX_SWAP_FILES,(void *)NULL)
	)
	{
		log_error(logger, "mutex_init - Error en creacion de mutex");

		return 1;
	}

	log_info(logger, "mutex_init - Mutex creados con exito");
	return 0;
}

uint32_t threads_init(){

	pthread_t kernel_thread;
	pthread_t cpu_thread;

	 if(pthread_create(&kernel_thread, NULL, (void*) process_kernel_functions, NULL)){
		 log_error(logger, "threads_init - Error en creacion de thread de kernel");
		 return 1;
	 }
	 pthread_detach(kernel_thread);
	 log_info(logger, "mutex_init - Thread de kernel creado");


	 if(pthread_create(&cpu_thread, NULL, (void*) process_cpu_functions, NULL)){
		 log_error(logger, "threads_init - Error en creacion de thread de cpu");
		 return 1;
	 }
	 pthread_detach(cpu_thread);
	 log_info(logger, "mutex_init - Thread de cpu creado");

	 return 0;
}

uint32_t aux_elements_init(){

	if(pthread_mutex_init(&MUTEX_KERNEL_QUEUE, (void *)NULL)){
		log_error(logger, "aux_elements_init - Error en creacion de mutex de cola de kernel");
		 return 1;
	}

	if(pthread_mutex_init(&MUTEX_CPU_QUEUE, (void *)NULL)){
		log_error(logger, "aux_elements_init - Error en creacion de mutex de cola de cpu");
		 return 1;
	}

    sem_init(&sem_kernel_thread, 1, 0);
    sem_init(&sem_cpu_thread, 1, 0);

    kernel_queue = queue_create();
    cpu_queue = queue_create();

    return 0;
}





void close_process() {
	//TODO: REVISAR
//	config_destroy(config_to_load);
//	config_destroy(ip_config_to_load);
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

