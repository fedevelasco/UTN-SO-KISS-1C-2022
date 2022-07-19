#ifndef RAM_PROTOCOL_H_
#define RAM_PROTOCOL_H_

#include <tads/process.h>
#include <libreriaConexiones.h>

t_process* recv_process(int32_t client_socket);
char* recv_buffer(int32_t* buffer_size, int32_t client_socket);


#endif /* RAM_PROTOCOL_H_ */





