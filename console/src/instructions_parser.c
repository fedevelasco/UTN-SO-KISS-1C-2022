#include <instructions_parser.h>


t_instructions_list *parse_pseudocode_file(char *path, t_log* logger) {
	log_info(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Inicio");
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		log_error(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Error al abrir el archivo");
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);


	t_instructions_list *instructions_list = create_instructions_list();
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

			if (strcmp(instruction->id, "NO_OP") == 0) {
				for (int i = 1; i < ((t_parameter*) list_get(instruction->parameters,0))->value; i++ ){
					t_instruction* instruction_noop = create_instruction();
					string_append(&instruction_noop->id, instruction->id);
					t_parameter* parameter = create_parameter();

					parameter->value = ((t_parameter*) list_get(instruction->parameters,0))->value;

					list_add(instruction_noop->parameters, parameter);

					list_add(instructions_list->instructions, instruction_noop);
				}
			}

			//Para debug.
			log_debug(logger, "Instruccion agregada a la lista: %s en i=%i\n", ((t_instruction*) list_get(instructions_list->instructions,*i))->id, *i);
			*i = *i + 1;
		}
	}

	void _add_instruction(char *line){
		return add_instruction_con_logger(line, logger, instructions_list, &i);
	}

	string_iterate_lines(lines, _add_instruction);

	int instructions_added = list_size(instructions_list->instructions);

	for(int i=0; i<instructions_added; i++){
		log_debug(logger, "parse_pseudocode_file - Instruction %s\n", ((t_instruction*) list_get(instructions_list->instructions,i))->id );
	}


	log_info(logger, "Carga de archivo de configuracion - Cantidad de instrucciones leidas: %i", instructions_added);

	log_info(logger, "Carga de archivo de configuracion - parse_pseudocode_file - Return");

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	return instructions_list;
}


t_instruction* parse_instruction(char *line, t_log* logger) {

	char** idAndParams = string_n_split(line, 2, " ");


	char id[strlen(idAndParams[0])+1];
	strcpy(id, idAndParams[0]);

	t_instruction* instruction = create_instruction();
	if(instruction == NULL){
		log_error(logger, "Carga de archivo de configuracion - parse_instruction - new_instruction - Error al crear instruccion");
		return NULL;
	}

	string_append(&instruction->id, id);

	if(strcmp(id, "COPY") == 0 || strcmp(id, "WRITE") == 0){

		t_parameter* parameter_1 = create_parameter();
		t_parameter* parameter_2 = create_parameter();

		char params[strlen(idAndParams[1])+1];
		strcpy(params, idAndParams[1]);
		char** parameters = string_split(params, " ");

		parameter_1->value = atoi(parameters[0]);
		parameter_2->value = atoi(parameters[1]);

		list_add(instruction->parameters, parameter_1);
		list_add(instruction->parameters, parameter_2);

		string_array_destroy(parameters);

		} else if(strcmp(id, "I/O") == 0 || strcmp(id, "READ") == 0 || strcmp(id, "NO_OP") == 0 ){

			t_parameter* parameter_1 = create_parameter();

			char params[strlen(idAndParams[1])+1];
			strcpy(params, idAndParams[1]);
			char** parameters = string_split(params, " ");

			parameter_1->value = atoi(parameters[0]);
			list_add(instruction->parameters, parameter_1);

			string_array_destroy(parameters);

		} else if(strcmp(id, "EXIT") == 0){

		}


	 string_array_destroy(idAndParams);

	 if(list_size(instruction->parameters) == 2){
		log_debug(logger, "Carga de archivo de configuracion - Instruction add: ID: %s Parameter_1: %d Parameter_2: %d List_size: %d", instruction->id, ((t_parameter*)list_get(instruction->parameters,0))->value, ((t_parameter*)list_get(instruction->parameters,1))->value, list_size(instruction->parameters));
	 } else if (list_size(instruction->parameters) == 1){
		 log_debug(logger, "Carga de archivo de configuracion - Instruction add: ID: %s Parameter_1: %d List_size: %d", instruction->id, ((t_parameter*)list_get(instruction->parameters,0))->value, list_size(instruction->parameters));
	 } else {
		 log_debug(logger, "Carga de archivo de configuracion - Instruction add: ID: %s List_size: %u", instruction->id, list_size(instruction->parameters));
	 }


	return instruction;
}
