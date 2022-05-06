#include "console.h"

int main(int8_t argc, char** argv){

	if(argc < 2) {
	        return EXIT_FAILURE;
	    }


	int8_t conexion;
	char* ip;
	char* port;
	char *psudocode_file_path;


	int8_t process_size;

	t_log* logger;
	t_config* config;
	t_instructions_list* instructions_list;


	/* ---------------- LOGGING ---------------- */

	logger = start_logger();
	log_info(logger, "Soy un logg");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = load_configuration_file();

	// Loggeamos el valor de config
	ip = config_get_string_value(config, "KERNEL_IP");
	port = config_get_string_value(config, "KERNEL_PORT");

	log_info(logger, port);

	/* ---------------- ARCHIVO DE PSEUDOCODIGO ---------------- */

	psudocode_file_path = argv[2];

	instructions_list = parse_pseudocode_file(psudocode_file_path);

	char *file_get_string_value(t_pseudocode *self, int index) {
		return list_get(self->lines, index);
	}






	/* ---------------- TAMANIO DE PROCESO ---------------- */

	char *a = argv[1];
	process_size = atoi(a);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------CONEXION AL SERVIDOR---------------------------------------------------*/

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(logger, "Server", ip, port);


	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

t_log* start_logger(void)
{
	t_log* new_logger;

	new_logger = log_create("tp0.log", "Client", 1, LOG_LEVEL_INFO);

	if(new_logger == NULL){
		printf("No fue posible crear el Logger\n");
		exit(1);
	}

	return new_logger;
}

t_config* load_configuration_file(void)
{
	t_config* new_configuration;

	new_configuration = config_create("../console/console.config");

	if (new_configuration == NULL){
		printf("No fue posible cargar la config\n");
		exit(2);
	}

	return new_configuration;
}

void leer_consola(t_log* logger)
{
	char* linea;

	while(1){

		linea = readline("> ");

		if(linea[0] == '\0'){
			break;
		}

		log_info(logger, linea);
	}

	free(linea);

}

void paquete(int8_t conexion)
{
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	while(leido[0] != '\0'){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}

	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int8_t conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}

}
