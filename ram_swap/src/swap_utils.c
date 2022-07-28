#include <swap_utils.h>

int32_t swap_init() {
    if(pthread_mutex_init(&MUTEX_SWAP, (void *)NULL)){
    	log_error(logger, "swap_init - Error en creacion de mutex de swap");
    	return 1;
    }

    log_info(logger, "swap_init - Mutex de swap creado con exito");
    return 0;
}

int32_t swap_create(int32_t pid, int32_t size){

    char* file_path = get_file_name(pid);
    int32_t swap_size =  get_swap_size(size);

//    log_info(logger, "Creando directorio para archivo swap");
//    mkdir("/home/utnso/swap/", 777);


    log_info(logger, "Retardo de swap %dms", retardo_swap);
    usleep(retardo_swap*1000);

    log_info(logger, "swap_create- Creando archivo swap en path %s", file_path);
    int32_t swap_file = open(file_path, O_RDWR|O_CREAT, 0666);
    if (swap_file == -1) {
        log_error(logger, "swap_create - No se pudo crear el area de SWAP. (errno %i)", errno);
        remove(file_path);
        return 1;
    }

    ftruncate(swap_file, swap_size);

    swap_area = mmap(NULL, swap_size, PROT_READ | PROT_WRITE, MAP_SHARED, swap_file, 0);
    if (errno!=0) log_error(logger, "Error en creacion de swap_area: errno %i", errno);

    memset(swap_area, 0, swap_size);

//    close(swap_file); //cerrar cuando termina el proceso

    return 0;

}

int32_t get_swap_size(int32_t size){
	int32_t total_pages_needed = (int32_t)ceil((double)size / (double)tam_pagina);
	return total_pages_needed * tam_pagina;
}

char* get_file_name(int32_t pid) {
    char * file_name = string_new();
    string_append_with_format(&file_name, "%s/process_%d.swap", path_swap ,pid);
    return file_name;
}


void write_frame_in_swap(void* frame, int32_t swap_page_id, int32_t pid) {

    pthread_mutex_lock(&MUTEX_SWAP);

    log_info(logger, "Retardo de swap de %dms", retardo_swap);
    usleep(retardo_swap*1000);

    int32_t offset = swap_page_id * tam_pagina;

    if(memcpy(swap_area+offset, frame, tam_pagina)){
    	log_error(logger, "write_frame_in_swap - No se pudo copiar el frame a el area de SWAP. (errno %i)", errno);
    }

    if(msync(swap_area, tam_pagina, MS_SYNC)){
    	log_error(logger, "write_frame_in_swap - No se pudo escribir en disco el archivo swap. (errno %i)", errno);
    }


    pthread_mutex_unlock(&MUTEX_SWAP);
}

void free_process_swap(int32_t pid){

	char* file_path = get_file_name(pid);

	log_info(logger, "Retardo de swap %dms", retardo_swap);

	usleep(retardo_swap*1000);

    if(remove(file_path) == -1){
    	log_error(logger, "free_process_swap - No se pudo eliminar el archivo SWAP. (errno %i)", errno);
    }
    free(file_path);
}

void* read_page_in_swap(int32_t swap_page_id, int32_t pid){

	pthread_mutex_lock(&MUTEX_SWAP);

	log_info(logger, "Retardo de swap de %dms", retardo_swap);
	usleep(retardo_swap*1000);

	int32_t offset = swap_page_id * tam_pagina;

	void* swap_page_data = malloc(sizeof(tam_pagina));

	if(memcpy(swap_page_data, swap_area+offset, tam_pagina)){
		log_error(logger, "read_frame_in_swap - No se pudo leer el frame del area de SWAP. (errno %i)", errno);
	}

	return swap_page_data;
}



