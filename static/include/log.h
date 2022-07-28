#ifndef LOGGER_H_
#define LOGGER_H_

#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>

// Funcion para iniciar un logger, chequea que se haya podido iniciar
t_log* iniciar_logger(char* nombre);

// Funcion que finaliza el logger
void terminar_logger(t_log* logger);

#endif