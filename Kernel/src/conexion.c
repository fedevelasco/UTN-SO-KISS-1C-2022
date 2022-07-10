#include "../include/conexion.h"


// ---------------- CONFIG ------------------------

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

void terminar_config(t_config* config, t_config_data* config_data)
{

	config_destroy(config);
	free(config_data);

}

void cargar_config(t_config* config, t_config_data* config_data){

	config_data->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA"); 
	config_data->PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
	config_data->IP_CPU = config_get_string_value(config, "IP_CPU"); 
    config_data->PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    config_data->PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    config_data->PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    config_data->ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    config_data->ESTIMACION_INICIAL = config_get_string_value(config, "ESTIMACION_INICIAL");
    config_data->ALFA = config_get_string_value(config, "ALFA");
    config_data->GRADO_MULTIPROGRAMACION = config_get_string_value(config, "GRADO_MULTIPROGRAMACION");
    config_data->TIEMPO_MAXIMO_BLOQUEADO = config_get_string_value(config, "TIEMPO_MAXIMO_BLOQUEADO"); 

}


// ---------------- CONEXION -----------------------

int32_t recibir_operacion(int32_t socket_cliente)
{
	int32_t cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int32_t), MSG_WAITALL) > 0) //Recibo la cantidad de bytes que representan al op_code en el stream de datos
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

char* recibir_buffer(int32_t* buffer_size, int32_t socket_cliente) 
{
	char* buffer;

	read(socket_cliente, buffer_size, sizeof(int32_t)); //Leo el tamanio del payload y lo escribo en buffer_size
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size); //Ahora leo la cantidad de payload que me indica buffer_size y o guardo en buffer que devuelvo

	return buffer;
}


t_instructions_list* recibir_paquete(int32_t socket_cliente, t_log* logger)
{
	int32_t buffer_size; //Creo el tamanio del buffer
	char* buffer; //Creo el buffer

	buffer = recibir_buffer(&buffer_size, socket_cliente); //leo el payload y lo asigno a buffer
	log_info(logger, "buffer_size: %i\n", buffer_size); 

	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones

	return instructions_list;
}



// --------------- PROCESAR CONEXIONES DE CONSOLA ------------------------

/*
Esta funcion va a ser la encargada de procesar las pedidos de las consolas que se conectan, se corre por cada nueva conexion
dentro de "case INSTRUCTIONS" van a encontrarse las funciones core del tp
*/
static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_instructions_list* instructions_list;

	t_PCB* pcb = malloc(sizeof(t_PCB));


    int32_t cod_op; // = recibir_operacion(cliente_fd); Recibo el op_code de consola
    while (cliente_socket != -1) {

        cod_op = recibir_operacion(cliente_socket); //Recibo el op_code de consola

        switch (cod_op) {

			case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

				instructions_list = recibir_paquete(cliente_socket, logger); //Recibo paquete, lo deserializo y lo guardo en instruction_list
				log_info(logger, "Me llegaron los siguientes valores:");
				log_info(logger, "Process size: %d", instructions_list->process_size);
				//imprimir_lista_instrucciones(instructions_list, logger); //Imprimo la lista de instrucciones

				planificadorLP_agregar_a_new(instructions_list, logger);

				planificadorLP_agregar_a_ready(10, logger);

				break;

			case -1:

				log_error(logger, "El cliente se desconecto. Terminando conexion con el cliente."); //Codigo para finalizar conexion
				return;

			default:

				log_warning(logger,"Operacion desconocida."); //Codigo para cualquier otro op_code no mencionado arriba
				break;

		}
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}

/*
Esta funcion es la encargada de crear hilos por cada consola que se conecta al kernel. Cuando se conecta la consola
esta la acepta usando "esperar_cliente" que luego levanta un hilo para que ejecute la funcion "procesar_conexion" 
con los argumentos "args" que se pasan por un struct ya que solo admite un argumento por "pthread_create" 
*/
int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    
	int32_t cliente_socket = esperar_cliente(logger, server_name, server_socket);
    
	if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}





















	/* PRUEBA MANUAL
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

	*/
