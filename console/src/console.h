#ifndef CONSOLE_H_
#define CONSOLE_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>

#include<readline/readline.h>

#include "consoleUtils.h"
#include "instructionsParser.h"


t_log* start_logger(void);
t_config* load_configuration_file();


void leer_consola(t_log*);
void paquete(int8_t);
void terminar_programa(int8_t, t_log*, t_config*);

#endif /* CONSOLE_H_ */
