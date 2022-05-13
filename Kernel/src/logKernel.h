#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>

// Funcion para iniciar un logger, chequea que se haya podido iniciar
t_log* iniciar_logger(void);

// Funcion que finaliza el logger
void terminar_logger(t_log* logger);