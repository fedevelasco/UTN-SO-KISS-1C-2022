#include <tads/proceso.h>

t_proceso* proceso_crear(){

    t_proceso* proceso = malloc(sizeof(t_proceso));

    proceso->consola = 0;
    proceso->cpu = 0;
    proceso->pcb = pcb_create();
    proceso->socketConsola = 0;
    proceso->socketCPU = 0;
    proceso->rafagas = 0;
    proceso->suspendido = false;

    return proceso;

}

void proceso_liberar(t_proceso* proceso){

    pcb_destroy(proceso->pcb);
    free(proceso);

}

void proceso_asignar_pcb(t_proceso* proceso, t_PCB* pcb){

    proceso->pcb = pcb;

}

void proceso_asignar_socket_de_cliente(t_proceso* proceso, uint32_t cliente_socket){

    proceso->socketConsola = cliente_socket;

}

void proceso_asignar_socket_de_cpu(t_proceso* proceso, uint32_t cpu_socket){

    proceso->socketCPU = cpu_socket;

}
