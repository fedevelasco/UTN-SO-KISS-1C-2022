#include <conexion.h>

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

uint32_t recibir_operacion(uint32_t socket_cliente)
{
	uint32_t cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL) > 0) //Recibo la cantidad de bytes que representan al op_code en el stream de datos
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

char* recibir_buffer(uint32_t* buffer_size, uint32_t socket_cliente) 
{
	char* buffer;

	read(socket_cliente, buffer_size, sizeof(uint32_t)); //Leo el tamanio del payload y lo escribo en buffer_size
	buffer = malloc(*buffer_size);
	read(socket_cliente, buffer, *buffer_size); //Ahora leo la cantidad de payload que me indica buffer_size y o guardo en buffer que devuelvo

	return buffer;
}


t_instructions_list* recibir_paquete(uint32_t socket_cliente, t_log* logger)
{
	uint32_t buffer_size; //Creo el tamanio del buffer
	char* buffer; //Creo el buffer

	buffer = recibir_buffer(&buffer_size, socket_cliente); //leo el payload y lo asigno a buffer
	log_info(logger, "buffer_size:%i\n", buffer_size); 

	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones

	return instructions_list;
}




static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_instructions_list* instructions_list;

	//TODO: ver si va
	// t_PCB* pcb = malloc(sizeof(t_PCB)); 


    uint32_t cod_op; // = recibir_operacion(cliente_fd); Recibo el op_code de consola
    while (cliente_socket != -1) {

        cod_op = recibir_operacion(cliente_socket); //Recibo el op_code de consola

        switch (cod_op) {

			case INSTRUCTIONS: //Ejecuto el caso donde op_code == INSTRUCTIONS

				instructions_list = recibir_paquete(cliente_socket, logger); //Recibo paquete, lo deserializo y lo guardo en instruction_list
				log_info(logger, "Me llegaron los siguientes valores:");
				log_info(logger, "Process size: %d", instructions_list->process_size);
				imprimir_lista_instrucciones(instructions_list); //Imprimo la lista de instrucciones

				planificadorLP_agregar_a_new(instructions_list, logger);

				planificadorLP_agregar_a_ready(10, logger);

				break;

			case -1:

				log_error(logger, "el cliente se desconecto. Terminando servidor"); //Codigo para finalizar conexion
				return;

			default:

				log_warning(logger,"Operacion desconocida. No quieras meter la pata"); //Codigo para cualquier otro op_code no mencionado arriba
				break;

		}
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}


int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    
	uint32_t cliente_socket = esperar_cliente(logger, server_name, server_socket);
	      //cliente_fd = esperar_cliente(logger, "Kernel", server_fd);
    
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
