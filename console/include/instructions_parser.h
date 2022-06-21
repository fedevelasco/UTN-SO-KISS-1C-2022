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

t_log* start_logger(void);
t_instructions_list* parse_pseudocode_file(char* path, t_log* logger);
t_instruction* parse_instruction(char *line, t_log* logger);
t_instructions_list* new_instructions_list(void);
t_instruction* new_instruction(char* id);

#endif /* INSTRUCTIONPARSER_H_ */
