#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <commons/collections/list.h>

pthread_attr_t thread_attributes;
t_list* mutex_list;

// Dejo un macro para no tener que estar rodeando todo con mutexs, pueden creer otro macro en base a este por cada mutex especifico
#define MUTEX(REPLACED, MUTEX_REPLACED) \
	{pthread_mutex_lock(&MUTEX_REPLACED); \
	REPLACED; \
	pthread_mutex_unlock(&MUTEX_REPLACED);} \

uint32_t mutex_init(int mutex_size, ...);
uint32_t thread_create(pthread_t * thread, void *(*function)(void *));
uint32_t thread_create_with_parameter(pthread_t * hilo, void *(*function)(void *), void * parameter);
void mutex_destroy();
uint32_t set_thread_attributes();
void thread_attributes_destroy();

#endif /* THREADS_H_ */
