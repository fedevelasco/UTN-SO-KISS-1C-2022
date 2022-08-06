#include <tads/instructions_list.h>


t_instructions_list* create_instructions_list(){
	t_instructions_list* instructions_list = malloc(sizeof(t_instructions_list));
	instructions_list->instructions = list_create();
	instructions_list->process_size = 0;
	return instructions_list;
}

t_instructions_list* create_instructions_list_with_size(uint32_t size){
	t_instructions_list* instructions_list = malloc(size);
	instructions_list->instructions = list_create();
	instructions_list->process_size = 0;
	return instructions_list;
}

t_instructions_list* new_instructions_list(t_list* instructions, uint32_t process_size) {
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

uint32_t bytes_instructions_list(t_instructions_list* instructions_list) {
	//Empieza en sizeof(uint32_t) por que en el primer byte tengo el tamanio de la lista.
	uint32_t bytes = sizeof(uint32_t);

	for(int i=0;i<list_size(instructions_list->instructions);i++){
		bytes += bytes_instruction(list_get(instructions_list->instructions, i));
	}

	bytes += sizeof(uint32_t);


	return bytes;

}

void imprimir_lista_instrucciones(t_instructions_list* instructions_list){

	for(int i=0;i<list_size(instructions_list->instructions);i++){ //Recorro la lista de instrucciones para mostrar por pantalla

		t_instruction* instruction = list_get(instructions_list->instructions,i);
		
		for(int i=0;i<list_size(instruction->parameters);i++){
			t_parameter* parameter = list_get(instruction->parameters,i);
			
			printf("Parameters: %d \n", parameter->value);
		}

	}

}
