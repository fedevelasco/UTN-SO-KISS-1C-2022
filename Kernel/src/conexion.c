#include "../include/conexion.h"

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
	log_info(logger, "buffer_size:%i\n", buffer_size); 

	t_instructions_list* instructions_list = create_instructions_list_with_size(buffer_size); //Creo una lista de instrucciones con tamanio ya definido

	deserialize_instructions_list(instructions_list, buffer); //Deserializo el buffer y lo guardo en la lista de instrucciones

	return instructions_list;
}
