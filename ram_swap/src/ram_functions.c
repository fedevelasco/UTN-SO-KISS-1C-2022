#include <ram_functions.h>


uint32_t create_pid_memory(t_process* process){

	if(process_table_not_exists(process->pid)){

		uint32_t total_pages_needed = process_total_pages_needed(process->process_size);
		uint32_t complete_second_level_tables =  process_complete_second_level_tables(total_pages_needed);
		uint32_t incomplete_second_level_table_pages = total_pages_needed % entradas_por_tabla;

		first_level_page_table_t* first_level_page_table = malloc(sizeof(first_level_page_table_t));
		first_level_page_table->pid = process->pid;
		first_level_page_table->first_level_entries = list_create();

		uint32_t swap_id = 0;

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

		uint32_t pid_first_level_table_number;
		pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
		pid_first_level_table_number = list_add(global_first_level_page_tables, first_level_page_table);
		pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

		log_info(logger, "create_pid_memory - Tabla de paginas de primer nivel con id: %d creada para el proceso %d", pid_first_level_table_number, process->pid);

		create_process_state(pid_first_level_table_number, process);

		return pid_first_level_table_number;
	}

	log_error(logger, "create_pid_memory - Tabla de paginas de primer nivel ya existe para proceso: %d", process->pid);

	exit(-1);


}

uint32_t process_total_pages_needed(uint32_t size){
    return (uint32_t)ceil((double)size / (double)tam_pagina);
}

uint32_t process_complete_second_level_tables(uint32_t total_pages_needed){
	return (uint32_t) floor ( (double) total_pages_needed / (double) entradas_por_tabla);
}

first_level_entries_t* create_first_level_entry(uint32_t second_level_entries, uint32_t process_pid, uint32_t *swap_id){
	second_level_page_table_t* second_level_page_table = create_second_level_table(second_level_entries, process_pid, swap_id);

    first_level_entries_t* first_level_entry = malloc(sizeof(first_level_entries_t));

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
    first_level_entry->second_level_table_id = list_add(global_second_level_page_tables, second_level_page_table);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

    log_info(logger, "create_first_level_entry - tabla segundo nivel creada para pid: %d agregada a lista global con id: %d", process_pid, first_level_entry->second_level_table_id );


    return first_level_entry;
}

second_level_page_table_t* create_second_level_table(uint32_t pages, uint32_t process_pid, uint32_t *swap_id){

	second_level_page_table_t* second_level_page_table = malloc(sizeof(second_level_page_table_t));
	second_level_page_table->pages = list_create();

    for(int i=0; i < pages; i++){
        page_t* page = create_second_level_page(process_pid, swap_id);
        list_add(second_level_page_table->pages, page);
    }
    log_info(logger, "create_second_level_table - tabla segundo nivel creada para pid: %d", process_pid);
    return second_level_page_table;
}

page_t* create_second_level_page(uint32_t process_pid, uint32_t *swap_id)
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

void create_process_state(uint32_t pid_first_level_table_number, t_process* process){

	process_state_t* process_state = malloc(sizeof(process_state_t));
	process_state->clock_pointer = 0;
	process_state->frames_used = 0;
	process_state->first_level_page_table_id = pid_first_level_table_number;

	pthread_mutex_lock(&MUTEX_PROCESS_EXTRA_INFO);
	dictionary_put(process_extra_info, string_itoa(process->pid), process_state);
	pthread_mutex_unlock(&MUTEX_PROCESS_EXTRA_INFO);
}

bool process_table_not_exists(uint32_t pid){
	uint32_t size = list_size(global_first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_first_level_page_tables,i);
		if(table->pid == pid)
			return false;
	}
	return true;
}


uint32_t suspend_pid(t_process* process){

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

        // free(page);
    }
}

void* read_frame(uint32_t frame_number){

    void* frame = malloc(tam_pagina);
    uint32_t offset = frame_number * tam_pagina;

    pthread_mutex_lock(&MUTEX_MEMORY);
    memcpy(frame, memory + offset, tam_pagina);
    pthread_mutex_unlock(&MUTEX_MEMORY);

    return frame;
}

uint32_t process_frames(uint32_t process_size){
    return (uint32_t)ceil((double)process_size / (double)tam_pagina);
}
uint32_t first_level_page(uint32_t swap_page_id){
    return (uint32_t)floor((double)swap_page_id / (double)entradas_por_tabla);
}
uint32_t second_level_page(uint32_t swap_page_id){
    return swap_page_id % entradas_por_tabla;
}


