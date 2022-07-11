#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>

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
t_config* iniciar_config(void);

// Funcion para leer todo lo que tiene el config file 
// y guardarlo en sus respectivas variables
CPU_cfg_t* leer_config_file(t_config *config);

// Funcion que finaliza el config
void terminar_config(t_config* config);