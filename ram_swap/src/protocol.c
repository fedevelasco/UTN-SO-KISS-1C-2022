#include <protocol.h>


char* recv_buffer(int32_t* buffer_size, int32_t client_socket)
{
	char* buffer;

	read(client_socket, buffer_size, sizeof(int32_t));
	buffer = malloc(*buffer_size);
	read(client_socket, buffer, *buffer_size);

	return buffer;
}


int32_t send_process_init(int32_t client_socket, int32_t pid_first_level_table_number){

//	void* stream = malloc(sizeof(int32_t));
//	memcpy(stream, &pid_first_level_table_number, sizeof(int32_t));

	t_buffer* buffer = create_buffer();

	buffer->size = sizeof(int32_t);

	buffer->stream = malloc(buffer->size);
	serialize_int(buffer->stream, &pid_first_level_table_number);

	t_package* package = create_package();

	package->operation_code = PROCESS_INIT_SEND;
	package->buffer = buffer;


	if (send_package(client_socket, package) == -1) {

	        return 1;
	    }

	return 0;
}

int32_t send_package(int32_t connection, t_package* package)
{
	int32_t bytes = package->buffer->size + sizeof(int32_t) + sizeof(op_code);
	char* to_send = serialize_package(package, bytes);

	if(send(connection, to_send, bytes, 0) == -1){
		free(to_send);
		return -1;
	}

	free(to_send);
	return 1;
}


