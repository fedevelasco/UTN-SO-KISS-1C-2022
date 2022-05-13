#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>

// Funcion que inicia el config para poder tomar los datos del configuracion.txt
t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("configuracion.config");

	if (nuevo_config == NULL){
		printf("Error al abrir el configuracion.config\n");
		exit(2);
	}

	return nuevo_config;
}

// Funcion para leer todo lo que tiene el config file 
// y guardarlo en sus respectivas variables
void leer_config_file(char *IP_MEMORIA, char *PUERTO_MEMORIA, char *IP_CPU, char *PUERTO_CPU_DISPATCH, char *PUERTO_CPU_INTERRUPT, char *PUERTO_ESCUCHA, char *ALGORITMO_PLANIFICACION, char *ESTIMACION_INICIAL, char *ALFA, char *GRADO_MULTIPROGRAMACION, char *TIEMPO_MAXIMO_BLOQUEADO, t_config *config)
{
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    IP_CPU = config_get_string_value(config, "IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ESTIMACION_INICIAL = config_get_string_value(config, "ESTIMACION_INICIAL");
    ALFA = config_get_string_value(config, "IP");
    GRADO_MULTIPROGRAMACION = config_get_string_value(config, "GRADO_MULTIPROGRAMACION");
    TIEMPO_MAXIMO_BLOQUEADO = config_get_string_value(config, "TIEMPO_MAXIMO_BLOQUEADO");
}


// Funcion que finaliza el config
void terminar_config(t_config* config)
{

	config_destroy(config);

}