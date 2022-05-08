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

			t_instruction* instruction = parse_instruction(line);
			list_add(instructions_list->instructions, instruction);
			free(instruction->id);
			free(instruction->parameters);
			free(instruction);

		}
	}

	string_iterate_lines(lines, add_instruction);

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	int size = list_size(instructions_list->instructions);

	for(int i = 0; i < size; i++){
		t_instruction* instruction = list_get(instructions_list->instructions, i);
		printf("Instruccion %u: %s ", i, instruction->id);
		free(instruction->id);
		free(instruction->parameters);
		free(instruction);
	}

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
	strcpy(instruction->id, id);

	//TODO: pasar inicializacion de parameters en el if a una funcion donde le paso puntero a la instruccion y cuantos parametros tiene para asignarle la memoria.

	if(strcmp(id, "COPY") == 0){
		instruction->parameters = malloc(2 * sizeof(u_int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->parameters[1]=0;
		instruction->cantParameters=2;

	} else if(strcmp(id, "WRITE") == 0) {
		instruction->parameters = malloc(2 * sizeof(uint32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->parameters[1]=0;
		instruction->cantParameters=2;
	} else if(strcmp(id, "I/O") == 0){
		instruction->parameters = malloc(sizeof(uint32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->cantParameters=1;
	} else if(strcmp(id, "READ") == 0) {
		instruction->parameters = malloc(sizeof(uint32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->cantParameters=1;
	} else if (strcmp(id, "NO_OP") == 0){
		instruction->parameters = malloc(sizeof(uint32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->cantParameters=1;
	} else if(strcmp(id, "EXIT") == 0){
		instruction->parameters = NULL;
		instruction->cantParameters=0;
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


	char** idAndParams = string_n_split(line, 2, " ");


	char id[strlen(idAndParams[0])+1];
	strcpy(id, idAndParams[0]);

	t_instruction* instruction = new_instruction(id);

	printf("instruction add: %s \n", instruction->id);

	 if(strcmp(id, "EXIT") != 0){

		 char params[strlen(idAndParams[1])+1];
		 strcpy(params, idAndParams[1]);

		 char** parameters = string_split(params, " ");

		 for(int i = 0; i < instruction->cantParameters; i++){
		 		aParameter = atoi(parameters[i]);
		 		instruction->parameters[i] = aParameter;
		 	}

		 string_array_destroy(parameters);

	    }

	 string_array_destroy(idAndParams);

	 if(instruction->cantParameters == 2){
		printf("instruction add: %s %u %u %u \n", instruction->id, instruction->parameters[0], instruction->parameters[1], instruction->cantParameters);
	 } else if (instruction->cantParameters == 1){
			printf("instruction add: %s %u %u \n", instruction->id, instruction->parameters[0], instruction->cantParameters);

	 } else {
		 printf("instruction add: %s %u \n", instruction->id, instruction->cantParameters);
	 }


	return instruction;
}
