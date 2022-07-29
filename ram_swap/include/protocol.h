#ifndef RAM_PROTOCOL_H_
#define RAM_PROTOCOL_H_

#include <serialization.h>
#include <tads/process.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <libreriaConexiones.h>
#include <global_structures.h>




char* recv_buffer(uint32_t client_socket);
uint32_t send_process_init(uint32_t client_socket, uint32_t pid_first_level_table_number);
uint32_t send_package(uint32_t connection, t_package* package);
uint32_t send_to_server(uint32_t connection, t_package* package);
t_buffer* new_process_init_buffer(uint32_t pid_first_level_table_number);
uint32_t send_process_suspend(uint32_t client_socket);
t_buffer* new_process_suspend_buffer();
uint32_t send_memory_config(uint32_t client_socket, t_memory_config* memory_config);
t_buffer* new_memory_config_buffer(t_memory_config* memory_config);
t_buffer* new_second_level_table_buffer(uint32_t second_level_table_id);
uint32_t send_second_level_table_id(uint32_t client_socket, uint32_t second_level_table_id);




#endif /* RAM_PROTOCOL_H_ */





