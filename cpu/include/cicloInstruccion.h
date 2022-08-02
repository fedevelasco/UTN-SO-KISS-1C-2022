#ifndef CPU_CICLO_INSTRUCCION_H_
#define CPU_CICLO_INSTRUCCION_H_

    #include <stdlib.h>
    #include <stdio.h>
	#include <libreriaConexiones.h>

//    #include "../include/sockets.h"
    #include <pthread.h>

    // sin static
    #include "../include/instruccion.h"
    
    // sin static
    #include "../include/serializacion_cpu.h"
    #include "../include/variablesGlobales.h"
    #include "../include/mmu.h"
    #include "../include/protocolo_transferencia.h"
    #include <serializacion.h>  // cambiar ruta
    #include "../include/conexiones.h"
    
    uint32_t tablaPaginasPrimerNivelPCB;
    t_paquete * cicloInstruccion(t_pcb * pcb);
    t_instruccion fetch(t_pcb *  pcb);
    bool execute(t_instruccion instruccion);
    uint32_t  execute_read(uint32_t direccion_logica);
    void execute_write(uint32_t direccion_logica, uint32_t dato);
    uint32_t  memoria_read(uint32_t direccion_fisica);
    void memoria_write(uint32_t direccion_fisica, uint32_t dato);

#endif
