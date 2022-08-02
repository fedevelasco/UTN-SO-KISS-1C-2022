#ifndef CPU_PROTOCOLO_TRANSFERENCIA_H_
#define CPU_PROTOCOLO_TRANSFERENCIA_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include <estructura.h> // otro static
#include <serialization.h> // nuestro static



void* pcb_serializar_estruc(void* stream, void* estructura, int offset);
int bytes_PCB(t_pcb* pcb);
char* pcb_create_package_with_opcode(t_pcb* pcb, int32_t bytes, t_op_code opcode);
t_pcb* pcb_create_with_size(int32_t size);
t_pcb* pcb_recibir_package(int32_t socket);
t_pcb* pcb_deserializar_estrucs(void* stream, int offset);

#endif