#include <ram_functions.h>


int32_t create_pid_memory(t_process* process){

	if(process_table_not_exists(process->pid)){

		int32_t total_pages_needed = process_total_pages_needed(process->process_size);
		int32_t complete_second_level_tables =  process_complete_second_level_tables(total_pages_needed);
		int32_t incomplete_second_level_table_pages = total_pages_needed % entradas_por_tabla;

		first_level_page_table_t* first_level_page_table = malloc(sizeof(first_level_page_table_t));
		first_level_page_table->pid = process->pid;
		first_level_page_table->first_level_entries = list_create();

		int32_t swap_id = 0;

		if (complete_second_level_tables > 0){
			for (int i = 0; i < complete_second_level_tables; i++){
				first_level_entries_t* first_level_entry = create_first_level_entry(entradas_por_tabla, process->pid, &swap_id);
				list_add(first_level_page_table->first_level_entries, first_level_entry);
			}
		}

		if (incomplete_second_level_table_pages > 0){
			first_level_entries_t* first_level_entry = create_first_level_entry(incomplete_second_level_table_pages, process->pid, &swap_id);
			list_add(first_level_page_table->first_level_entries, first_level_entry);
		}

		int32_t pid_first_level_table_number;
		pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
		pid_first_level_table_number = list_add(global_first_level_page_tables, first_level_page_table);
		pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

		log_info(logger, "create_pid_memory - Tabla de paginas de primer nivel con id: %d creada para el proceso %d", pid_first_level_table_number, process->pid);

		create_process_state(pid_first_level_table_number, process);

		return pid_first_level_table_number;
	}

	log_error(logger, "create_pid_memory - Tabla de paginas de primer nivel ya existe para proceso: %d", process->pid);

	return -1;


}

int32_t process_total_pages_needed(int32_t size){
    return (int32_t)ceil((double)size / (double)tam_pagina);
}

int32_t process_complete_second_level_tables(int32_t total_pages_needed){
	return (int32_t) floor ( (double) total_pages_needed / (double) entradas_por_tabla);
}

first_level_entries_t* create_first_level_entry(int32_t second_level_entries, int32_t process_pid, int32_t *swap_id){
	second_level_page_table_t* second_level_page_table = create_second_level_table(second_level_entries, process_pid, swap_id);

    first_level_entries_t* first_level_entry = malloc(sizeof(first_level_entries_t));

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
    first_level_entry->second_level_table_id = list_add(global_second_level_page_tables, second_level_page_table);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

    log_info(logger, "create_first_level_entry - tabla segundo nivel creada para pid: %d agregada a lista global con id: %d", process_pid, first_level_entry->second_level_table_id );


    return first_level_entry;
}

second_level_page_table_t* create_second_level_table(int32_t pages, int32_t process_pid, int32_t *swap_id){

	second_level_page_table_t* second_level_page_table = malloc(sizeof(second_level_page_table_t));
	second_level_page_table->pages = list_create();

    for(int i=0; i < pages; i++){
        page_t* page = create_second_level_page(process_pid, swap_id);
        list_add(second_level_page_table->pages, page);
    }
    log_info(logger, "create_second_level_table - tabla segundo nivel creada para pid: %d", process_pid);
    return second_level_page_table;
}

page_t* create_second_level_page(int32_t process_pid, int32_t *swap_id)
{
    page_t* page = malloc(sizeof(page_t));

    page->frame_number = tam_memoria;
    page->pid = process_pid;
    page->bit_M = false;
    page->bit_P = false;
    page->bit_U = false;
    page->swap_page_id = *swap_id;

    log_info(logger, "create_second_level_page - pagina segundo nivel creada para pid: %d", page->pid);
    log_info(logger, "create_second_level_page - id de pagina en swap: %d", *swap_id);
    (*swap_id)++;
    return page;
}

void create_process_state(int32_t pid_first_level_table_number, t_process* process){

	process_state_t* process_state = malloc(sizeof(process_state_t));
	process_state->clock_pointer = 0;
	process_state->frames_used = 0;
	process_state->first_level_page_table_id = pid_first_level_table_number;

	pthread_mutex_lock(&MUTEX_PROCESS_EXTRA_INFO);
	dictionary_put(process_extra_info, string_itoa(process->pid), process_state);
	pthread_mutex_unlock(&MUTEX_PROCESS_EXTRA_INFO);
}

bool process_table_not_exists(int32_t pid){
	int32_t size = list_size(global_first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_first_level_page_tables,i);
		if(table->pid == pid)
			return false;
	}
	return true;
}


