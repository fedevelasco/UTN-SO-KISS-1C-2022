#ifndef CPU_CICLO_INSTRUCCION_H_
#define CPU_CICLO_INSTRUCCION_H_

#include <stdlib.h>
#include <stdio.h>
#include <libreriaConexiones.h>
#include <pthread.h>
#include <global_structures.h>
#include <mmu.h>
#include <protocolo_transferencia.h>
#include <serialization.h>
#include <conexiones.h>

uint32_t tablaPaginasPrimerNivelPCB;
t_paquete * cicloInstruccion(t_pcb* pcb);
t_instruction* fetch(t_pcb* pcb);
bool execute(t_instruction* instruccion);
uint32_t execute_read(uint32_t direccion_logica);
void execute_write(uint32_t direccion_logica, uint32_t dato);
uint32_t memoria_read(uint32_t direccion_fisica);
void memoria_write(uint32_t direccion_fisica, uint32_t dato);

#endif