void free_memory(uint32_t first_level_table_number){
    log_info(logger, "Se borra de memoria la tabla de primer nivel: %d", first_level_table_number);

    pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
    first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, first_level_table_number);
    pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_ENTRY);
    list_iterate(first_level_page_table->first_level_entries, free_first_level_entry);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_ENTRY);
}

void free_first_level_entry(void* entry){
	uint32_t second_level_table_id = ((first_level_entries_t*) entry)->second_level_table_id;
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

uint32_t get_second_level_page_table(t_page_table_request* page_table_request){

    log_info(logger, "get_second_level_page_table - Inicio");


	log_info(logger, "Retardo de memoria %dms", retardo_memoria);
	usleep(retardo_memoria*1000);

    pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
    first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, page_table_request->table_number);
    pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    first_level_entries_t* first_level_entry = list_get(first_level_page_table->first_level_entries, page_table_request->entry_number);

    log_info(logger, "get_second_level_page_table - Fin");

    return first_level_entry->second_level_table_id;
}

uint32_t get_frame_number(t_page_table_request* page_table_request, bool isWrite){

		log_info(logger, "get_frame_number - Inicio");

		log_info(logger, "Retardo de memoria %dms", retardo_memoria);
		usleep(retardo_memoria*1000);

	    // pthread_mutex_lock(&MUTEX_SECOND_LEVEL_ENTRY);

			pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
				second_level_page_table_t* second_level_page_table = list_get(global_second_level_page_tables, page_table_request->table_number);
			pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

			page_t* page = list_get(second_level_page_table->pages, page_table_request->entry_number);

			pthread_mutex_lock(&MUTEX_PROCESS_EXTRA_INFO);
			process_state_t* process_state = dictionary_get(process_extra_info, string_itoa(page_table_request->pid));
			pthread_mutex_unlock(&MUTEX_PROCESS_EXTRA_INFO);

			//Pagina no esta presente en memoria
			if (!page->bit_P){

				if (process_state->frames_used < marcos_por_proceso){

					uint32_t frame = -1;

					//Busco primer frame libre en memoria
					pthread_mutex_lock(&MUTEX_OCCUPIED_FRAMES);

					do{
						frame++;
					} while (bitarray_test_bit(occupied_frames_bitarray, frame));

					bitarray_set_bit(occupied_frames_bitarray, frame);

					pthread_mutex_unlock(&MUTEX_OCCUPIED_FRAMES);

					page->frame_number = frame;
					process_state->frames_used++;

					//Busco pagina en swap y la escribo en frame libre de memoria
					void* swap_page_data = read_page_in_swap(page->swap_page_id, page_table_request->pid);
					write_page_in_memory(frame, swap_page_data);
					free(swap_page_data);

				} else {
					//Pagina presente en memoria. Busco victima para reemplazar.

					page_t* victim_page = find_victim_page_to_replace(process_state, page);

					//Si la pagina victima fue modificada, la llevo a swap.
					if (victim_page->bit_M){

						void* frame_data = read_frame(victim_page->frame_number);
						write_frame_in_swap(frame_data, victim_page->swap_page_id, page_table_request->pid);
						free(frame_data);

						//Le saco un frame de memoria utilizado al proceso que tuvo una pagina victima
						pthread_mutex_lock(&MUTEX_PROCESS_EXTRA_INFO);
						process_state_t* process_state = dictionary_get(process_extra_info, string_itoa(victim_page->pid));
						pthread_mutex_unlock(&MUTEX_PROCESS_EXTRA_INFO);

						process_state->frames_used--;

					}
					victim_page->bit_M = false;
					victim_page->bit_P = false;
					victim_page->bit_U = false;

					page->frame_number = victim_page->frame_number;

					//Escribo en memoria la pagina traida de swap que reemplazo a la victima
					void* swap_page_data = read_page_in_swap(page->swap_page_id, page_table_request->pid);
					write_page_in_memory(page->frame_number, swap_page_data);
					free(swap_page_data);
				}
				page->bit_P = true;
			}

			if (isWrite){
				page->bit_M = true;
				log_info(logger, "Al pid: %d se le asigna el frame: %d para Escritura", page->frame_number, page_table_request->pid);
			} else {
				log_info(logger, "Al pid: %d se le asigna el frame: %d para Lectura", page->frame_number, page_table_request->pid);
			}

			page->bit_U = true;


	    // pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_ENTRY);

	    log_info(logger, "get_frame_number - Fin");
	    return page->frame_number;

}

