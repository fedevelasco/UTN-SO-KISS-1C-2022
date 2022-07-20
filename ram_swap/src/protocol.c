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


bool send_process_init(int32_t client_socket, int32_t pid_first_level_table_number){
	void* stream = malloc(sizeof(int32_t));
	memcpy(stream, &pid_first_level_table_number, sizeof(int32_t));
	if (send(client_socket, stream, sizeof(int32_t), 0) == -1) {
	        free(stream);
	        return false;
	    }
	    free(stream);
	    return true;
}


