#include "../include/kernel.h"

int32_t main(void) {

	// Log
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	// Config
	t_config* config;
	t_config_data* config_data = malloc(sizeof(t_config_data));
	char* ip = "127.0.0.1";
	config = iniciar_config(); //Inicio de config
	cargar_config(config, config_data); //Asigno los valores de config
	
	// Conexion
	log_info(logger, "Kernel - Iniciando conexiones");
	log_info(logger, "Kernel - K");
	int32_t server_fd = iniciar_servidor(logger, "Kernel", ip, config_data->PUERTO_ESCUCHA); //Inicio el servidor
	log_info(logger, "Kernel - Listo para recibir a clientes");
	int32_t cliente_cpu_fd = iniciar_cliente(config_data->IP_CPU, config_data->PUERTO_CPU_DISPATCH, logger);

	// Listas y Colas
	COLA_NEW = queue_create();
	LISTA_READY = list_create();
	t_queue* COLA_EXIT;

	// Main
	while (server_escuchar(logger, "Kernel", server_fd)); //espero a mi cliente

	// Cierre de programa
	close(server_fd); 

	return EXIT_SUCCESS;
}