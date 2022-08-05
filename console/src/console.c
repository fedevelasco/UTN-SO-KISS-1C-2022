#include <console.h>

int main(int32_t argc, char** argv){

	/* ---------------- LOGGING ---------------- */
	t_log* logger;
	logger = start_logger();

	if(argc < 2) {
		log_error(logger, "Error en argumentos - Es obligatorio especificar path y tamanio de proceso - Fin proceso");
		return EXIT_FAILURE;
	}


	uint32_t connection;


	char* ip_config_path = argv[3];


	char* ip;
	char* port;
	char *psudocode_file_path;


	uint32_t process_size;


	t_config* config;
	t_instructions_list* instructions_list;


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, "Carga de archivo de configuracion - Inicio");
	config = load_configuration_file(ip_config_path,logger);

	// Loggeamos el valor de config
	ip = config_get_string_value(config, "IP_KERNEL");
	port = config_get_string_value(config, "PUERTO_KERNEL");

	log_info(logger, "El kernel configurado tiene ip: %s:%s", ip, port);
	log_info(logger, "Carga de archivo de configuracion - Fin");

	/* ---------------- ARCHIVO DE PSEUDOCODIGO ---------------- */

	log_info(logger, "Carga de archivo de pseudocodigo - Inicio");
	psudocode_file_path = argv[1];
	if (strcmp(psudocode_file_path, "") == 0){
		log_error(logger, "Carga de archivo de pseudocodigo - La ruta del archivo de pseudocodigo esta vacia - Fin proceso");
		return EXIT_FAILURE;
	}

	instructions_list = parse_pseudocode_file(psudocode_file_path, logger);
	if (instructions_list == NULL){
		log_error(logger, "Carga de archivo de pseudocodigo - Error al cargar el archivo de pseudocodigo - Fin proceso");
		return EXIT_FAILURE;
		}

	log_info(logger, "Carga de archivo de pseudocodigo - Fin");


	/* ---------------- TAMANIO DE PROCESO -------------------- */

	log_info(logger, "Carga de tamanio de proceso - Inicio");
	char *process_size_string = argv[2];
	if (strcmp(process_size_string, " ") == 0){
			log_error(logger, "Carga de tamanio de proceso - El tamanio de proceso esta vacio - Fin proceso");
			return EXIT_FAILURE;
		}
	process_size = atoi(process_size_string);
	log_info(logger, "Carga de tamanio de proceso - Tamanio de proceso leido: %i", process_size);
	instructions_list->process_size = process_size;
	log_info(logger, "Carga de tamanio de proceso - Fin");

	/* ---------------- CONEXION AL KERNEL -------------------- */

	log_info(logger, "Conexion a Kernel - Inicio");

	// Creo una conexión hacia el kernel
	log_info(logger, "Conexion a Kernel - Creando conexion a kernel");
	connection = create_connection(ip, port);
	if (connection == -1){
			log_error(logger, "Conexion a Kernel - Error al crear conexion - Fin proceso");
			return EXIT_FAILURE;
		}

	log_debug(logger, "Conexion a Kernel - create_connection - server_socket: %i", connection);

	log_info(logger, "Conexion a Kernel - Creando instrucction buffer");
	t_buffer* instructions_buffer = new_instruction_buffer(instructions_list, logger);
	if (instructions_buffer == NULL){
		log_error(logger, "Conexion a Kernel - Error al crear instructions_buffer - Fin proceso");
		return EXIT_FAILURE;
	}

	log_info(logger, "Conexion a Kernel - Creando paquete de instrucciones");
	t_package* instructions_package = new_package(instructions_buffer, INSTRUCTIONS);
	if (instructions_package == NULL){
			log_error(logger, "Conexion a Kernel - Error al crear instructions_package - Fin proceso");
			return EXIT_FAILURE;
		}

	// Envio paquete
	log_info(logger, "Conexion a Kernel - Envio de paquete");
	if(send_to_server(connection, instructions_package) == -1){
		log_error(logger, "Conexion a Kernel - Error al enviar paquete al servidor - Fin proceso");
		return EXIT_FAILURE;
	}


	//Espero mensaje de finalizacion del kernel
	log_info(logger, "Conexion a Kernel - Consola esperando mensaje de finalizacion del Kernel");

	t_paquete * package = wait_package(connection);

	log_info(logger, "Llego paquete de kernel codop: %d", package->codigo_operacion);
	if (package->codigo_operacion == EXIT_CONSOLE){
		log_info(logger, "Conexion a Kernel - Finalizacion exitosa");
		end_process(connection, logger, config);

	}



}



