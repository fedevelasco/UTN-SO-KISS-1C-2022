#ifndef STATIC_SERIALIZACION_ESTRUCTURAS_H_
#define STATIC_SERIALIZACION_ESTRUCTURAS_H_

	#include <stdlib.h>
	#include <stdint.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <commons/collections/list.h>
	#include <commons/collections/queue.h>
	#include <tads/op_code.h>
	#include <tads/instructions_list.h>
    #include <tads/package.h>
   	#include <tads/buffer.h>
	#include <tads/process.h>
	#include <serialization.h>
	
	typedef struct{
        uint32_t size;
        void* stream;
    }t_buffer_new;

	typedef struct{
        t_op_code codigo_operacion;
	    t_buffer_new* buffer;
    }t_paquete;


	typedef struct{
		uint32_t tamanioProceso;
		uint32_t sizeInstrucciones;
		t_instructions_list* instrucciones;
	}t_proceso;

	typedef struct{
		uint32_t tamanio_pagina;
		uint32_t paginas_por_tabla;
	}t_traduccion_direcciones;

	typedef struct{
		uint32_t longitud;
		char * texto;
	}t_mensaje;


	typedef struct{
		uint32_t id;
		uint32_t tamanioProceso;
		uint32_t PC;
		uint32_t tablaDePaginas;
		uint32_t estimacionRafaga;
		uint32_t lengthUltimaRafaga;
		uint32_t sizeInstrucciones;
		t_instructions_list* instrucciones;
	}t_pcb;

	typedef struct{
        t_pcb * pcb;
        uint32_t tiempoBloqueo;
		uint32_t tiempoEspera;
    }t_IO;
	
	typedef struct{
		uint32_t tablaDePaginas;
		uint32_t entradaPagina;
		uint32_t id;
	}t_consultaTablaPagina;

	typedef struct{
		uint32_t direccionFisica;
		uint32_t dato;
		uint32_t id;
	}t_peticionEscritura;

	char * codOPtoString(t_op_code);

	void* serializarEstructura(void* estructura,int tamanio,t_op_code codigoOperacion);
	int tamanioEstructura(void* estructura ,t_op_code cod_op);
	
	void crearBuffer(t_paquete* paquete);

    t_paquete* crearPaquete(t_op_code cod_op);
	int tamanioEstructura(void* estructura ,t_op_code cod_op);
    t_paquete* armarPaqueteCon(void* estructura,t_op_code cod_op);
    void* serializarPaquete(t_paquete* paquete, uint32_t bytes);
    void enviarPaquete(t_paquete* paquete, int socket_cliente);
    void eliminarPaquete(t_paquete* paquete);
	

	t_traduccion_direcciones* crearTraduccionDirecciones(uint32_t PID);
	void * serializarTraduccionDirecciones(void* stream, void* estructura);
	t_traduccion_direcciones* deserializarTraduccionDirecciones(void* stream);
	

	t_mensaje*  deserializarMensaje(void* stream);
	void*  serializarMensaje(void* stream, void* estructura);

	// void* serializarPCB(void* stream, void* estructura, int offset);
	// t_pcb* deserializarPCB(void* stream, int offset);
	void* pcb_serializar_estruc(void* stream, void* estructura, int offset);//nuevo
	t_pcb* pcb_deserializar_estrucs(void* stream, int offset);//nuevo

	int bytes_PCB_struc(t_pcb* pcb);//nuevo
	char* pcb_create_package_with_opcode(t_pcb* pcb, int32_t bytes, t_op_code opcode);//nuevo
	t_pcb* pcb_create_with_size_for_des(int32_t size);
	void freePCB(t_pcb* pcb);
	
	void*  serializarIO(void* stream, void* estructura);
	t_IO* deserializarIO(void* stream);

	void* serializarUINT32_T(void* stream, void* estructura);
	uint32_t* deserializarUINT32_T(void* stream);

	t_consultaTablaPagina* deserializarConsultaTablaPagina(void* stream);
	void* serializarConsultaTablaPagina(void* stream, void* estructura);

	t_peticionEscritura* deserializarPeticionEscritura(void* stream);
	void* serializarPeticionEscritura(void* stream, void* estructura);

#endif