void write_page_in_memory(uint32_t frame, void* swap_page_data){

	uint32_t offset = frame * tam_pagina;

    pthread_mutex_lock(&MUTEX_MEMORY);
    memcpy(memory + offset, swap_page_data, tam_pagina);
    pthread_mutex_unlock(&MUTEX_MEMORY);
}

page_t* find_victim_page_to_replace(process_state_t* process_state, page_t* page){

    page_t* victim_page = NULL;

    pthread_mutex_lock(&MUTEX_SECOND_LEVEL_ENTRY);
    t_list* all_process_pages_list = get_second_level_pages(process_state->first_level_page_table_id);
    pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_ENTRY);

    //TODO: Revisar estos filtros
    bool filtroEntradasSegundoNivel(page_t* entradaFilter){
		return entradaFilter->bit_P;
	}

	t_list *listaEntradas = list_filter(all_process_pages_list, (void *) filtroEntradasSegundoNivel);

	bool compararEntradasSegundoNivel(page_t* entrada, page_t* entrada2){
		return entrada->frame_number < entrada2->frame_number;
	}

	list_sort(listaEntradas, (void *) compararEntradasSegundoNivel);



    if (string_equals_ignore_case(algoritmo_reemplazo, "CLOCK")){

    	victim_page = replace_with_clock(process_state, listaEntradas, page);

    } else if (string_equals_ignore_case(algoritmo_reemplazo, "CLOCK-M")){

    	victim_page = replace_with_clock_m(process_state, listaEntradas, page);

    } else {

        log_error(logger, "El algoritmo de reemplazo: %s no esta disponible", algoritmo_reemplazo);
        exit(-1);
    }
    list_destroy(all_process_pages_list);
    list_destroy(listaEntradas);
    return victim_page;
}

t_list* get_second_level_pages(uint32_t first_level_page_table_id)
{
    pthread_mutex_lock(&MUTEX_FIRST_LEVEL_TABLES);
    first_level_page_table_t* first_level_page_table = list_get(global_first_level_page_tables, first_level_page_table_id);
    pthread_mutex_unlock(&MUTEX_FIRST_LEVEL_TABLES);

    t_list* all_process_pages_list = list_create();

    for(int i=0; i < list_size(first_level_page_table->first_level_entries);i++){

    	first_level_entries_t* entry = list_get(first_level_page_table->first_level_entries, i);

    	pthread_mutex_lock(&MUTEX_SECOND_LEVEL_TABLES);
    	second_level_page_table_t* second_level_page_table = list_get(global_second_level_page_tables, entry->second_level_table_id);
    	pthread_mutex_unlock(&MUTEX_SECOND_LEVEL_TABLES);

    	for(int j=0; j < list_size(second_level_page_table->pages);j++){
    		page_t* page = list_get(second_level_page_table->pages, j);

    		list_add(all_process_pages_list, page);
    	}
    }


    return all_process_pages_list;
}

