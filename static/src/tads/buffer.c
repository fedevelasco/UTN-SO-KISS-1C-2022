#include <tads/buffer.h>


t_buffer* create_buffer(){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->stream = NULL;

	return buffer;
}

void buffer_destroy(t_buffer* buffer){
	free(buffer->stream);
	free(buffer);
}


