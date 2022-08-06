#include "../include/cpu_alt.h" 


void iniciarHilos(){
    log_info(logger, "Iniciando hilos");
    char* dispatch = "Dispatch";
    char* interrupt = "Interrupt";

    socket_dispatch = iniciar_servidor(logger, dispatch, IP, PUERTO_ESCUCHA_DISPATCH);

    socket_interrupt = iniciar_servidor(logger, interrupt, IP, PUERTO_ESCUCHA_INTERRUPT);

    pthread_t thread_dispatch, thread_interrupt;
    servidor_dispatch = obtenerServidor(socket_dispatch, deserializarDispatch, "dispatch");

    
    pthread_create(&thread_dispatch, NULL, (void*)servidor, (void*)servidor_dispatch);
    pthread_create(&thread_interrupt, NULL, (void*)servidorInterrupt, (void*)socket_interrupt);
    
    
    pthread_detach(thread_interrupt);
    pthread_join(thread_dispatch, NULL);

}
void inicializarVariablesGlobales(char * pathConfig,char * pathConfigIP){
    config = config_create(pathConfig);
    ips = config_create(pathConfigIP);
    IP = config_get_string_value(ips, "IP_CPU");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(ips, "PUERTO_CPU_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(ips, "PUERTO_CPU_INTERRUPT");
    IP_MEMORIA = config_get_string_value(ips, "IP_RAM");
    PUERTO_MEMORIA = config_get_string_value(ips, "PUERTO_ESCUCHA_RAM");
    RETARDO_NOOP = config_get_int_value(config, "RETARDO_NOOP") / 1000;
    ENTRADAS_TLB = config_get_int_value(config, "ENTRADAS_TLB");
    REEMPLAZO_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
    hayInterrupcion = false;
    char* r = "\r";
    if(string_ends_with(IP, r)){
    	log_debug(logger, "WARNING!: ip termina con \r: %s", IP);
    }
    pthread_mutex_init(&mutex_interrupcion, NULL);
    log_info(logger, "Variables de configuracion Leidas: ip_cpu %s", IP);
}
int main(int argc, char* argv[]) {
    logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);

    if(argc < 2) {
			log_error(logger, "Error en argumentos - Es obligatorio especificar path de config y de ips - Fin proceso");
			//end_process();
			return EXIT_FAILURE;
		}
        
    log_info(logger, "Parametros Validados");
    char * pathConfig = argv[1];
    char * pathIPS = argv[2];
    inicializarVariablesGlobales(pathConfig, pathIPS);
    //handshake con memoria

    log_info(logger, "IP_MEMORIA: %s, PORT: %s", IP_MEMORIA, PUERTO_MEMORIA);

//    int socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA, logger);
    traduccion_direcciones = obtenerTraduccionDeDirecciones();
//    close(socket_memoria);
    log_info(logger, "Traduccion de direcciones obtenida de memoria: tamanio_pagina=%d, paginas_por_tabla=%d ", traduccion_direcciones->page_size, traduccion_direcciones->pages_per_table);
    iniciarEstructurasMMU();
    iniciarHilos();
    
    //liberar heap
    config_destroy(config);
    config_destroy(ips);
    memory_config_destroy(traduccion_direcciones);
    log_destroy(logger);
    return 0;
}











//main