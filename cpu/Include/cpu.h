// #ifndef CPU_H_
// #define CPU_H_

// #include <stdio.h>
// #include <stdlib.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <errno.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <math.h>
// #include <commons/string.h>
// #include <commons/log.h>
// #include <commons/config.h>
// #include <commons/collections/dictionary.h>
// #include <commons/collections/queue.h>
// #include <commons/collections/list.h>
// #include <parser/parser.h>
// #include <string.h>
// #include <signal.h>
// #include <stdbool.h>
// #include "handshake.h"
// #include "header.h"
// #include "cliente-servidor.h"
// #include "log.h"
// #include "commonTypes.h"
// #include "serializacion.h"
// #include "primitivas.h"
// #include "conexiones.h"


// //  NO RECUERDO PUNTUALMENTE QUE HACE ESTO  //

// #define ANSI_COLOR_RED     "\t"//\x1b[31m
// #define ANSI_COLOR_GREEN   "\t"//\x1b[32m
// #define ANSI_COLOR_RESET   "\t"//\x1b[0m

// // --------------------------------------- //

// typedef struct customConfig {
// 	int puertoKernel;
// 	char* ipKernel;
// 	int puertoMemoria;
// 	char* ipMemoria;
// 	int DEBUG_IGNORE_MEMORIA;
// 	int DEBUG_IGNORE_PROGRAMS;
// 	int DEBUG_IGNORE_KERNEL;
// 	int DEBUG_RAISE_LOG_LEVEL;
// 	int DEBUG_RUN_TESTS_WITH_MEMORIA;
// 	int DEBUG_RUN_UNITARY_TESTS;
// 	int DEBUG_LOG_ON_TESTS;
// } customConfig_t;

// typedef enum {
// 	DECLARADA, PARAMETRO, NOEXISTE
// } t_localidad_variable;

// /*------------Variables Globales--------------*/
// AnSISOP_funciones funciones;		//funciones de AnSISOP
// AnSISOP_kernel funcionesKernel;		// funciones kernel de AnSISOP

// int tamanioPaginas;					//Tamaño de las paginas
// int cantidadPaginasCodigo;
// int quantum_sleep;  //es lo que tiene que esperar cpu cuando termina de ejecutar una sentencia
// //TODO usar unsigned int??

// t_PCB* pcbActual;
// t_stack* stack;

// char* sentenciaPedida;

// t_config* configCPU;
// customConfig_t config;

// bool terminar;						//flag para que el proceso sepa cuando terminar
// int overflow;
// bool ejecutando;
// bool runOverflowException;
// bool flagMeSalteoTodoConGoto;

// /*----- Operaciones sobre el PC y avisos por quantum -----*/
// void informarInstruccionTerminada(char* sentencia);
// void setearPC(t_PCB* pcb, t_puntero_instruccion pc);
// void incrementarPC(t_PCB* pcb) ;
// void loggearFinDePrimitiva(char* instr);
// void desalojarProceso();


// /*--------FUNCIONES----------*/
// void parsear(char* const sentencia);
// void esperar_programas();
// void procesarHeader(char *header);
// void pedir_tamanio_paginas();
// int longitud_sentencia(t_sentencia* sentencia);
// int obtener_offset_relativo(t_sentencia* fuente, t_sentencia* destino);
// int cantidad_paginas_ocupa(t_sentencia* sentencia);
// int queda_espacio_en_pagina(t_sentencia* sentencia);
// void enviar_solicitud(int pagina, int offset, int size);
// void pedirYRecibirSentencia();
// void esperar_sentencia();
// void enviarPID();
// void obtenerPCB();
// void enviarPCB();
// void obtener_y_parsear();
// void lanzar_excepcion_overflow();
// bool esExcepcion(char* cad);
// void handler(int sign);
// bool hayOverflow();
// void pass();
// void finalizar_proceso_por_variable_invalida();


// // ***** Funciones de conexiones ***** //
// int getHandshake(int cli);
// void warnDebug();
// void conectar_kernel();
// void hacer_handshake_kernel();
// void conectar_memoria();
// void hacer_handshake_memoria();
// void establecerConexionConMemoria();
// void establecerConexionConKernel();

// // ***** Funciones de inicializacion y finalizacion ***** //
// void cargarConfig();
// void inicializar();
// void finalizar();


// // ***** Test ***** //
// void correrTests();  //De cpu.c
// int testear(int (*suite)(void)); //De test.c
// int test_cpu(); //De test.c
// int test_cpu_con_memoria(); //De test.c

// #endif /* CPU_H_ */
