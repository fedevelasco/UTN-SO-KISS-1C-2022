#include <ram_functions.h>

bool create_process_in_memory(t_process* process) {

	first_level_page_table_t* first_level_page_table;

	if(process_table_not_exists(process->pid)){

		log_info(logger, "Tabla de primer nivel no existe para pid: %d. Creando una nueva.", process->pid);


		first_level_page_table = malloc(sizeof(first_level_page_table_t));
		first_level_page_table->pid = process->pid;
		first_level_page_table->entries = list_create(); //El maximo es entradas_por_tabla;

	} else {
		//TODO: Esto puede pasar? que me manden crear un proceso en memoria otra vez? lo unico que tienen que cambiar son los bit de precencia de las paginas.
		log_info(logger, "Tabla de primer nivel ya existe para pid: %d. Deberia ser un error esto", process->pid);
		first_level_page_table = get_process_table(process->pid);
	}

	for (int i = 0; i < config.entradas_por_tabla; i++){

		second_level_page_tables_t* second_level_page_table = create_empty_second_level_page_table();

		int32_t second_level_page_table_number = list_add(global_page_lists->second_level_page_tables, second_level_page_table);

		log_info(logger, "Se agrega tabla nivel 2 numero: %d a las entradas de tabla de nivel 1", second_level_page_table_number);
		second_level_table_number_t* second_level_table_number = malloc(sizeof(second_level_table_number_t));
		second_level_table_number->value = second_level_page_table_number;

		list_add(first_level_page_table->entries, second_level_table_number);

	}

	log_info(logger, "Se agrega tabla nivel 1 de pid: %d a tabla global", first_level_page_table->pid);

	list_add(global_page_lists->first_level_page_tables, first_level_page_table);

    return true;
}


bool process_table_not_exists(int32_t pid){
	int32_t size = list_size(global_page_lists->first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_page_lists->first_level_page_tables,i);
		if(table->pid == pid)
			return false;
	}
	return true;
}

first_level_page_table_t* get_process_table(int32_t pid){
	int32_t size = list_size(global_page_lists->first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_page_lists->first_level_page_tables,i);
		if(table->pid == pid)
			return table;
	}
	return 0;
}

second_level_page_tables_t* create_empty_second_level_page_table(){
	second_level_page_tables_t* second_level_page_table = malloc(sizeof(second_level_page_table));
	second_level_page_table->pages = list_create(); //El maximo es entradas_por_tabla;

	for (int i = 0; i < config.entradas_por_tabla; i++){
		page_t* page = create_empty_page();
		list_add(second_level_page_table->pages, page);
	}

	return second_level_page_table;
}

page_t* create_empty_page(){
	page_t* page = malloc(sizeof(page_t));
	page->frame_number=0;
	page->bit_P=false;
	page->bit_U=false;

	return page;
}

int32_t get_pid_first_level_table_number(int32_t pid){
	int32_t size = list_size(global_page_lists->first_level_page_tables);

	for(int i = 0;i<size;i++){
		first_level_page_table_t* table;
		table = list_get(global_page_lists->first_level_page_tables,i);
		if(table->pid == pid)
			return i;
	}
	return 0;
}





