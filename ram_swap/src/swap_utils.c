#include <swap_utils.h>

uint32_t swap_init() {
    if(pthread_mutex_init(&MUTEX_SWAP, (void *)NULL)){
    	log_error(logger, "swap_init - Error en creacion de mutex de swap");
    	return 1;
    }

    log_info(logger, "swap_init - Mutex de swap creado con exito");
    return 0;
}

uint32_t swap_create(uint32_t pid, uint32_t size){

    char* file_path = get_file_name(pid);
    uint32_t swap_size =  get_swap_size(size);

//    log_info(logger, "Creando directorio para archivo swap");
//    mkdir("/home/utnso/swap/", 777);

    log_info(logger, "swap_create- Creando archivo swap en path %s", file_path);
    uint32_t swap_file_fd = open(file_path, O_RDWR|O_CREAT, 0666);
    if (swap_file_fd == -1) {
        log_error(logger, "swap_create - No se pudo crear el area de SWAP. (errno %i)", errno);
        remove(file_path);
        return 1;
    }

    ftruncate(swap_file_fd, swap_size);

    t_swap_file* swap_file = malloc(sizeof(t_swap_file));
    swap_file->pid = pid;
    swap_file->swap_file = swap_file_fd;
    swap_file->swap_size = swap_size;

    swap_file->swap_mmap = mmap(NULL, swap_size, PROT_READ | PROT_WRITE, MAP_SHARED, swap_file_fd, 0);
    if ( swap_file->swap_mmap == MAP_FAILED){
    	perror("Errpr en creacion de swap_area:");
    	log_error(logger, "Error en creacion de swap_area: errno %i", errno);
    }

    memset(swap_file->swap_mmap, 0, swap_size);

    pthread_mutex_lock(&MUTEX_SWAP_FILES);
    list_add(swap_files_list, swap_file);
    pthread_mutex_unlock(&MUTEX_SWAP_FILES);

    return 0;

}

uint32_t get_swap_size(uint32_t size){
	uint32_t total_pages_needed = (uint32_t)ceil((double)size / (double)tam_pagina);
	return total_pages_needed * tam_pagina;
}

char* get_file_name(uint32_t pid) {
    char * file_name = string_new();
    string_append_with_format(&file_name, "%s/process_%d.swap", path_swap ,pid);
    return file_name;
}


void write_frame_in_swap(void* frame, uint32_t swap_page_id, uint32_t pid) {

	log_info(logger, "write_frame_in_swap - Escribiendo pagina en SWAP del proceso: %d - Inicio", pid);

	bool with_pid(t_swap_file* a_swap_file) {
	            return a_swap_file->pid == pid;
	        }

	 pthread_mutex_lock(&MUTEX_SWAP_FILES);
	 t_swap_file* swap_file = (t_swap_file*) list_find(swap_files_list, (void*) with_pid);
	 pthread_mutex_unlock(&MUTEX_SWAP_FILES);

    pthread_mutex_lock(&MUTEX_SWAP);

    log_info(logger, "Retardo de swap de %dms", retardo_swap);
    usleep(retardo_swap*1000);

    uint32_t offset = swap_page_id * tam_pagina;

    memcpy(swap_file->swap_mmap+offset, frame, tam_pagina);


    if(msync(swap_file->swap_mmap, tam_pagina, MS_SYNC)){
    	log_error(logger, "write_frame_in_swap - No se pudo escribir en disco el archivo swap. (errno %i)", errno);
    }

    log_info(logger, "write_frame_in_swap - Escribiendo pagina en SWAP del proceso: %d - Fin", pid);

    pthread_mutex_unlock(&MUTEX_SWAP);
}

void free_process_swap(uint32_t pid){

	log_info(logger, "free_process_swap - Eliminando el archivo SWAP del proceso: %d - Inicio", pid);

	bool with_pid(t_swap_file* a_swap_file) {
		            return a_swap_file->pid == pid;
		        }

	 pthread_mutex_lock(&MUTEX_SWAP_FILES);
	 t_swap_file* swap_file = (t_swap_file*) list_find(swap_files_list, (void*) with_pid);
	 pthread_mutex_unlock(&MUTEX_SWAP_FILES);

	 munmap(swap_file->swap_mmap, swap_file->swap_size);
	 close(swap_file->swap_file);

	char* file_path = get_file_name(pid);

    if(remove(file_path) == -1){
    	log_error(logger, "free_process_swap - No se pudo eliminar el archivo SWAP. (errno %i)", errno);
    }
    free(file_path);

    log_info(logger, "free_process_swap - Eliminando el archivo SWAP del proceso: %d - Fin", pid);
}

void* read_page_in_swap(uint32_t swap_page_id, uint32_t pid){

	log_info(logger, "read_page_in_swap - Leyendo pagina en SWAP del proceso: %d - Inicio", pid);

	bool with_pid(t_swap_file* a_swap_file) {
		            return a_swap_file->pid == pid;
		        }

	 pthread_mutex_lock(&MUTEX_SWAP_FILES);
	 t_swap_file* swap_file = (t_swap_file*) list_find(swap_files_list, (void*) with_pid);
	 pthread_mutex_unlock(&MUTEX_SWAP_FILES);


	pthread_mutex_lock(&MUTEX_SWAP);

	log_info(logger, "Retardo de swap de %dms", retardo_swap);
	usleep(retardo_swap*1000);

	uint32_t offset = swap_page_id * tam_pagina;

	void* swap_page_data = malloc(tam_pagina);

	memcpy(swap_page_data, swap_file->swap_mmap+offset, tam_pagina);

	pthread_mutex_unlock(&MUTEX_SWAP);

	log_info(logger, "read_page_in_swap - Leyendo pagina en SWAP del proceso: %d - Fin", pid);

	return swap_page_data;
}



