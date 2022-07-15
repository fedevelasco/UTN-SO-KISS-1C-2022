#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
#include <stdarg.h>
#include <commons/collections/list.h>

pthread_attr_t thread_attributes;
t_list* mutex_list;

// Dejo un macro para no tener que estar rodeando todo con mutexs, pueden creer otro macro en base a este por cada mutex especifico
#define MUTEX(REPLACED, MUTEX_REPLACED) \
	{pthread_mutex_lock(&MUTEX_REPLACED); \
	REPLACED; \
	pthread_mutex_unlock(&MUTEX_REPLACED);} \

void mutex_init(int mutex_size, ...);
int thread_create(pthread_t * thread, void *(*function)(void *));
int thread_create_with_parameter(pthread_t * hilo, void *(*function)(void *), void * parameter);
void mutex_destroy();
void set_thread_attributes();
void thread_attributes_destroy();

#endif /* THREADS_H_ */
