#include "../../include/tads/pcb.h"

t_PCB* pcb_create(){
	t_PCB* pcb = malloc(sizeof(t_PCB));
	pcb->PID = 0;
    pcb->TAMANIO_PROCESO = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
    pcb->PC = 0;
    pcb->ESTIMACION_RAFAGA = 0;
	return pcb;
}

void pcb_destroy(t_PCB* pcb){
    instructions_list_destroy(pcb->LISTA_INSTRUCCIONES);
    free(pcb);
}

void pcb_imprimir(t_PCB* pcb, t_log* logger){
    log_info(logger, "Print de PCB: \n");
    log_info(logger, "PID = %i", pcb->PID);
    log_info(logger, "Size = %i", pcb->TAMANIO_PROCESO);
    log_info(logger, "Instruction List ="); 
    imprimir_lista_instrucciones(pcb->LISTA_INSTRUCCIONES, logger);
    log_info(logger, "PC = %i\n", pcb->PC);
}

void pcb_asignar_lista_instrucciones(t_PCB* pcb, t_instructions_list* lista_instrucciones){

    pcb->LISTA_INSTRUCCIONES = lista_instrucciones;

}

void pcb_asignar_lista_tamanio(t_PCB* pcb, t_instructions_list* lista_instrucciones){

    pcb->TAMANIO_PROCESO = lista_instrucciones->process_size;

}

void pcb_asignar_id_unico(t_PCB* pcb, t_log* logger){
    
    static short int id_unico = 0;
    pcb->PID = id_unico;
    id_unico++;

}



