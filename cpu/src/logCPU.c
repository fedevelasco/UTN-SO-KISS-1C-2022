#include "../include/logCPU.h"

// Funcion para iniciar un logger, chequea que se haya podido iniciar
t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("CPU.log","CPU", 1, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL){
		printf("Error al abrir el CPU.log\n");
		exit(1);
	}

	return nuevo_logger;
}

// Funcion que finaliza el logger
void terminar_logger(t_log* logger)
{
	log_destroy(logger);
}