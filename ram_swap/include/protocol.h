#ifndef RAM_PROTOCOL_H_
#define RAM_PROTOCOL_H_

#include <serialization.h>
#include <tads/process.h>
#include <tads/package.h>
#include <tads/buffer.h>
#include <libreriaConexiones.h>
#include <global_structures.h>




char* recv_buffer(int32_t* buffer_size, int32_t client_socket);
int32_t send_process_init(int32_t client_socket, int32_t pid_first_level_table_number);
int32_t send_package(int32_t connection, t_package* package);
int32_t send_to_server(int32_t connection, t_package* package);
t_buffer* new_process_init_buffer(int32_t pid_first_level_table_number);



#endif /* RAM_PROTOCOL_H_ */





