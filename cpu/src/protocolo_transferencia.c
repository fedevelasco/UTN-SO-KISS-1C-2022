#include "../include/protocolo_transferencia.h"


// --------- serializa el stream y devuelve el offset ------------
void* pcb_serializar_estruc(void* stream, void* estructura, int offset){
	
	t_pcb* pcb = (t_pcb*) estructura;

    offset += serialize_int(stream + offset, &(pcb->id));
    offset += serialize_int(stream + offset, &(pcb->tamanioProceso));
    offset += serialize_int(stream + offset, &(pcb->PC));
 	offset += serialize_int(stream + offset, &(pcb->tablaDePaginas));
    offset += serialize_int(stream + offset, &(pcb->estimacionRafaga));
	offset += serialize_int(stream + offset, &(pcb->lengthUltimaRafaga));
	offset += serialize_int(stream + offset, &(pcb->sizeInstrucciones));
	offset += serialize_instructions_list (stream + offset, pcb->instrucciones);

    return offset;
}
// ------------- Devuelve un pcb deserializado a partir de un stream de datos ----------------
t_pcb* pcb_deserializar_estrucs(void* stream, int offset){
	
	t_pcb* pcb = malloc(sizeof(t_pcb));
	offset += deserialize_int(&(pcb->id), stream + offset);
	offset += deserialize_int(&(pcb->tamanioProceso), stream + offset);
	offset += deserialize_int(&(pcb->PC), stream + offset);
    offset += deserialize_int(&(pcb->tablaDePaginas), stream + offset);
	offset += deserialize_int(&(pcb->estimacionRafaga), stream + offset);
	offset += deserialize_int(&(pcb->lengthUltimaRafaga), stream + offset);
	offset += deserialize_int(&(pcb->sizeInstrucciones), stream + offset);
	offset += deserialize_instructions_list(pcb->instrucciones, stream + offset);    

	return pcb;
}
// --------- Caculo los bytes para el pcb ------------
int bytes_PCB(t_pcb* pcb){
    int temp = sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
}
// --------- armo un pcb serializado y me arma el stream a mandar de una ------------
t_paquete * pcb_create_package_with_opcode(t_pcb* pcb, t_op_code opcode){
    t_paquete * paquete = crearPaquete(opcode);
    paquete->buffer->size = bytes_PCB(pcb);
    pcb_serializar_estruc(paquete->buffer->stream, pcb, 0); 
    
	// char * output  = malloc(bytes);
	// memset(output, 0, bytes);

	// int32_t offset = 0;

	// offset += serialize_opcode(output + offset, opcode);
	// //offset += pcb_serializar(pcb, output + offset);
	// pcb_serializar_estruc(output, pcb, offset);

	return paquete;
}
// ----------- Creo pcb para la deserializacion -----------
t_pcb* pcb_create_with_size(int32_t size){ // Uso el pcb create con size para cuando deserealizo un PCB
	t_pcb* pcb = malloc(size);
	pcb->id= 0;
    pcb->tamanioProceso = 0;
	pcb->PC = 0;
    pcb->tablaDePaginas = 0;
    pcb->estimacionRafaga = 0;
	pcb->lengthUltimaRafaga = 0;
	pcb->lengthUltimaRafaga = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
	return pcb;
}
// ----------- recibo stream de datos post opcode y me genera un pcb ----------------
t_pcb* pcb_recibir_package(int32_t socket){
	int32_t bytes;
	recv(socket, bytes, sizeof(int32_t), 0);
	char* a_deserializar;
	recv(socket, a_deserializar, bytes, 0);
	t_pcb* temp = pcb_create_with_size(bytes);
	temp = pcb_deserializar_estrucs(a_deserializar, 0);
	return temp;
}

// 1: int32_t cod_op = recibir_operacion(cliente_fd)
// 2: t_pcb pcb = pcb_recibir_package(int32_t socket)