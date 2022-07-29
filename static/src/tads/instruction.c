#include <tads/instruction.h>

t_instruction* create_instruction(){
	t_instruction* instruction = malloc(sizeof(t_instruction));
	memset(instruction, 0, sizeof(t_instruction));
	instruction->id = string_new();
	instruction->parameters = list_create();
	return instruction;
}

t_instruction* new_instruction(char* id, t_list* parameters) {
	t_instruction* instruction = create_instruction();

	string_append(&instruction->id, id);

	for(int i=0; i<list_size(parameters);i++)
		list_add(instruction->parameters, list_get(parameters, i));

	return instruction;
}

void instruction_destroy(t_instruction* instruction){
	list_destroy_and_destroy_elements(instruction->parameters, (void*) parameter_destroy);
	free(instruction->id);
	free(instruction);
}

uint32_t bytes_instruction(t_instruction* instruction) {
	//Empieza en 1 por que en el primer byte esta el largo del string.
	return 1 + string_length(instruction->id)+1 + bytes_list(instruction->parameters, sizeof(uint32_t));

}
