#ifndef RAM_H_
#define RAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <libreriaConexiones.h>

#include <ram_utils.h>
#include <connection_utils.h>
#include <swap_utils.h>

#define SERVERNAME "RAM_SWAP"


pthread_mutex_t MUTEX_CLIENT;

#define MUTEX_CLIENT(REPLACED) \
	MUTEX(REPLACED,mutex_client);

pthread_mutex_t MUTEX_SWAP;

#define MUTEX_SWAP(REPLACED) \
	MUTEX(REPLACED,mutex_swap);

#define THREAD void*

//////////////////////////////////////////
//TODO: Aca definir todos los mutex. Los tengo que cargar en mutex_init de main.
pthread_mutex_t LOCK_ACCESS_RAM;
pthread_mutex_t LOCK_ACCESS_TABLE;
//////////////////////////////////////////

uint32_t server_socket;


#endif /* CONSOLE_H_ */
