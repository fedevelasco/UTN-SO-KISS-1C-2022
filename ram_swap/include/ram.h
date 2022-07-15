#ifndef RAM_H_
#define RAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include <ram_utils.h>

#define SERVERNAME "RAM_SWAP"


pthread_mutex_t mutex_client;

#define MUTEX_CLIENT(REPLACED) \
	MUTEX(REPLACED,mutex_client);

pthread_mutex_t mutex_swap;

#define MUTEX_SWAP(REPLACED) \
	MUTEX(REPLACED,mutex_swap);

#define THREAD void*

//////////////////////////////////////////
//TODO: Aca definir todos los mutex. Los tengo que cargar en mutex_init de main.
pthread_mutex_t lock_access_ram;
pthread_mutex_t lock_access_table;
//////////////////////////////////////////

int32_t server_fd;


#endif /* CONSOLE_H_ */
