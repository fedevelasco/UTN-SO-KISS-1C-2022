#include "console.h"

int main(int32_t argc, char** argv){

	/* ---------------- LOGGING ---------------- */
	t_log* logger;
	logger = start_logger();

	if(argc < 2) {
		log_error(logger, "Error en argumentos - Es obligatorio especificar path y tamanio de proceso");
		return EXIT_FAILURE;
	}


	int32_t connection;
	char* ip;
	char* port;
	char *psudocode_file_path;


	int32_t process_size;


	t_config* config;
	t_instructions_list* instructions_list;


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, "Carga de archivo de configuracion - Inicio");
	config = load_configuration_file(logger);

	// Loggeamos el valor de config
	ip = config_get_string_value(config, "KERNEL_IP");
	port = config_get_string_value(config, "KERNEL_PORT");

	log_info(logger, "El kernel configurado tiene ip: %s:%s", ip, port);
	log_info(logger, "Carga de archivo de configuracion - Fin");

	/* ---------------- ARCHIVO DE PSEUDOCODIGO ---------------- */

	log_info(logger, "Carga de archivo de pseudocodigo - Inicio");
	psudocode_file_path = argv[2];
	if (strcmp(psudocode_file_path, "") == 0){
		log_error(logger, "Carga de archivo de pseudocodigo - La ruta del archivo de pseudocodigo esta vacia");
		return EXIT_FAILURE;
	}

	instructions_list = parse_pseudocode_file(psudocode_file_path, logger);
	if (instructions_list == NULL){
		log_error(logger, "Carga de archivo de pseudocodigo - Error al cargar el archivo de pseudocodigo");
		return EXIT_FAILURE;
		}

	log_info(logger, "Carga de archivo de pseudocodigo - Fin");


	/* ---------------- TAMANIO DE PROCESO -------------------- */

	log_info(logger, "Carga de tamanio de proceso - Inicio");
	char *process_size_string = argv[1];
	if (strcmp(process_size_string, " ") == 0){
			log_error(logger, "Carga de tamanio de proceso - El tamanio de proceso esta vacio");
			return EXIT_FAILURE;
		}
	process_size = atoi(process_size_string);
	log_info(logger, "Carga de tamanio de proceso - Tamanio de proceso leido: %i", process_size);
	instructions_list->process_size = process_size;
	log_info(logger, "Carga de tamanio de proceso - Fin");

	/* ---------------- CONEXION AL KERNEL -------------------- */

	log_info(logger, "Conexion a Kernel - Inicio");

	// Creo una conexión hacia el kernel
	connection = create_connection(logger, "Kernel", ip, port);

	log_debug(logger, "Conexion a Kernel - create_connection - server_socket: %i", connection);

	t_buffer* instructions_buffer = create_instruction_buffer(instructions_list);
	if (instructions_buffer == NULL){
		log_error(logger, "Conexion a Kernel - Error al crear instructions_buffer");
		return EXIT_FAILURE;
	}

	t_package* instructions_package = create_instructions_package(instructions_buffer);
	if (instructions_package == NULL){
			log_error(logger, "Conexion a Kernel - Error al crear instructions_package");
			return EXIT_FAILURE;
		}

	// Envio paquete
	if(send_to_server(connection, instructions_package) == -1){
		log_error(logger, "Conexion a Kernel - Error al enviar paquete al servidor");
		return EXIT_FAILURE;
	}

	//Espero mensaje de finalizacion del kernel
	while(1){
		int cod_op = receive_operation_code(connection);
		if (cod_op == 1){
			end_process(connection, logger, config);
		}
	}

}






t_log* start_logger(void)
{
	t_log* new_logger;

	new_logger = log_create("console.log", "Console", 1, LOG_LEVEL_DEBUG);

	if(new_logger == NULL){
		printf("No fue posible crear el Logger\n");
		exit(1);
	}

	return new_logger;
}

t_config* load_configuration_file(t_log* logger)
{
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Inicio");
	t_config* new_configuration;

	new_configuration = config_create("../console/console.config");

	if (new_configuration == NULL){
		log_error(logger, "Carga de archivo de configuracion - load_configuration_file - Error al crear configuracion");
		exit(2);
	}
	log_info(logger, "Carga de archivo de configuracion - load_configuration_file - Config en %s cargada ok", new_configuration->path);
	return new_configuration;
}


void end_process(int32_t connection, t_log* logger, t_config* config)
{

	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}

    close(connection);


}