page_t* replace_with_clock(process_state_t* process_state, t_list* all_process_pages_list, page_t* page){
	log_info(logger, "Reemplazando pagina con algoritmo CLOCK - Inicio");

//	uint32_t pages_number = list_size(all_process_pages_list);

	page_t* victim_page;

	//TODO: Revisar
//	log_info(logger, "------Estado inicial antes de reemplazo------");
//	    log_info(logger, "PUNTERO: %d", process_state->clock_pointer);
//	    void imprimirEstadoInicial(page_t* page)
//	    {
//	        log_info(logger, "PAGINA:(%d,%d) - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d",
//	                 first_level_page(page->swap_page_id),
//	                 second_level_page(page->swap_page_id),
//	                 page->frame_number,
//	                 page->bit_U,
//	                 page->bit_P);
//	    }
//
//	    list_iterate(all_process_pages_list, (void *)imprimirEstadoInicial);

	page_t* pointer;

	log_info(logger, "Reemplazando pagina con algoritmo CLOCK - Buscando pagina victima presente con bit uso=0");
	while (true) {

		pointer = list_get(all_process_pages_list, process_state->clock_pointer);

		//Sumo el puntero 1 por cada loop y reinicia a 0 cuando supera la cantidad de paginas.
		process_state->clock_pointer = (process_state->clock_pointer + 1) % marcos_por_proceso;
		log_info(logger, "Avanzo puntero a: %d", process_state->clock_pointer);


		//Si pagina puntero esta presente pero no esta en uso -> Reemplazo
		if (pointer->bit_P && !pointer->bit_U) {

			victim_page = pointer;

			log_info(logger,
					"Usando algoritmo: %s - se reemplaza pagina de swap victima : [%d,%d] por pagina de swap: [%d,%d]  - Ingresa a memoria en frame:%d",
					algoritmo_reemplazo, first_level_page(victim_page->swap_page_id), second_level_page(victim_page->swap_page_id),
					first_level_page(page->swap_page_id), second_level_page(page->swap_page_id), victim_page->frame_number);

			break;

		} else {
			//Saco bit de uso y vuelvo a recorrer
			pointer->bit_U = false;
			log_info(logger, "Se cambia bit uso = 0 para pagina:[%d,%d]", first_level_page(pointer->swap_page_id), second_level_page(pointer->swap_page_id));

		}

		if(!pointer->bit_U){
			log_info(logger, "La pagina: [%d,%d] tiene bit de presencia en: %d. Paso a la proxima pagina.", first_level_page(pointer->swap_page_id), second_level_page(pointer->swap_page_id), pointer->bit_P);
		}
	}

	log_info(logger, "Reemplazando pagina con algoritmo CLOCK - Fin");
	return victim_page;
}

