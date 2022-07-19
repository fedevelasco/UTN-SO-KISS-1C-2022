#include <protocol.h>

t_process* recv_process(int32_t client_socket){

	int32_t buffer_size;
	char* buffer;

	buffer = recv_buffer(&buffer_size, client_socket);
	log_info(logger, "recv_process - buffer_size:%i\n", buffer_size);

	t_process* process = create_process();
	deserialize_process(process, buffer);

	return process;
}

char* recv_buffer(int32_t* buffer_size, int32_t client_socket)
{
	char* buffer;

	read(client_socket, buffer_size, sizeof(int32_t));
	buffer = malloc(*buffer_size);
	read(client_socket, buffer, *buffer_size);

	return buffer;
}

