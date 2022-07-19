#include <ram_functions.h>

static bool create_process_in_memory(t_process* process) {

	if(process_table_not_exists(process->pid)){

		first_level_page_table_t* first_level_page_table = malloc(sizeof(first_level_page_table_t));
		first_level_page_table->pid = process->pid;
		first_level_page_table->entries = list_create();
		}

	int32_t process_pages_amount = process->process_size / config.tam_pagina;

	for(int i = 0; i < process_pages_amount; i++ ){

	}


	second_level_page_number_t* second_level_page_number = next_second_level_page;

	for (int i = 0; i < config.entradas_por_tabla; i++){
		list_add(first_level_page_table->table, )
		dictionary_put(, )
	}

	int i;
		for (i = 0; i < cantPaginasPedidas; i++) {
			tablaPagina_t *nuevaPag = malloc(sizeof(tablaPagina_t));
			int posicion;
			if (existePidEnListadeTablas(pid)) {
				posicion = primerNumeroPaginaLibre(pid);
			} else {
				posicion = i;
			}

			pthread_mutex_lock(&lock_accesoTabla);
			nuevaPag->nroPagina = posicion;
			nuevaPag->marcoUtilizado = -1;
			nuevaPag->bitPresencia = 0;
			nuevaPag->bitModificacion = 0;
			nuevaPag->bitUso = 0;

			tabla_t* tablaPag = buscarTabla(pid);
			list_add_in_index((t_list*)tablaPag->listaPaginas, posicion, nuevaPag);
			pthread_mutex_unlock(&lock_accesoTabla);
		}
		return 1;


    return true;
}


bool process_table_not_exists(int32_t pid){
	int size = list_size(global_page_lists->first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_page_lists->first_level_page_tables,i);
		if(table->pid == pid)
			return false;
	}
	return true;
}



