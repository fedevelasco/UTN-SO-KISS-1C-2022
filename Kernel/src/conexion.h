#include "libreriaConexiones.h"

#include <pthread.h>

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

void* recibir_buffer(int32_t* size, int32_t socket_cliente, t_log* logger);

t_package* recibir_package(int32_t socket_cliente, t_log* logger);

static void procesar_conexion(void* void_args);

int server_escuchar(t_log* logger, char* server_name, int server_socket);


// Funciones de ejemplo
static void* serializar_mirar_netflix(size_t* size, char* peli, uint8_t cant_pochoclos);

static void deserializar_mirar_netflix(void* stream, char** peli, uint8_t* cant_pochoclos);

bool send_mirar_netflix(int fd, char* peli, uint8_t cant_pochoclos);

bool recv_mirar_netflix(int fd, char** peli, uint8_t* cant_pochoclos);

