#include "../include/servidor_abstracto.h"

void servidor(void * params){
    t_servidor * servidor = (t_servidor *)params;
    while(1){
//        int socket_cliente = esperar_cliente(*servidor->socket);
        char* cliente = "Cliente";
        int socket_cliente = esperar_cliente(logger, cliente, servidor->socket);
        log_info(logger, "%s: se conecta kernel en el server interrupt", servidor->nombre);
        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        
        if (string_equals_ignore_case(REQ_INTERRUPCION_KERNEL_CPU,paquete->codigo_operacion)){ // si matchea entra al if
            deserializarInterrupt(paquete, socket_cliente); // esto setea interrupcion a true REVISAR
        }
        servidor->deserializarSegun(paquete, socket_cliente);
    }
}

t_servidor * obtenerServidor(int socket, void(*deserializarSegun)(t_paquete*,int), char nombre[10]){
    t_servidor * servidor = malloc(sizeof(t_servidor));
    servidor->socket = socket;
    servidor->deserializarSegun=deserializarSegun;
    memcpy(servidor->nombre, nombre, 10);
    return servidor;
}
