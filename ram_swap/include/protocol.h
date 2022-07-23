#ifndef RAM_PROTOCOL_H_
#define RAM_PROTOCOL_H_

#include <tads/process.h>
#include <libreriaConexiones.h>
#include <global_structures.h>
#include <serialization.h>


char* recv_buffer(int32_t* buffer_size, int32_t client_socket);
int32_t send_process_init(int32_t client_socket, int32_t pid_first_level_table_number);
int32_t send_package(int32_t connection, t_package* package);



#endif /* RAM_PROTOCOL_H_ */





