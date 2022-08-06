#include <inicializacion.h>


void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

/* Inicializo los listas y colas correspondientes a los estados */
void inicializarEstados(){
    log_info(logger, "Kernel - Inicializando estructuras");
    estado_ready = list_create();
    estado_new = queue_create();
    estado_susp_ready = queue_create();
    estado_blocked = queue_create();
}

void inicializarVariablesGlobales(char * pathConfig, char * pathConfigIP){

    config = config_create(pathConfig);
    ips = config_create(pathConfigIP);
    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_INFO);
    log_info(logger, "Kernel - Copiando configuracion de archivos .cfg");

    IP = config_get_string_value(ips, "IP_KERNEL");
    log_info(logger, "Kernel - IP configurado: %s", IP);
    PUERTO_ESCUCHA = config_get_string_value(ips, "PUERTO_KERNEL");
    log_info(logger, "Kernel - PUERTO configurado: %s", PUERTO_ESCUCHA);

    IP_CPU = config_get_string_value(ips, "IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(ips, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(ips, "PUERTO_CPU_INTERRUPT");
    IP_MEMORIA = config_get_string_value(ips, "IP_RAM");
    PUERTO_MEMORIA = config_get_string_value(ips,"PUERTO_ESCUCHA_RAM");
    GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
    ALFA = config_get_double_value(config, "ALFA");
    TIEMPO_MAXIMO_BLOQUEADO = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO")*1000;
}

//TO DO: 
// mostrar_instrucciones_deserializados