page_t* replace_with_clock_m(process_state_t* process_state, t_list* all_process_pages_list, page_t* page){
	log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Inicio");

	//TODO: Uso marcos_por_proceso. Revisar cual usar.
//	uint32_t pages_number = list_size(all_process_pages_list);

	page_t* victim_page;


	//    void print_use_bit(page_t* page){
		//        if (page->bit_M)
		//            log_debug(logger, "INDICE SWAP: %d - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d", page->swap_page_id, page->frame_number, page->bit_U, page->bit_P);
		//    }
		//
		//    list_iterate(all_process_pages_list, (void *) imprimirBitsUso);

	page_t* pointer;
	uint32_t pointer_start = process_state->clock_pointer;

	bool completed = false;

	//Primera vuelta: busco USO=0 y MODIFICADO=0 sin poner USO=0
	log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Buscando pagina victima presente con U=0 y M=0");
	while (true){

		pointer = list_get(all_process_pages_list, process_state->clock_pointer);

		//Sumo el puntero 1 por cada loop y reinicia a 0 cuando supera la cantidad de paginas.
		process_state->clock_pointer = (process_state->clock_pointer + 1) % marcos_por_proceso;

		if (pointer->bit_P && !(pointer->bit_M || pointer->bit_U) && !completed){

			victim_page = pointer;

			log_info(logger,
					"Usando algoritmo: %s - se reemplaza pagina de swap victima :%d por pagina de swap:%d  - Ingresa a memoria en frame:%d",
					algoritmo_reemplazo, victim_page->swap_page_id,
					page->swap_page_id, victim_page->frame_number);

			completed = true;

			break;
		}

		//Si ya recorri todas las paginas, salgo y hago el siguiente intento diferente
		if(process_state->clock_pointer == pointer_start) break;
	}

	if(!completed){
		log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Segundo intento - Buscando pagina victima presente con U=0 y M=1");
	}


	//Segunda vuelta: busco USO=0 y MODIFICADO=1 poniendo USO=0
	while (true){

		pointer = list_get(all_process_pages_list, process_state->clock_pointer);

		//Sumo el puntero 1 por cada loop y reinicia a 0 cuando supera la cantidad de paginas.
		process_state->clock_pointer = (process_state->clock_pointer + 1) % marcos_por_proceso;

		if (pointer->bit_P && !pointer->bit_U && pointer->bit_M && !completed) {

			victim_page = pointer;

			log_info(logger,
					"REEMPLAZO EXITOSO - Usando algoritmo: %s - se reemplaza pagina de swap :%d por pagina de swap:%d  - Ingresa a memoria en frame:%d",
					algoritmo_reemplazo, victim_page->swap_page_id,
					page->swap_page_id, victim_page->frame_number);

			completed = true;

			break;
		} else {
			//Saco bit de uso y vuelvo a recorrer
			pointer->bit_U = false;

		}

		//Si ya recorri todas las paginas, salgo y hago el siguiente intento diferente
		if(process_state->clock_pointer == pointer_start) break;
	}

	if(!completed){
		log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Tercer Intento - Buscando pagina victima presente con U=0 y M=0");

	}
	//Tercera vuelta - paso 1 otra vez - busco USO=0 y MODIFICADO=0 sin poner USO=0
	//Ahora deberia encontrar victima, por que si estaban en USO=1 se pasaron a USO=0

	while (true){

		pointer = list_get(all_process_pages_list, process_state->clock_pointer);

		//Sumo el puntero 1 por cada loop y reinicia a 0 cuando supera la cantidad de paginas.
		process_state->clock_pointer = (process_state->clock_pointer + 1) % marcos_por_proceso;

		if (pointer->bit_P && !(pointer->bit_M || pointer->bit_U) && !completed){

			victim_page = pointer;

			log_info(logger,
					"REEMPLAZO EXITOSO - Usando algoritmo: %s - se reemplaza pagina de swap :%d por pagina de swap:%d  - Ingresa a memoria en frame:%d",
					algoritmo_reemplazo, victim_page->swap_page_id,
					page->swap_page_id, victim_page->frame_number);

			completed = true;

			break;
		}

		//Si ya recorri todas las paginas, salgo y hago el siguiente intento diferente
		if(process_state->clock_pointer == pointer_start) break;
	}

	if(!completed){
		log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Ultimo intento - Buscando pagina victima presente con U=0 y M=1");
	}

	//Cuarta vuelta: busco USO=0 y MODIFICADO=1 poniendo USO=0
	//Ahora si o si tiene que encontrar una victima.
	while (true){

		pointer = list_get(all_process_pages_list, process_state->clock_pointer);

		//Sumo el puntero 1 por cada loop y reinicia a 0 cuando supera la cantidad de paginas.
		process_state->clock_pointer = (process_state->clock_pointer + 1) % marcos_por_proceso;

		if (pointer->bit_P && !pointer->bit_U && pointer->bit_M && !completed) {

			victim_page = pointer;

			log_info(logger,
					"REEMPLAZO EXITOSO - Usando algoritmo: %s - se reemplaza pagina de swap :%d por pagina de swap:%d  - Ingresa a memoria en frame:%d",
					algoritmo_reemplazo, victim_page->swap_page_id,
					page->swap_page_id, victim_page->frame_number);

			completed = true;

			break;
		} else {
			//Saco bit de uso y vuelvo a recorrer
			pointer->bit_U = false;
			break;
		}
	}

	log_info(logger, "Reemplazando pagina con algoritmo CLOCK-M - Fin");
	return victim_page;
}


uint32_t read_fisical_address(uint32_t fisical_address){

	log_info(logger, "read_fisical_address - Leyendo direccion fisica - Inicio");

	log_info(logger, "Retardo de memoria %dms", retardo_memoria);
	usleep(retardo_memoria*1000);

    uint32_t data;
//    uint32_t data = malloc(sizeof(uint32_t));

    pthread_mutex_lock(&MUTEX_MEMORY);
    memcpy(&data, memory + fisical_address, sizeof(uint32_t));
    pthread_mutex_unlock(&MUTEX_MEMORY);
    log_info(logger, "LECTURA - En la direccion fisica: %d se LEE el dato: %d", fisical_address, data);
    log_info(logger, "read_fisical_address - Leyendo direccion fisica - Fin");
    return data;
}

void write_memory_data(t_memory_write_request* write){

	log_info(logger, "write_memory_data - Escribiendo memoria - Inicio");

	log_info(logger, "Retardo de memoria %dms", retardo_memoria);
	usleep(retardo_memoria*1000);

	uint32_t data = write->data;

    pthread_mutex_lock(&MUTEX_MEMORY);
    memcpy(memory + write->fisical_address, &data, sizeof(uint32_t));
    pthread_mutex_unlock(&MUTEX_MEMORY);

    log_info(logger, "ESCRITURA - En la direccion fisica: %d se ESCRIBE el dato: %d", write->fisical_address, data);
	log_info(logger, "write_memory_data - Escribiendo memoria - Fin");
}



