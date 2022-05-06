#include "instructionsParser.h"
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(*a))

t_instructions_list *parse_pseudocode_file(char *path) {
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);


	t_instructions_list *instructions_list = new_instructions_list();

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	char** lines = string_split(buffer, "\n");

	void add_instruction(char *line) {
		if (!string_is_empty(line)) {

			list_add(instructions_list->instructions, parse_instruction(line));

		}
	}

	string_iterate_lines(lines, add_instruction);

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	return instructions_list;
}

t_instruction* new_instruction(char* id) {

	int id_size = string_length(id);

	t_instruction *instruction = malloc(sizeof(t_instruction));
	if (instruction == NULL)
		return NULL;

	instruction->id = malloc((id_size + 1) * sizeof(char));
	if (instruction->id == NULL) {
		free(instruction);
		return NULL;
	}

	//TODO: pasar inicializacion de parameters en el if a una funcion donde le paso puntero a la instruccion y cuantos parametros tiene para asignarle la memoria.

	if(strcmp(id, "COPY") == 0){
		instruction->parameters = malloc(2 * sizeof(u_int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
	} else if(strcmp(id, "WRITE") == 0) {
		instruction->parameters = malloc(2 * sizeof(u_int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
	} else if(strcmp(id, "I/O") == 0){
		instruction->parameters = malloc(sizeof(u_int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
	} else if(strcmp(id, "READ") == 0) {
		instruction->parameters = malloc(sizeof(u_int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
	} else if(strcmp(id, "EXIT") == 0){
		instruction->parameters = NULL;
	}
	return instruction;
}



t_instructions_list* new_instructions_list() {

	t_instructions_list *instructions_list = malloc(sizeof(t_instructions_list));
	if (instructions_list == NULL)
		return NULL;

	instructions_list->instructions = list_create();

	return instructions_list;
}

t_instruction* parse_instruction(char *line) {
	int aParameter;
	char* line2 = line;

	//TODO: Explota en la segunda iteracion adentro de una funcion de commons/string. Creo que falta liberar memoria en algun lado
	char** idAndParams = string_n_split(line2, 2, " ");
	char* id = idAndParams[0];
	char* params = idAndParams[1];


	t_instruction* instruction = new_instruction(id);
	memcpy(instruction->id, id, sizeof(char));




    printf("instruction add: %s \n", instruction->id);

	char** parameters = string_split(params, " ");

	//https://stackoverflow.com/questions/5643316/how-to-find-the-number-of-elements-in-char-array

	int size = sizeof(parameters) / sizeof(parameters[0]);

	for(int i = 0; i < size; i++){
		aParameter = atoi(parameters[i]);
		instruction->parameters[i] = aParameter;
	}

	printf("instruction add: %s %u %u %u \n", instruction->id, instruction->parameters[0], instruction->parameters[1], size);


	return instruction;
}
