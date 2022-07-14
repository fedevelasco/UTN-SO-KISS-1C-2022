#include "../../include/tads/pcb.h"

t_PCB* pcb_create(){
	t_PCB* pcb = malloc(sizeof(t_PCB));
	pcb->PID = 0;
    pcb->TAMANIO_PROCESO = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
    pcb->PC = 0;
    pcb->TABLA_DE_PAGINAS = 0;
    pcb->ESTIMACION_RAFAGA = 0;
	return pcb;
}

t_PCB* pcb_create_with_size(int32_t size){
	t_PCB* pcb = malloc(size);
	pcb->PID = 0;
    pcb->TAMANIO_PROCESO = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
    pcb->PC = 0;
    pcb->TABLA_DE_PAGINAS = 0;
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

int32_t pcb_serializar(t_PCB* pcb, t_log* logger, char* output){

    /*
        int PID; // identificador único
        int32_t TAMANIO_PROCESO; // tamanio del proceso
        t_instructions_list* LISTA_INSTRUCCIONES; // Lista de instrucciones a ejecutar
	    int PC;	 // Program Counter
	    // Tabla de paginas A DEFINIR
	    short int ESTIMACION_RAFAGA;
    */

	int32_t offset = 0;

    offset += serialize_int(output + offset, &(pcb->PID));
    offset += serialize_int(output + offset, &(pcb->TAMANIO_PROCESO));
    offset += serialize_instructions_list (output + offset, pcb->LISTA_INSTRUCCIONES);
    offset += serialize_int(output + offset, &(pcb->PC));
    offset += serialize_int(output + offset, &(pcb->TABLA_DE_PAGINAS));
    offset += serialize_int(output + offset, &(pcb->ESTIMACION_RAFAGA));

    return offset;
}

int bytes_PCB(t_PCB* pcb){
    int temp = sizeof(pcb->PID) + sizeof(pcb->TAMANIO_PROCESO) + sizeof(pcb->PC) + sizeof(pcb->ESTIMACION_RAFAGA) + bytes_instructions_list(pcb->LISTA_INSTRUCCIONES);
}

int32_t pcb_deserializar(t_PCB* destino, char* fuente){ // INT devolvia 0

	int32_t offset=0;

    //int32_t size = bytes_PCB(destino);

	//destino = pcb_create_with_size(size);

	offset += deserialize_int(&(destino->PID), fuente + offset);
	offset += deserialize_int(&(destino->TAMANIO_PROCESO), fuente + offset);
    offset += deserialize_instructions_list(destino->LISTA_INSTRUCCIONES, fuente + offset);
	offset += deserialize_int(&(destino->PC), fuente + offset);
    offset += deserialize_int(&(destino->TABLA_DE_PAGINAS), fuente + offset);
	offset += deserialize_int(&(destino->ESTIMACION_RAFAGA), fuente + offset);    

    return 0;
}



