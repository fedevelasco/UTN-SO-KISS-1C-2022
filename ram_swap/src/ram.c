#include <ram.h>

int main(void){

	/* ---------------- LOGGING ---------------- */
	t_log* logger;
	logger = start_logger();


	t_config* config_to_load;

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, "Carga de archivo de configuracion - Inicio");

	if(!load_configuration_file(config_to_load, logger)){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "La Ram configurada tiene escucha en: %d, con tamanio: %d y algoritmo: %s", config.puerto_escucha, config.tam_memoria, config.algoritmo_reemplazo);
	log_info(logger, "Carga de archivo de configuracion - Fin");


	/* ---------------- LOCKS Y MUTEX -------------------- */

	log_info(logger, "Creacion de locks y mutex - Inicio");

	if(!set_thread_attributes()){
		//end_process();
		return EXIT_FAILURE;
	}

	//TODO: Definir los mutex que tengo que usar y los inicializo aca.
	//(El primer numero es la cantidad de mutex que voy a inicializar)
	if(!mutex_init(3, &MUTEX_CLIENT, &MUTEX_SWAP, &LOCK_ACCESS_RAM)){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Creacion de locks y mutex - Fin");

	/* ---------------- INICIO MEMORIA -------------------- */

	log_info(logger, "Creacion de memoria - Inicio");

	if(!memory_create(config, logger)){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Creacion de memoria - Fin");

	/* ---------------- INICIO PAGINACION -------------------- */

	log_info(logger, "Inicio de paginacion - Inicio");

	if(!paging_create(config, logger)){
		//end_process();
		return EXIT_FAILURE;
	}

	log_info(logger, "Inicio de paginacion - Fin");

	/* ---------------- CREACION SERVER -------------------- */


	//TODO: Ver como pasarle la ip posta de la vm.
    server_fd = server_init(logger, SERVERNAME, "0.0.0.0", config.puerto_escucha);


	/* ---------------- ESPERAR CLIENTES Y PROCESAR CONEXION  -------------------- */

	while (server_listen_ram(SERVERNAME, server_fd));

	/* ---------------- LIBERO CONEXIONES Y FINALIZO  -------------------- */

	liberar_conexion(&server_fd);

	close_process();

}



