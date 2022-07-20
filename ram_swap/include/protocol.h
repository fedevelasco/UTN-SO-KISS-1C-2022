#ifndef RAM_PROTOCOL_H_
#define RAM_PROTOCOL_H_

#include <tads/process.h>
#include <libreriaConexiones.h>
#include <ram.h>

t_process* recv_process(int32_t client_socket);
char* recv_buffer(int32_t* buffer_size, int32_t client_socket);
bool send_process_init(int32_t client_socket, int32_t pid_first_level_table_number);



#endif /* RAM_PROTOCOL_H_ */





