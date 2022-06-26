#include "../include/configKernel.h"
/*
typedef struct kernel_cfg_t{
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_CPU;
    char* PUERTO_CPU_DISPATCH;
    char* PUERTO_CPU_INTERRUPT;
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    char* ESTIMACION_INICIAL;
    char* ALFA;
    char* GRADO_MULTIPROGRAMACION;
    char* TIEMPO_MAXIMO_BLOQUEADO;
} kernel_cfg_t;
*/
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
/*
// Funcion para leer todo lo que tiene el config file 
// y guardarlo en sus respectivas variables
kernel_cfg_t* leer_config_file(t_config *config)
{
    kernel_cfg_t* kernel_cfg;
    
    kernel_cfg->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    kernel_cfg->PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    kernel_cfg->IP_CPU = config_get_string_value(config, "IP_CPU");
    kernel_cfg->PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    kernel_cfg->PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    kernel_cfg->PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    kernel_cfg->ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    kernel_cfg->ESTIMACION_INICIAL = config_get_string_value(config, "ESTIMACION_INICIAL");
    kernel_cfg->ALFA = config_get_string_value(config, "IP");
    kernel_cfg->GRADO_MULTIPROGRAMACION = config_get_string_value(config, "GRADO_MULTIPROGRAMACION");
    kernel_cfg->TIEMPO_MAXIMO_BLOQUEADO = config_get_string_value(config, "TIEMPO_MAXIMO_BLOQUEADO");

    return kernel_cfg;
}

*/

// Funcion que finaliza el config
void terminar_config(t_config* config)
{

	config_destroy(config);

}