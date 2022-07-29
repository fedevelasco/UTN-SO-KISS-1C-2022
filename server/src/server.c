#include <server.h>

uint32_t main(void) {

	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	//Config
	char* ip;
	char* puerto;
	t_config* config;

	//test_serialization();

	config = iniciar_config();
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");


	uint32_t server_fd = iniciar_servidor(logger, "Server", ip, puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
	uint32_t cliente_fd = esperar_cliente(logger, "Server", server_fd);


	t_instructions_list* instructions_list;

	while (1) {
		uint32_t cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case INSTRUCTIONS:
			instructions_list = recibir_paquete(cliente_fd, logger);
			log_info(logger, "Me llegaron los siguientes valores:");
			log_info(logger, "Process size: %d", instructions_list->process_size);

			for(int i=0;i<list_size(instructions_list->instructions);i++){

				t_instruction* instruction = list_get(instructions_list->instructions,i);
				log_info(logger, "Instruction: %s ", instruction->id);
				for(int i=0;i<list_size(instruction->parameters);i++){
					t_parameter* parameter = list_get(instruction->parameters,i);

					log_info(logger, "Parameters: %d ", parameter->value);
				}

			}


			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	close(server_fd);
	close(cliente_fd);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("/home/utnso/tp-2022-1c-Grupo-TP-SO/server/server.config");

	if (nuevo_config == NULL){
		printf("No fue posible cargar la config\n");
		exit(2);
	}

	return nuevo_config;
}
