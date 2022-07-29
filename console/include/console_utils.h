#ifndef CONSOLE_UTILS_H_
#define CONSOLE_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/collections/list.h>
#include<commons/config.h>

#include <tads/parameter.h>
#include <tads/instruction.h>
#include <tads/instructions_list.h>
#include <tads/op_code.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <serialization.h>



typedef struct {
		char *path;
		t_list *lines;
	} t_pseudocode;


uint32_t create_connection(t_log* logger, const char* server_name, char *ip, char* port);
void end_connection(uint32_t connection);
uint32_t receive_operation_code(uint32_t server_socket);
uint32_t send_package(uint32_t connection, t_package* package);
uint32_t send_to_server(uint32_t connection, t_package* package);
t_log* start_logger(void);
t_config* load_configuration_file(t_log* logger);
void end_process(uint32_t connection, t_log* logger, t_config* config);

#endif /* CONSOLE_UTILS_H_ */
