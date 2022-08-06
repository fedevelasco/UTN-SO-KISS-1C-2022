#include "../include/mmu.h"

void iniciarEstructurasMMU() {
    tiempoAccesoGlobal = 0;
    indiceFIFO = 0;
    listaTLB = list_create();
    PCB_ACTUAL = 0;
}

t_EntradaTLB * newEntradaTLB(uint32_t pagina, uint32_t marco) {
    t_EntradaTLB * nuevaEntrada = malloc(sizeof(t_EntradaTLB));
    nuevaEntrada->pagina = pagina;
    nuevaEntrada->marco = marco;
    nuevaEntrada->tiempoAcceso = tiempoAccesoGlobal;
    tiempoAccesoGlobal++;
    return nuevaEntrada;
}


void agregarTLB(uint32_t pagina, uint32_t marco) {
    if(list_size(listaTLB)<ENTRADAS_TLB){
        t_EntradaTLB * entrada = newEntradaTLB(pagina, marco);
        list_add(listaTLB, entrada);
    }
    else {
        if(string_equals_ignore_case(REEMPLAZO_TLB, "FIFO")){
            reemplazarTLB_FIFO(pagina, marco);
        }
        else if(string_equals_ignore_case(REEMPLAZO_TLB, "LRU")){
            reemplazarTLB_LRU(pagina, marco);
        }
        else {
            log_error(logger, "REMPLAZO_TLB: %s no contemplado", REEMPLAZO_TLB);
            exit(-1);
        }
    }
    //imprimirEntradasTLB();
}

void imprimirEntradasTLB(){
    list_iterate(listaTLB, (void*) imprimirEntradaTLB);
}
void imprimirEntradaTLB(t_EntradaTLB* entrada){
    log_info(logger, "ENTRADA_TLB: página: %d - marco: %d - tiempoAcceso: %d", entrada->pagina, entrada->marco, entrada->tiempoAcceso);
}

// void imprimirBitsUso(t_entradaSegundoNivel* entrada) {
//         if(entrada->presencia)
//             log_info(logger, "PAGINASWAP: %d - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d", entrada->paginaSwap, entrada->marco, entrada->uso, entrada->presencia);
//     }

//     list_iterate(entradasSegundoNivel, (void*) imprimirBitsUso);

void vaciarTLB(uint32_t pcb_nuevo){
    void destroyer(void * entrada){
        free(entrada);
    }
    if(PCB_ACTUAL != pcb_nuevo){
        list_clean_and_destroy_elements(listaTLB, destroyer);
        log_info(logger, "TLB vaciada por cambio de proceso");
    }
    else{
        log_info(logger, "No se vacía la TLB porque sigue ejecutando el mismo proceso");
    }
    PCB_ACTUAL = pcb_nuevo;
}

void actualizarTLB(t_EntradaTLB * entrada) {
    entrada->tiempoAcceso = tiempoAccesoGlobal;
    tiempoAccesoGlobal++;
}

void reemplazarTLB_FIFO(uint32_t pagina, uint32_t marco) {
    t_EntradaTLB * nuevaEntrada = newEntradaTLB(pagina, marco);
    t_EntradaTLB * viejaEntrada = list_replace(listaTLB, indiceFIFO, nuevaEntrada);
    //log_info(logger, "pagina viejaEntrada:%d - marco viejaEntrada:%d - pagina nuevaEntrada: %d - marco nuevaEntrada:%d", viejaEntrada->pagina, viejaEntrada->marco, nuevaEntrada->pagina, nuevaEntrada->marco);
    free(viejaEntrada);
    incrementarIndiceFIFO();
}

void reemplazarTLB_LRU(uint32_t pagina, uint32_t marco) {
    //last recently used
    t_EntradaTLB * nuevaEntrada = newEntradaTLB(pagina, marco);
    bool comparator(void * tlb1, void * tlb2) {
        return ((t_EntradaTLB *) tlb1)->tiempoAcceso < ((t_EntradaTLB *) tlb2)->tiempoAcceso;
    }
    list_sort(listaTLB, comparator);
    t_EntradaTLB * viejaEntrada = list_remove(listaTLB,0);
    list_add(listaTLB, nuevaEntrada);
    //log_info(logger, "pagina viejaEntrada:%d - marco viejaEntrada:%d - pagina nuevaEntrada: %d - marco nuevaEntrada:%d", viejaEntrada->pagina, viejaEntrada->marco, nuevaEntrada->pagina, nuevaEntrada->marco);
    free(viejaEntrada);
}


t_EntradaTLB * obtenerEntradaTLB(uint32_t pagina) {
    bool condicion(void * tlb) {
        return ((t_EntradaTLB *) tlb)->pagina == pagina;
    }
    t_EntradaTLB * entrada = list_find(listaTLB, condicion);
    return entrada;
}


void incrementarIndiceFIFO() {
    indiceFIFO = (indiceFIFO + 1) % ENTRADAS_TLB;
}

