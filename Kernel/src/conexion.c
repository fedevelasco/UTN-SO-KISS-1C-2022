#include "/home/utnso/tp-2022-1c-Grupo-TP-SO/static/include/libreriaConexiones.h"
#include "../include/configKernel.h"
#include <pthread.h>

// -------------- Estructuras para recibir la informacion --------------

/*
typedef struct { // Estructura para poder pasarle info a los threads
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

typedef enum
{
	INSTRUCTIONS,
	EXIT
}op_code; // el operation code te permite identificar que tipo de operacion esta queriendo / estoy queriendo realizar

typedef struct {
		int32_t process_size;
		t_list *instructions;
	} t_instructions_list;

typedef struct
{
	int32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code operation_code;
	t_buffer* buffer;
} t_package;



// -------------- Funciones para recibir procesos --------------

void* recibir_buffer(int32_t* size, int32_t socket_cliente, t_log* logger)
{
	void * buffer;

	log_info(logger, "Conexion con Consola - Creando buffer");
	// Devuelve el tamanio de lo que recibe y guarda el buffer en la variable size
	recv(socket_cliente, size, sizeof(int32_t), MSG_WAITALL); 
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

t_package* recibir_package(int32_t socket_cliente, t_log* logger)
{
	int32_t size;
	int desplazamiento = 0;
	void * buffer;
	t_package* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente, logger);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int32_t));
		desplazamiento+=sizeof(int32_t);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}


// -------------- Funciones para crear conexiones tipo Server y abrir hilos --------------

static void procesar_conexion(void* void_args) { // Esta funcion va a correr el hilo
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    op_code cop;
    while (cliente_socket != -1) {
    //El choclo de bytes que recibo viene encabezado por el op_code
        if (recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) { 
            //Al recv le paso un buffer a donde leer (cop) y le digo cuando leer (sizeof op_code)
            //Si leyo correctamente entonces tiene que coincidir la cantidad de bytes.
            log_info(logger, "DISCONNECT!");
            return;
        }

        switch (cop) { //Analizo el op_code para ver que estoy queriendo mandar
            case EXIT:
                log_info(logger, "Exit Program");
                break;

            case INSTRUCTIONS:
            {
				// Competar
                break;
            }

            // Errores
            case -1:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                return;
            default:
                log_error(logger, "Algo anduvo mal en el server de %s", server_name);
                log_info(logger, "Cop: %d", cop);
                return;
        }
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(server_socket, logger);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

*/



// -------------- Funciones para recibir procesos y deserializar --------------



// TESTING
int main(){
    char* nombre = malloc(sizeof(char));
    nombre = "KERNEL_TESTING";
    t_log* logger = iniciar_logger(nombre);
    
    log_info(logger, "Iniciando conexion del servidor");
    int32_t server_fd = iniciar_servidor(logger, "127.0.0.1", "8000");
    
    log_info(logger, "Esperando conexion del cliente");
    int32_t conexion_fd = esperar_cliente(server_fd, logger);

    
    
}