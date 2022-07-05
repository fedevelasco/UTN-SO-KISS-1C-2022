#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include<commons/config.h>

#include "conexion.h"
#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/libreriaConexiones.h"
#include "planificacion.h"

t_config* iniciar_config(void);
void terminar_config(t_config* config);



#endif /* SERVER_H_ */