uint32_t consultarTablaSegundoNivel(uint32_t tablaDePaginasPrimerNivel, uint32_t pagina) {

    int socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA);
    uint32_t entradaPrimerNivel = obtenerEntradaTabla1erNivel(pagina);
  
    t_page_table_request* request = create_page_table_request();
    request->table_number = tablaDePaginasPrimerNivel;
    request->entry_number = entradaPrimerNivel;
    request->pid = PCB_ACTUAL;

    t_buffer* buffer = new_page_table_request_buffer(request);

	t_package* package = new_package(buffer, GET_SECOND_LEVEL_TABLE_REQUEST);

	if (send_to_server(socket_memoria, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}

   uint32_t cod_op = recibir_operacion(socket_memoria);
    if(cod_op != GET_SECOND_LEVEL_TABLE_RESPONSE){
            perror("respuesta inesperada");
            exit(EXIT_FAILURE);
        }
    
    char* buffer_recibido = recibir_paquete(socket_memoria);

    uint32_t tabla_segundo_nivel_numero;

    deserialize_int(&tabla_segundo_nivel_numero, buffer_recibido);
    
    free(buffer_recibido);

    return tabla_segundo_nivel_numero;
}


uint32_t consultarMarco(uint32_t tablaDePaginasSegundoNivel, uint32_t pagina, t_op_code codOP) {
    uint32_t socket_memoria = iniciar_cliente(IP_MEMORIA, PUERTO_MEMORIA);
    uint32_t entradaSegundoNivel = obtenerEntradaTabla2doNivel(pagina);

    // t_consultaTablaPagina * consulta = malloc(sizeof(t_consultaTablaPagina));

    t_page_table_request* request = create_page_table_request(); 
    request->table_number = tablaDePaginasSegundoNivel;
    request->entry_number = entradaSegundoNivel;
    request->pid = PCB_ACTUAL;

    t_buffer* buffer = new_page_table_request_buffer(request);

	t_package* package = new_package(buffer, codOP);

    // t_paquete * paquete = armarPaqueteCon(consulta, codOP);
    // enviarPaquete(paquete,socket_memoria);
    // free(consulta);
    // eliminarPaquete(paquete);
    // t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);

    	if (send_to_server(socket_memoria, package) == -1) {
		log_error(logger, "Error al enviar paquete al servidor");
		return 1;
	}
  
    uint32_t cod_op = recibir_operacion(socket_memoria);
    if(cod_op != GET_FRAME_WRITE_RESPONSE && cod_op != GET_FRAME_READ_RESPONSE){
            perror("Respuesta inesperada");
            exit(EXIT_FAILURE);
        }
    
    char* buffer_recibido = recibir_paquete(socket_memoria);

    uint32_t marco;

    deserialize_int(&marco, buffer_recibido);
    
    free(buffer_recibido);

    return marco;
}

uint32_t consultarDireccionFisica(uint32_t tablaPaginasPrimerNivelPCB, uint32_t direccion_logica, t_op_code codOP) {
    uint32_t pagina = obtenerNumeroPagina(direccion_logica);
    t_EntradaTLB * entrada = obtenerEntradaTLB(pagina);
    uint32_t marco;
    
    if(entrada==NULL) { //Si no esta en TLB
        uint32_t tablaDePaginasSegundoNivel = consultarTablaSegundoNivel(tablaPaginasPrimerNivelPCB, pagina);
        marco = consultarMarco(tablaDePaginasSegundoNivel, pagina, codOP);
        log_info(logger, "TLB MISS: página:%d - marco:%d NO encontrada en TLB, buscando en memoria", pagina, marco);
        agregarTLB(pagina, marco);
    }
    else {
        marco = entrada->marco;
        log_info(logger, "TLB HIT: página:%d - marco:%d encontrada en TLB", pagina, marco);
        actualizarTLB(entrada);
    }

    uint32_t desplazamiento = obtenerDesplazamiento(direccion_logica, pagina);
    uint32_t direccionFisica = obtenerDireccionFisica(desplazamiento, marco);
    return direccionFisica;
}

uint32_t obtenerNumeroPagina(uint32_t direccion_logica) {
    return floor(direccion_logica/traduccion_direcciones->page_size);
}

uint32_t obtenerEntradaTabla1erNivel(uint32_t numero_pagina) {
    return floor(numero_pagina/traduccion_direcciones->pages_per_table);
}

uint32_t obtenerEntradaTabla2doNivel(uint32_t numero_pagina) {
    return numero_pagina % traduccion_direcciones->pages_per_table;
}

uint32_t obtenerDesplazamiento(uint32_t direccion_logica, uint32_t numero_pagina) {
    return direccion_logica - numero_pagina * traduccion_direcciones->page_size;
}

uint32_t obtenerDireccionFisica(uint32_t desplazamiento, uint32_t numero_marco) {
    return desplazamiento + numero_marco * traduccion_direcciones->page_size;
}






// Operaciones y traduccion de direcciones logicas a fisicas
// ESTRUCTURA DE TLB