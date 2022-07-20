#ifndef RAM_CONNECTION_PROCESSOR_H_
#define RAM_CONNECTION_PROCESSOR_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <tads/process.h>
#include <libreriaConexiones.h>
#include <protocol.h>
#include <connection_utils.h>
#include <ram.h>
#include <ram_functions.h>

bool process_init(int32_t client_socket);

#endif /* RAM_CONNECTION_PROCESSOR_H_ */


