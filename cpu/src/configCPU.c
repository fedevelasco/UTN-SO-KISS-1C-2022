#include "configCPU.h"

typedef struct CPU_cfg{
    char* ENTRADAS_TLB;
    char* REEMPLAZO_TLB;
    char* RETARDO_NOOP;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
} CPU_cfg_t;

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

CPU_cfg_t* leer_config_file(t_config *config)
{
    CPU_cfg_t* kernel_cfg;
    
    CPU_cfg->ENTRADAS_TLB = config_get_string_value(config, "ENTRADAS_TLB");
    CPU_cfg->REEMPLAZO_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
    CPU_cfg->RETARDO_NOOP = config_get_string_value(config, "RETARDO_NOOP");
    CPU_cfg->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    CPU_cfg->PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    CPU_cfg->PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    CPU_cfg->PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    

    return CPU_cfg;
}



// Funcion que finaliza el config
void terminar_config(t_config* config)
{

	config_destroy(config);

}