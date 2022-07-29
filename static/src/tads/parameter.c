#include <tads/parameter.h>

t_parameter* create_parameter(){
	t_parameter* parameter = malloc(sizeof(t_parameter));
	parameter->value=0;
	return parameter;
}

t_parameter* new_parameter(uint32_t value){
	t_parameter* parameter = create_parameter();
	parameter->value = value;
	return parameter;
}

void parameter_destroy(t_parameter* parameter){
	free(parameter);
}
