#ifndef INSTRUCTIONPARSER_H_
#define INSTRUCTIONPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include<readline/readline.h>

#include "consoleUtils.h"

typedef struct {
		char *path;
		t_list *lines;
	} t_pseudocode;

typedef struct {
		char* id;
		uint32_t* parameters;
		uint32_t cantParameters;
	} t_instruction;

typedef struct {
		t_list *instructions;
	} t_instructions_list;


t_log* start_logger(void);
t_instructions_list* parse_pseudocode_file(char* path);
t_instruction* parse_instruction(char *line);
t_instructions_list* new_instructions_list(void);
t_instruction* new_instruction(char* id);

#endif /* INSTRUCTIONPARSER_H_ */
