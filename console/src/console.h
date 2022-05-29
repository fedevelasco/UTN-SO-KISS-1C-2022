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
t_config* load_configuration_file(t_log*);
int send_package(int32_t connection, t_package* package);
void end_process(int32_t, t_log*, t_config*);

#endif /* CONSOLE_H_ */
