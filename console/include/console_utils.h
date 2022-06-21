#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/collections/list.h>



typedef enum
{
	INSTRUCTIONS,
	EXIT
}op_code;

typedef struct
{
	int32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code operation_code;
	t_buffer* buffer;
} t_package;

typedef struct {
		char *path;
		t_list *lines;
	} t_pseudocode;

typedef struct {
	    int32_t id_length;
		char* id;
		int32_t* parameters;
		int32_t cantParameters;
	} t_instruction;

typedef struct {
		int32_t process_size;
		t_list *instructions;
	} t_instructions_list;


void* serialize_package(t_package* package, int32_t bytes);
int32_t create_connection(t_log* logger, const char* server_name, char *ip, char* port);
void create_buffer(t_package* package, t_instructions_list* instructions_list);
t_package* create_package(t_instructions_list* instructions_list);
void free_package(t_package* package);
void end_connection(int32_t connection);
int32_t receive_operation_code(int32_t server_socket);
t_buffer* create_instruction_buffer(t_instructions_list* instructions_list, t_log* logger);
t_package* create_instructions_package(t_buffer* instructions_buffer);
int32_t send_package(int32_t connection, t_package* package);
int32_t send_to_server(int32_t connection, t_package* package);
int32_t instructions_list_size(t_instructions_list* instructions_list);
void instructions_list_destroyer(t_instructions_list* instructions_list);

#endif /* UTILS_H_ */