int32_t suspend_pid(t_process* process){

	log_info(logger, "suspend_pid - Swapeando paginas del proceso:%d - Inicio", process->pid);

	pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
	first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, process->first_level_table_number);
	pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    log_info(logger, "suspend_pid - Enviando tabla de primer nivel con id: %d a Swap", process->first_level_table_number);


	pthread_mutex_lock(&MUTEX_SECOND_LEVEL_ENTRY);
	list_iterate(first_level_page_table->first_level_entries, swap_first_level_entry);
	pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_ENTRY);

	pthread_mutex_lock(&MUTEX_PROCESS_EXTRA_INFO);
	process_state_t* process_state = dictionary_get(process_extra_info, string_itoa(process->pid));
	pthread_mutex_unlock(&MUTEX_PROCESS_EXTRA_INFO);

	process_state->frames_used = 0;


	return 0;
}

void swap_first_level_entry(void* entry){

	first_level_entries_t* first_level_entry = (first_level_entries_t*) entry;

    log_info(logger, "swap_first_level_entry - Enviando tabla de segundo nivel con id: %d a Swap", first_level_entry->second_level_table_id);

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
    second_level_page_table_t* second_level_page_table = list_get(global_second_level_page_tables, first_level_entry->second_level_table_id);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

    list_iterate(second_level_page_table->pages, swap_second_level_entry);
}

void swap_second_level_entry(void* entry){

	page_t* page = (page_t*) entry;

    if (page->bit_M && page->bit_P){

    	log_debug(logger, "Leyendo de memoria frame:%d del proceso: %d", page->frame_number, page->pid);
        void* frame = read_frame(page->frame_number);

        log_debug(logger, "Escribiendo en swap el frame:%d del proceso: %d", page->frame_number, page->pid);
        write_frame_in_swap(frame, page->swap_page_id, page->pid);

        //Reincio la pagina
        page->bit_P = 0;
        page->bit_M = 0;
        page->bit_U = 0;

        //Libero el frame de memoria
        pthread_mutex_lock(&MUTEX_OCCUPIED_FRAMES);
        bitarray_clean_bit(occupied_frames_bitarray, page->frame_number);
        pthread_mutex_unlock(&MUTEX_OCCUPIED_FRAMES);

        free(page);
    }
}

void* read_frame(int32_t frame_number){

    void* frame = malloc(tam_pagina);
    int32_t offset = frame_number * tam_pagina;

    pthread_mutex_lock(&MUTEX_MEMORY);
    memcpy(frame, memory + offset, tam_pagina);
    pthread_mutex_unlock(&MUTEX_MEMORY);

    return frame;
}

void free_memory(int32_t first_level_table_number){
    log_info(logger, "Se borra de memoria la tabla de primer nivel: %d", first_level_table_number);

    pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
    first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, first_level_table_number);
    pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_ENTRY);
    list_iterate(first_level_page_table->first_level_entries, free_first_level_entry);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_ENTRY);
}

void free_first_level_entry(void* entry){
	int32_t second_level_table_id = ((first_level_entries_t*) entry)->second_level_table_id;
    log_info(logger, "Se borra de memoria la tabla de segundo nivel:%d", second_level_table_id);

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
    second_level_page_table_t* second_level_page_table  = list_get(global_second_level_page_tables, second_level_table_id);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

    list_iterate(second_level_page_table->pages, free_second_level_entry);
}

void free_second_level_entry(void* entry){
	page_t* page = (page_t*) entry;
	log_info(logger, "Se borra de memoria la pagina de segundo nivel en el frame:%d del proceso:%d", page->frame_number, page->pid);
    if (page->bit_P){
        //Libero el frame de memoria. Dejo el frame disponible para otro proceso. El que lo use, va a pisar el contenido, no hace falta ponerlo en 0.
        pthread_mutex_lock(&MUTEX_OCCUPIED_FRAMES);
        bitarray_clean_bit(occupied_frames_bitarray, page->frame_number);
        pthread_mutex_unlock(&MUTEX_OCCUPIED_FRAMES);
    }
}

int32_t get_second_level_page_table(t_page_table_request* page_table_request){

	log_info(logger, "Retardo de memoria %dms", retardo_memoria);
	usleep(retardo_memoria*1000);

    pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
    first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, page_table_request->table_number);
    pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    first_level_entries_t* first_level_entry = list_get(first_level_page_table->first_level_entries, page_table_request->entry_number);

    return first_level_entry->second_level_table_id;
}




