#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>

// Funcion para iniciar un logger, chequea que se haya podido iniciar
t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("kernel.log","Kernel", 1, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL){
		printf("Error al abrir el kernel.log\n");
		exit(1);
	}

	return nuevo_logger;
}

// Funcion que finaliza el logger
void terminar_logger(t_log* logger)
{
	log_destroy(logger);
}

int main() {

    // Inicio un logger
    t_log* logger = iniciar_logger();

    // Logueo que se realizo correctamente
    log_info(logger, "Log de Kernel iniciado correctamente\n");

	// Finalizo el programa
	terminar_logger(logger);

}