#include "threads.h"

uint32_t mutex_init(int mutex_size, ...) {

	mutex_list = list_create();

	//Obtengo de la variadic function todos los mutex y los inicializo
	va_list arguments;
	va_start(arguments, mutex_size);

	for(int i = 0; i < mutex_size; i++) {
		void* mutex = va_arg(arguments, void*);
		pthread_mutex_init(mutex, NULL);
		list_add(mutex_list, mutex);
	}

	va_end(arguments);

	return 1;
}

uint32_t thread_create(pthread_t * thread, void *(*function)(void *)) {

	return pthread_create(thread, &thread_attributes, (void*) function, NULL);
}

uint32_t thread_create_with_parameter(pthread_t * thread, void *(*function)(void *), void * parameter) {

	return pthread_create(thread, &thread_attributes, (void*) function, parameter);
}

void mutex_destroy() {

	list_destroy_and_destroy_elements(mutex_list, (void*) pthread_mutex_destroy);
}

uint32_t set_thread_attributes(){

	//Seteo los threads como detached
	pthread_attr_init(&thread_attributes);
	pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED);

	return 1;
}

void thread_attributes_destroy(){

	pthread_attr_destroy(&thread_attributes);
}
