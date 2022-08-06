#include <estructurasEnlazadas.h>

static unsigned long obj_tid = 0; 

bool filter_t_PCB_by_tid(void *item){
  t_PCB* t_r = (t_PCB*) item;
  return t_r->PID == obj_tid;
}

// Funciones de cola NEW 

void push_cola_new(t_PCB* pcb){ // Agregar nuevo elemento a la cola NEW
  queue_push(COLA_NEW, pcb);   
}

t_PCB* pop_cola_new(){
  t_PCB* element = (t_PCB*) queue_pop(COLA_NEW); // Quitar y devolver un elemento a la cola NEW
  return element;
}

int largo_cola_new() { // Devuelve el tamanio de la cola
  int ret = queue_size(COLA_NEW);
  return ret;
}

t_PCB* buscar_cola_new(unsigned long tid){ // Busca y retorna un elemento de la cola NEW
  obj_tid = tid;
  t_PCB* ret = list_find(COLA_NEW->elements, filter_t_PCB_by_tid);
  return ret; 
}

void* remover_cola_new(unsigned long tid){ // Elimina un elemento de la cola NEW
  obj_tid = tid;
  void* ret = list_remove_by_condition(COLA_NEW->elements, filter_t_PCB_by_tid); 
  return ret;
}

// Funciones de lista READY

void push_lista_ready(t_PCB* pcb){
  list_add(LISTA_READY, pcb);
  //send_carpincho_ready(KERNEL_CFG->MEMORIA_FD, carpincho->id);
}

int largo_lista_ready() {
  int ret = list_size(LISTA_READY);
  return ret;
}

// Funciones de cola EXIT

void push_cola_exit(t_PCB* pcb){
  queue_push(COLA_EXIT, pcb);
}
