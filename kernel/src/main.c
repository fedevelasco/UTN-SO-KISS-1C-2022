#include <main.h>

/* ------------------- Creo y asigno las estructuras proceso, PCB y consola. Agrego el PCB al estado new ---------------- */
void manejarProceso(int* socket_consola){

    t_proceso* nuevo_proceso = malloc(sizeof(t_proceso));
   
    nuevo_proceso = recibirPaqquete_inicio(*socket_consola);
    
    // Print de lista de instrucciones

    t_pcb* pcb = iniciarPcb(nuevo_proceso); // Creo un PCB y le asigno los valores recolectados en nuevo_proceso
    
    t_consola* nuevaConsola = malloc(sizeof(t_consola)); // Almaceno la info de la consola en nuevaConsola
    nuevaConsola->id = pcb->id; 
    nuevaConsola->socket = socket_consola;

    pthread_mutex_lock(&mutex_consolas_conectadas); 
    list_add(consolas_conectadas, (void *) nuevaConsola); // Agrego a la lista la consola que cree
    pthread_mutex_unlock(&mutex_consolas_conectadas);
    
    ingresarANew(pcb); // Ingreso el PCB al estado new   
}


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    char * pathConfig = argv[1];
    char * pathIPS = argv[2];

    /* Inicializo la config y asigno los valores a las variables globales */
    inicializarVariablesGlobales(pathConfig, pathIPS);

    /* Inicializacion de los mutex */
    pthread_mutex_init(&mutex_estado_new, (void *)NULL);
    pthread_mutex_init(&mutex_estado_ready, (void *)NULL);
    pthread_mutex_init(&mutex_pcb_ejecutando, (void *)NULL);
    pthread_mutex_init(&mutex_estado_exit, (void *)NULL);
    pthread_mutex_init(&mutex_estado_blocked, (void *)NULL);
    pthread_mutex_init(&mutex_estado_susp_ready, (void*)NULL);
    pthread_mutex_init(&mutex_consolas_conectadas, (void*)NULL);
     
    /* Creo la lista para almacenar consolas */
    consolas_conectadas = list_create();

    /* Creo los semaforos y los inicio con los valores correspondientes */
    sem_init(&sem_multiprogramacion, 0, GRADO_MULTIPROGRAMACION);
    sem_init(&sem_hay_pcb_en_new, 0, 0);
    sem_init(&sem_ready, 0, 0);
    sem_init(&sem_fin_proceso, 0, 0);   
    sem_init(&sem_block, 0, 0);
    sem_init(&sem_susp_ready, 0, 0);
    sem_init(&sem_hay_pcb_esperando_ready, 0, 0);
    
    /* Inicializo el sevidor en modo escucha */
    int* socket = malloc(sizeof(int));
    char* nombre = "KERNEL";
    *socket = iniciar_servidor(logger, nombre, IP, PUERTO_ESCUCHA);

    /* Inicializo los listas y colas correspondientes a los estados */
    inicializarEstados();

    /* Creo los hilos encargados de mover los procesos entre estados y ejecutar la logica */
    inicializarPlanificacion();


    //atender consola
    while(1){

        int* socket_consola = malloc(sizeof(int));
        *socket_consola = esperar_cliente(logger, nombre, *socket);

        //log_info(logger, "Se conecto cliente");
        manejarProceso(socket_consola);
    }
    
    return 0;
}
