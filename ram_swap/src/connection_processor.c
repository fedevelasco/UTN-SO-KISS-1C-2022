#include <connection_processor.h>

void process_init(operation_buffer_t* operation_buffer) {

	log_info(logger, "process_init - Iniciando proceso en memoria - Inicio");

	t_process* process = create_process();
	deserialize_process(process, operation_buffer->buffer);

	log_info(logger, "process_init - Creando estructuras para pid: %d", process->pid);

	uint32_t pid_first_level_table_number = create_pid_memory(process);
	if(pid_first_level_table_number == -1){
		log_error(logger, "Error creando proceso en memoria. Ya existe");
		process_destroy(process);
	}

	log_info(logger, "process_init - Creando archivo swap para pid: %d", process->pid);

	if(swap_create(process->pid, process->process_size)){
		log_error(logger, "process_init - Error creando swap");
		process_destroy(process);
	}

	log_info(logger, "process_init - Enviando numero de tabla del proceso a Kernel: %d", pid_first_level_table_number);


	if(send_process_init(operation_buffer->client_socket, pid_first_level_table_number)) {
		log_error(logger, "Error enviando numero de tabla del proceso a Kernel");
		process_destroy(process);
	}

	log_debug(logger, "process_init - se envio el paquete");

	process_destroy(process);

	log_info(logger, "process_init - Iniciando proceso en memoria - Fin");

}

void process_suspend(operation_buffer_t* operation_buffer){

	log_info(logger, "process_suspend - Suspendiendo proceso - Inicio");

	t_process* process = create_process();
	deserialize_process(process, operation_buffer->buffer);

	log_info(logger, "process_suspend - Se suspende el proceso: %d", process->pid);

	if(suspend_pid(process)){
		log_error(logger, "Error suspendiendo proceso");
		process_destroy(process);
	}

	free_memory(process->first_level_table_number);


	if(send_process_suspend(operation_buffer->client_socket)) {
			log_error(logger, "Error enviando confirmacion de suspension de proceso a Kernel");
			process_destroy(process);
		}


	process_destroy(process);
	log_info(logger, "process_suspend - Suspendiendo proceso - Fin");

}

void process_kill(operation_buffer_t* operation_buffer){

	log_info(logger, "process_kill - Eliminando proceso de memoria y swap - Inicio");

	t_process* process = create_process();
	deserialize_process(process, operation_buffer->buffer);

	log_info(logger, "process_kill - Eliminando proceso: %d", process->pid);

	free_memory(process->first_level_table_number);

	free_process_swap(process->pid);

	process_destroy(process);


	log_info(logger, "process_kill - Eliminado proceso de memoria y swap- Fin");

}

void get_memory_config(operation_buffer_t* operation_buffer){

	log_info(logger, "get_memory_config - Enviar configuracion de memoria a cpu - Inicio");

	t_memory_config* memory_config = create_memory_config();
	memory_config->page_size = tam_pagina;
	memory_config->pages_per_table = entradas_por_tabla;

	log_debug(logger, "get_memory_config - page_size=%d pages_per_table=%d", memory_config->page_size, memory_config->pages_per_table);

	if(send_memory_config(operation_buffer->client_socket, memory_config)) {
		log_error(logger, "Error enviando configuracion de memoria a cpu");
		memory_config_destroy(memory_config);
	}

	memory_config_destroy(memory_config);

	log_info(logger, "get_memory_config - Enviar configuracion de memoria a cpu - Fin");
}

void get_second_level_table(operation_buffer_t* operation_buffer){

	log_info(logger, "get_second_level_table - Enviar tabla de segundo nivel de memoria a cpu - Inicio");

	t_page_table_request* page_table_request = create_page_table_request();
	deserialize_page_table_request(page_table_request, operation_buffer->buffer);

	uint32_t second_level_table_id = get_second_level_page_table(page_table_request);

	if(send_second_level_table_id(operation_buffer->client_socket, second_level_table_id)) {
		log_error(logger, "Error enviando numero de tabla de segundo nivel del proceso a cpu");
		page_table_request_destroy(page_table_request);
	}

	page_table_request_destroy(page_table_request);

	log_info(logger, "get_second_level_table - Enviar tabla de segundo nivel de memoria a cpu - Fin");
}

void get_frame_read(operation_buffer_t* operation_buffer){

	log_info(logger, "get_frame_read - Enviar frame de lectura memoria a cpu - Inicio");

	t_page_table_request* page_table_request = create_page_table_request();
	deserialize_page_table_request(page_table_request, operation_buffer->buffer);

	uint32_t frame_number = get_frame_number(page_table_request, false);

	if(send_frame_number_read(operation_buffer->client_socket, frame_number)) {
		log_error(logger, "Error enviando numero de framede lectura del proceso a cpu");

	}

	log_info(logger, "get_frame_read - Enviar frame de lectura memoria a cpu - Fin");


}

void get_frame_write(operation_buffer_t* operation_buffer){

	log_info(logger, "get_frame_write - Enviar frame de escritura memoria a cpu - Inicio");

	t_page_table_request* page_table_request = create_page_table_request();
	deserialize_page_table_request(page_table_request, operation_buffer->buffer);

	uint32_t frame_number = get_frame_number(page_table_request, true);

	if(send_frame_number_write(operation_buffer->client_socket, frame_number)) {
		log_error(logger, "Error enviando numero de frame de escritura del proceso a cpu");

	}

	log_info(logger, "get_frame_write - Enviar frame de escritura memoria a cpu - Fin");


}

void read_memory(operation_buffer_t* operation_buffer){

	log_info(logger, "read_memory - Enviar frame leido a cpu - Inicio");

	uint32_t fisical_address;

	deserialize_int(&fisical_address, operation_buffer->buffer);

	uint32_t memory_data = read_fisical_address(fisical_address);

	if(send_memory_data(operation_buffer->client_socket, memory_data)) {
		log_error(logger, "Error enviando numero de frame de escritura del proceso a cpu");

	}

	log_info(logger, "read_memory - Enviar frame leido a cpu - Fin");

}

void write_memory(operation_buffer_t* operation_buffer){

	log_info(logger, "write_memory - Escribir dato en memoria - Inicio");

	t_memory_write_request* write = create_memory_write_request();
	deserialize_memory_write_request(write, operation_buffer->buffer);

   write_memory_data(write);

   if(send_write_memory(operation_buffer->client_socket)) {
   		log_error(logger, "Error enviando numero de frame de escritura del proceso a cpu");
   		free(write);
   	}

   free(write);


}



