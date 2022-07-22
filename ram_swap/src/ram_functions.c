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
		int32_t pid_first_level_table_number = list_add(global_first_level_page_tables, first_level_page_table);
		pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

		log_info(logger, "create_pid_memory - Tabla de paginas de primer nivel con id: %d creada para el proceso %d", pid_first_level_table_number, process->pid);

		create_process_state(pid_first_level_table_number, process);

		return pid_first_level_table_number;
	}

	log_error(logger, "create_pid_memory - Tabla de paginas de primer nivel ya existe para proceso: %d", process->pid);

	return -1;


}

int32_t process_total_pages_needed(int32_t tamanioProceso){
    return (int32_t)ceil((double)tamanioProceso / (double)tam_pagina);
}

int32_t process_complete_second_level_table_pages(total_pages_needed){
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

void process_state(int32_t pid_first_level_table_number, t_process* process){

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






