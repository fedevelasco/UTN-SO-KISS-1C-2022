#include "conexion.h"

// -------------- Estructuras para recibir la informacion --------------


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

//while(server_escuchar(.., .., ..)); Esto va a escuchar todos los clientes Consola que se conecten




// -------------- Funciones para recibir procesos y deserializar --------------


static void* serializar_mirar_netflix(size_t* size, char* peli, uint8_t cant_pochoclos) {
    size_t size_peli = strlen(peli) + 1;
    *size =
          sizeof(op_code)   // cop
        + sizeof(size_t)    // total
        + sizeof(size_t)    // size de char* peli
        + size_peli         // char* peli
        + sizeof(uint8_t);  // cant_pochoclos
    size_t size_payload = *size - sizeof(op_code) - sizeof(size_t);

    void* stream = malloc(*size);

    op_code cop = MIRAR_NETFLIX;
    memcpy(stream, &cop, sizeof(op_code));
    memcpy(stream+sizeof(op_code), &size_payload, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t), &size_peli, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t)*2, peli, size_peli);
    memcpy(stream+sizeof(op_code)+sizeof(size_t)*2+size_peli, &cant_pochoclos, sizeof(uint8_t));

    return stream;
}

static void deserializar_mirar_netflix(void* stream, char** peli, uint8_t* cant_pochoclos) {
    // Peli
    size_t size_peli;
    memcpy(&size_peli, stream, sizeof(size_t));

    char* r_peli = malloc(size_peli);
    memcpy(r_peli, stream+sizeof(size_t), size_peli);
    *peli = r_peli;

    // Pochoclos
    memcpy(cant_pochoclos, stream+sizeof(size_t)+size_peli, sizeof(uint8_t));
}

bool send_mirar_netflix(int fd, char* peli, uint8_t cant_pochoclos) {
    size_t size;
    void* stream = serializar_mirar_netflix(&size, peli, cant_pochoclos);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_mirar_netflix(int fd, char** peli, uint8_t* cant_pochoclos) {
    size_t size_payload;
    if (recv(fd, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;

    void* stream = malloc(size_payload);
    if (recv(fd, stream, size_payload, 0) != size_payload) {
        free(stream);
        return false;
    }

    deserializar_mirar_netflix(stream, peli, cant_pochoclos);

    free(stream);
    return true;
}
