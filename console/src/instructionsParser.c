#include "instructionsParser.h"
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(*a))

t_instructions_list *parse_pseudocode_file(char *path, t_log* logger) {
	log_info(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Inicio");
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		log_error(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Error al abrir el archivo");
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);


	t_instructions_list *instructions_list = new_instructions_list();
	if (instructions_list == NULL) {
		log_error(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Error al crear lista de instrucciones");
		return NULL;
	}

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	char** lines = string_split(buffer, "\n");

	int i = 0;

	void add_instruction_con_logger(char *line, t_log* logger, t_instructions_list* instructions_list, int* i) {
		if (!string_is_empty(line)) {

			t_instruction*  instruction = parse_instruction(line, logger);

			log_debug(logger, "Instruccion que sale del parseo: %s\n", instruction->id );

			list_add(instructions_list->instructions, instruction);

			//Para debug.
			t_instruction* instruction2 = list_get(instructions_list->instructions,*i);
			log_debug(logger, "Instruccion agregada a la lista: %s en i=%i\n", instruction2->id, *i);
			*i = *i + 1;
		}
	}

	void _add_instruction(char *line){
		return add_instruction_con_logger(line, logger, instructions_list, &i);
	}

	string_iterate_lines(lines, _add_instruction);

	int size = list_size(instructions_list->instructions);
	t_instruction* instructionTest = list_get(instructions_list->instructions,0);
	log_info(logger, "create_instruction_buffer - Instruction %s\n", instructionTest->id );
	instructionTest = list_get(instructions_list->instructions,2);
	log_info(logger, "create_instruction_buffer - Instruction %s\n", instructionTest->id );


	log_info(logger, "Carga de archivo de configuracion - Cantidad de instrucciones leidas: %i", size);

	log_info(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Return");

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
	memset(instruction, 0, sizeof(t_instruction));

	instruction->id_length = 0;

	instruction->id = malloc((id_size + 1) * sizeof(char));
	if (instruction->id == NULL) {
		free(instruction);
		return NULL;
	}

	instruction->cantParameters = 0;

	instruction->id_length = id_size;
	strcpy(instruction->id, id);

	if(strcmp(id, "COPY") == 0){
		instruction->parameters = malloc(2 * sizeof(int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->parameters[1]=0;
		instruction->cantParameters=2;

	} else if(strcmp(id, "WRITE") == 0) {
		instruction->parameters = malloc(2 * sizeof(int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->parameters[1]=0;
		instruction->cantParameters=2;
	} else if(strcmp(id, "I/O") == 0){
		instruction->parameters = malloc(sizeof(int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->cantParameters=1;
	} else if(strcmp(id, "READ") == 0) {
		instruction->parameters = malloc(sizeof(int32_t));
		if (instruction->parameters == NULL) {
			free(instruction);
			return NULL;
		}
		instruction->parameters[0]=0;
		instruction->cantParameters=1;
	} else if (strcmp(id, "NO_OP") == 0){
		instruction->parameters = malloc(sizeof(int32_t));
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

    instructions_list->process_size = 0;

	instructions_list->instructions = list_create();

	return instructions_list;
}

t_instruction* parse_instruction(char *line, t_log* logger) {
	int aParameter;


	char** idAndParams = string_n_split(line, 2, " ");


	char id[strlen(idAndParams[0])+1];
	strcpy(id, idAndParams[0]);

	t_instruction* instruction = new_instruction(id);
	if(instruction == NULL){
		log_error(logger, "Carga de archivo de configuracion - parse_instruction - new_instruction - Error al crear instruccion");
		return NULL;
	}

	 if(strcmp(id, "EXIT\r") != 0){

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
		log_debug(logger, "Carga de archivo de configuracion - Instruction add: %s %u %u %u", instruction->id, instruction->parameters[0], instruction->parameters[1], instruction->cantParameters);
	 } else if (instruction->cantParameters == 1){
		 log_debug(logger, "Carga de archivo de configuracion - Instruction add: %s %u %u", instruction->id, instruction->parameters[0], instruction->cantParameters);
	 } else {
		 log_debug(logger, "Carga de archivo de configuracion - Instruction add: %s %u", instruction->id, instruction->cantParameters);
	 }


	return instruction;
}
