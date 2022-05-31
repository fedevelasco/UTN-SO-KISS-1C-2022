#include <stdlib.h>

typedef struct listas_instrucciones { 
    char instruccion;
    struct listas_instrucciones* sig;

} listas_instrucciones_t;

typedef struct PCB{ // Estructura para PCB
    unsigned short int id;
    int tamanio_bytes;
    listas_instrucciones_t instrucciones;
    short int program_counter;
    // **** Tabla_páginas;
    short int estimacion_ráfaga;

} PCB_t;

// lista enlazada para el estado new
typedef struct estado_new {
    PCB_t nuevo_pcb;
    struct estado_new * next;
} estado_new_t;

//Agregar PCB al final de la lista
void push(estado_new_t* head, PCB_t val) {
    estado_new_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (estado_new_t *) malloc(sizeof(estado_new_t));
    current->next->nuevo_pcb = val;
    current->next->next = NULL;
}

int contador_procesos_new(estado_new_t* lista){

    int contador = 0;
    estado_new_t* current = lista;
    while (current->next != NULL){
        contador++;
        current = current->next;
    }
    return contador;
}

int id_count = 1; //Contador global para asignar IDs a procesos

void creacion_pcb(estado_new_t* lista_new ,listas_instrucciones_t instrucciones, char grado_multiprogramacion){

    PCB_t* aux;
    aux->id = id_count;
    id_count++;
    aux->tamanio_bytes = 19;
    aux->instrucciones = instrucciones;
    //aux->program_counter
    //aux->estimacion_ráfaga



    //Agrega el PCB a la lista de new si es menor al 
    if(contador_procesos_new(lista_new) <= grado_multiprogramacion){

        push(lista_new, aux);

    }
    
    //Enviar a memoria la info para que pasen a ready los procesos
    //siempre y cuando el grado de multiprogramación lo permita



}





