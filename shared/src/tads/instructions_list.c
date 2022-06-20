#include <instructions_list.h>


t_instructions_list* create_instructions_list(){
	t_instructions_list* instructions_list = malloc(sizeof(t_instructions_list));
//	memset(instructions_list, 0, sizeof(t_instructions_list));
	instructions_list->instructions = list_create();
	instructions_list->process_size = 0;
	return instructions_list;
}

t_instructions_list* new_instructions_list(t_list* instructions, int32_t process_size) {
	t_instructions_list* instructions_list = create_instructions_list();

	for(int i=0; i<list_size(instructions);i++)
		list_add(instructions_list->instructions, list_get(instructions, i));

	instructions_list->process_size = process_size;

	return instructions_list;
}

void instructions_list_destroy(t_instructions_list* instructions_list){

	list_destroy_and_destroy_elements(instructions_list->instructions, (void*) instruction_destroy);
	free(instructions_list);
}

int32_t bytes_instructions_list(t_instructions_list* instructions_list) {
	int32_t bytes = 0;

	for(int i=0;i<list_size(instructions_list->instructions);i++){
		bytes += bytes_instruction(list_get(instructions_list->instructions, i));
	}

	bytes += sizeof(int32_t);


	return bytes;

}
