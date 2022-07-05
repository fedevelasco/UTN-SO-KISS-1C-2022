#include "../include/kernel.h"

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("/home/utnso/tp-2022-1c-Grupo-TP-SO/Kernel/src/configuracion.config");

	if (nuevo_config == NULL){
		printf("No fue posible cargar la config\n");
		exit(2);
	}

	return nuevo_config;
}

void terminar_config(t_config* config)
{

	config_destroy(config);

}

int32_t main(void) {

	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	//Config
	char* ip;
	char* puerto;
	t_config* config;

	//test_serialization();

	config = iniciar_config(); //Inicio de config
	ip = config_get_string_value(config, "IP"); //Busco IP
	puerto = config_get_string_value(config, "PUERTO"); //Busco PUERTO


	int32_t server_fd = iniciar_servidor(logger, "Kernel", ip, puerto); //Inicio el servidor
	log_info(logger, "Kernel   listo para recibir a consola");
	int32_t cliente_fd = esperar_cliente(logger, "Kernel", server_fd); //Configuro el socket para esperar clientes


	t_instructions_list* instructions_list;

	t_PCB* pcb;

	COLA_NEW = queue_create();

	LISTA_READY = list_create();

	while (1) {

		int32_t cod_op = recibir_operacion(cliente_fd); //Recibo el op_code de consola 
		switch (cod_op) {

		case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

			instructions_list = recibir_paquete(cliente_fd, logger); //Recibo paquete, lo deserializo y lo guardo en instruction_list
			log_info(logger, "Me llegaron los siguientes valores:");
			log_info(logger, "Process size: %d", instructions_list->process_size);
			imprimir_lista_instrucciones(instructions_list, logger); //Imprimo la lista de instrucciones
			
			planificadorLP_agregar_a_new(instructions_list, logger);

			planificadorLP_agregar_a_ready(10, logger);

			break;

		case -1:

			log_error(logger, "el cliente se desconecto. Terminando servidor"); //Codigo para finalizar conexion
			return EXIT_FAILURE;

		default:

			log_warning(logger,"Operacion desconocida. No quieras meter la pata"); //Codigo para cualquier otro op_code no mencionado arriba
			break;

		}
	}
	
	// Agregar PCB a la cola de new

	// Enviar el pcb del proceso a ejecutar a CPU DISPATCH

	
	close(server_fd); //Cierro server file descriptor 
	close(cliente_fd); //Cierro cliente file descriptor 
	pcb_destroy(pcb); //Mato al PCB
	return EXIT_SUCCESS;


}