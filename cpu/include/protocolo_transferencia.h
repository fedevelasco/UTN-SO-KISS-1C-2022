#ifndef CPU_PROTOCOLO_TRANSFERENCIA_H_
#define CPU_PROTOCOLO_TRANSFERENCIA_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include <global_structures.h>
#include <serialization.h>


void* pcb_serializar_estruc(void* stream, void* estructura, int offset);
t_pcb* pcb_deserializar_estrucs(void* stream, int offset);

uint32_t bytes_PCB(t_pcb* pcb);
t_paquete * pcb_create_package_with_opcode(t_pcb* pcb, t_op_code opcode);
t_pcb* pcb_create_with_size(int32_t size);
t_pcb* pcb_recibir_package(int32_t socket);
void freePCB(t_pcb * pcb);

void * serializarIO(void* stream, void* estructura);
t_IO * deserializarIO(void* stream);

void * serializarUINT32_T(void* stream, void* estructura);
uint32_t * deserializarUINT32_T(void* stream);

void * serializarMensaje(void* stream, void* estructura);
t_mensaje * deserializarMensaje(void* stream);

void* serializarEstructura(void* estructura, int tamanio, t_op_code cod_op);

void crearBuffer(t_paquete* paquete);
t_paquete* crearPaquete(t_op_code cod_op);
t_paquete* armarPaqueteCon(void* estructura, t_op_code cod_op);
void* serializarPaquete(t_paquete* paquete, uint32_t bytes);
void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio);
void enviarPaquete(t_paquete* paquete, int socket_cliente);
void eliminarPaquete(t_paquete* paquete);

uint32_t tamanioEstructura(void* estructura ,t_op_code cod_op);



#endif
