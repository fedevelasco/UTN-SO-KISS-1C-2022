#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>

// Funcion que inicia el config para poder tomar los datos del configuracion.txt
t_config* iniciar_config(void);


// Funcion para leer todo lo que tiene el config file 
// y guardarlo en sus respectivas variables
void leer_config_file(char *IP_MEMORIA, char *PUERTO_MEMORIA, char *IP_CPU, char *PUERTO_CPU_DISPATCH, char *PUERTO_CPU_INTERRUPT, char *PUERTO_ESCUCHA, char *ALGORITMO_PLANIFICACION, char *ESTIMACION_INICIAL, char *ALFA, char *GRADO_MULTIPROGRAMACION, char *TIEMPO_MAXIMO_BLOQUEADO, t_config *config);



// Funcion que finaliza el config
void terminar_config(t_config* config);
