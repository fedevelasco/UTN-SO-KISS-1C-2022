#include <tads/package.h>


t_package* create_package(){

	t_package* package = malloc(sizeof(t_package));
	
	package->operation_code = UNDEFINED;
	package->buffer = malloc(sizeof(t_buffer));
    return package;
}

t_package* new_package(t_buffer* buffer, t_op_code opcode){

	t_package* package = create_package();
	
	package->operation_code = opcode;
	package->buffer->stream = malloc(buffer->size);
	uint32_t buffer_size = buffer->size;
	package->buffer->size = buffer_size;
	memcpy(package->buffer->stream, buffer->stream, buffer->size);

	buffer_destroy(buffer);

    return package;
}

void package_destroy(t_package* package)
{
	free(package->buffer->stream);
	free(package->buffer);
	free(package);
}
