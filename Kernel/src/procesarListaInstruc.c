#include "procesarListaInstruc.h"

/*
typedef struct { //Struc de instrucciones de Fede
	    int32_t id_length;
		char* id;
		int32_t* parameters;
		int32_t cantParameters;
	} t_instruction;

typedef struct { //Struc lista de instrucciones de Fede
		int32_t process_size;
		t_list *instructions;
	} t_instructions_list;

typedef struct {
        unsigned int id; //Id: identificador del proceso.
        int32_t tamanio; //Tamaño: tamaño en bytes del proceso
        t_instructions_list* lista_instrucciones; //Instrucciones: lista de instrucciones a ejecutar.
        unsigned int pc; //Program Counter: número de la próxima instrucción a ejecutar.
        //Tabla_páginas: identificador de tabla de páginas del proceso en memoria, esta información la tendremos recién cuando el proceso pase a estado READY.
        //Estimación_ráfaga: estimación utilizada para planificar los procesos en el algoritmo SRT, la misma tendrá un valor inicial definido por archivo de configuración y será recalculada bajo la fórmula de promedio ponderado vista en clases.
    } t_pcb;

unsigned int global_id = 0;

t_queue* cola_new; // Cola del estado READY



t_pcb* crear_pcb(t_instructions_list* lista, t_log* logger) //Crea un PCB a partir de una lista de instrucciones
{
    log_info(logger, "Creando PCB:\n");
    t_pcb* pcb;
    pcb->id = global_id;
    log_info(logger, "PCB id = %i\n", global_id);
    pcb->tamanio = lista->process_size;
    global_id ++;
    pcb->lista_instrucciones = lista;
    pcb->pc = 0;

    return pcb;
}

/*
PLANIFICADOR LARGO PLAZO
Al conectarse una consola al kernel, deberá generarse la estructura PCB detallada anteriormente y asignarse este proceso al estado NEW.
En caso de que el grado de multiprogramación lo permita, los procesos pasarán al estado READY, enviando un mensaje al módulo Memoria para que inicialice sus estructuras necesarias y obtener el valor de la tabla de páginas que deberá estar almacenado en nuestro PCB.
Cuando se reciba un PCB con motivo de finalizar el mismo, se deberá pasar al proceso al estado EXIT y dar aviso al módulo Memoria para que éste libere sus estructuras. Una vez liberadas, se dará aviso a la Consola de la finalización del proceso.

void planificador_largo_plazo(t_instructions_list* lista_nueva, int grado_multiprogramacion, t_log* logger)
{
    log_info(logger, "Planificador a Largo Plazo: Creando PCB")
    t_pcb* pcb_nuevo = crear_pcb(lista_nueva);
    log_info(logger, "Planificador a Largo Plazo: Asignando PCB a la cola de READY")
    queue_push(cola_new, pcb_nuevo);

    if (  <=grado_multiprogramacion)

}


*/