#include <ram.h>

int main(int argc, char* argv[]){

	/* ---------------- LOGGING ---------------- */

	logger = start_logger();

	if(argc < 2) {
			log_error(logger, "Error en argumentos - Es obligatorio especificar path de config y de ips - Fin proceso");
			//end_process();
			return EXIT_FAILURE;
		}

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, "Carga de archivo de configuracion - Inicio");

	char* config_path = argv[1];
	char* ip_config_path = argv[2];

	if(load_ram_configuration_file(config_path, ip_config_path)){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "La Ram configurada tiene escucha en: %s : %s, con tamanio: %d y algoritmo: %s", ip, puerto_escucha, tam_memoria, algoritmo_reemplazo);
	log_info(logger, "Carga de archivo de configuracion - Fin");


	/* ---------------- LOCKS Y MUTEX -------------------- */

	log_info(logger, "Creacion de mutex - Inicio");

	if(mutex_init()){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Creacion de mutex - Fin");

	/* ---------------- INICIO MEMORIA -------------------- */

	log_info(logger, "Creacion de memoria - Inicio");

	if(memory_create()){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Creacion de memoria - Fin");

	/* ---------------- INICIO PAGINACION -------------------- */

	log_info(logger, "Inicio de paginacion - Inicio");

	if(paging_tables_create()){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Inicio de paginacion - Fin");

	/* ---------------- INICIO SWAP -------------------- */

		log_info(logger, "Inicio de swap - Inicio");

		if(swap_init()){
			//end_process();
			return EXIT_FAILURE;
		}

		log_info(logger, "Inicio de swap - Fin");

	/* ---------------- CREACION SERVER -------------------- */

	log_info(logger, "Inicio de elementos auxiliares - Inicio");

	if(aux_elements_init()){
		//end_process();
		return EXIT_FAILURE;
	}
	log_info(logger, "Inicio de elementos auxiliares - Fin");

	log_info(logger, "Inicio de threads - Inicio");

	if(threads_init()){
		//end_process();
		return EXIT_FAILURE;
	}
	log_info(logger, "Inicio de threads - Fin");


	log_info(logger, "Inicio de server - Inicio");

	server_socket = iniciar_servidor(logger, SERVERNAME, ip, puerto_escucha);

    log_info(logger, "Inicio de server - Fin");


	/* ---------------- ESPERAR CLIENTES Y PROCESAR CONEXION  -------------------- */

    log_info(logger, "Esperando clientes - Inicio");
	server_listen_ram(SERVERNAME, server_socket);

	/* ---------------- LIBERO CONEXIONES Y FINALIZO  -------------------- */

	liberar_conexion(server_socket);

	close_process();

}



