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

	// TESTING SERIALIZACION DE PCB
	t_PCB* testing = malloc(sizeof(t_PCB));
	testing = pcb_create();
	testing->PID = 3;
	testing->PC = 3;
	testing->ESTIMACION_RAFAGA = 3;
	testing->TAMANIO_PROCESO = 3;


	char* id =string_new();
	string_append(&id, "READ");
	char* id2 =string_new();
	string_append(&id2, "WRITE");

	t_list* parameters = list_create();
	t_parameter* parameter1 = new_parameter(2);
	t_parameter* parameter2 = new_parameter(100);
	list_add(parameters, parameter1);
	list_add(parameters, parameter2);

	t_list* parameters2 = list_create();
	t_parameter* parameter3 = new_parameter(22);
	t_parameter* parameter4 = new_parameter(33);
	list_add(parameters2, parameter3);
	list_add(parameters2, parameter4);

	t_instruction* instruction = new_instruction(id, parameters);
	t_instruction* instruction2 = new_instruction(id2, parameters2);

	list_add(testing->LISTA_INSTRUCCIONES->instructions, instruction);

	testing->LISTA_INSTRUCCIONES->process_size = 1000;

	pcb_imprimir(testing, logger);
	int bytes = bytes_PCB(testing); //Tamanio de PCB
	char* stream_de_datos = malloc (bytes);	
	pcb_serializar(testing, logger, stream_de_datos);
	t_PCB* testing2; //= malloc(bytes);
	testing2 = pcb_create_with_size(bytes);
	pcb_deserializar(testing2, stream_de_datos);
	pcb_imprimir(testing2, logger);
	//printf("%i");


	// Listas y Colas
	COLA_NEW = queue_create();
	LISTA_READY = list_create();
	t_queue* COLA_EXIT;

	// Main
	//while (server_escuchar(logger, "Kernel", server_fd)); //espero a mi cliente

	// Cierre de programa
	close(server_fd); 

	return EXIT_SUCCESS;
}