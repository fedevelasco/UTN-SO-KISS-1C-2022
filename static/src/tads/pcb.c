#include <tads/pcb.h>

t_PCB* pcb_create(){
	t_PCB* pcb = malloc(sizeof(t_PCB));
	pcb->PID = 0;
    pcb->TAMANIO_PROCESO = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
    pcb->PC = 0;
    pcb->TABLA_DE_PAGINAS = 0;
    pcb->ESTIMACION_RAFAGA = 0;
	return pcb;
}

t_PCB* pcb_create_with_size(uint32_t size){ // Uso el pcb create con size para cuando deserealizo un PCB
	t_PCB* pcb = malloc(size);
	pcb->PID = 0;
    pcb->TAMANIO_PROCESO = 0;
    pcb->LISTA_INSTRUCCIONES = create_instructions_list();
    pcb->PC = 0;
    pcb->TABLA_DE_PAGINAS = 0;
    pcb->ESTIMACION_RAFAGA = 0; //milisegundos
	return pcb;
}

void pcb_destroy(t_PCB* pcb){
    instructions_list_destroy(pcb->LISTA_INSTRUCCIONES);
    free(pcb);
}

void pcb_imprimir(t_PCB* pcb, t_log* logger){
    log_info(logger, "Print de PCB: \n");
    log_info(logger, "PID = %i", pcb->PID);
    log_info(logger, "Size = %i", pcb->TAMANIO_PROCESO);
    log_info(logger, "Instruction List ="); 
    imprimir_lista_instrucciones(pcb->LISTA_INSTRUCCIONES);
    log_info(logger, "PC = %i", pcb->PC);
	log_info(logger, "Tabla de Paginas = %i", pcb->TABLA_DE_PAGINAS);
	log_info(logger, "Estimacion = %i\n", pcb->ESTIMACION_RAFAGA);
}

void pcb_asignar_lista_instrucciones(t_PCB* pcb, t_instructions_list* lista_instrucciones){

    pcb->LISTA_INSTRUCCIONES = lista_instrucciones;

}

/* IGNORAR
uint32_t pcb_convertir_float_a_int(float numero_float){
	//Esta funcion convierte el float inicial del archivo de config a un int32 para poder ser serializado, a la hora
	//de deserializar, lo vuelo a convertir un float

	numero_float = numero_float * 10; //lo multiplico por 10 para eliminar la coma
	uint32_t numero_int = (uint32_t)numero_float;
	return numero_int;
}

float pcb_convertir_int_a_float(uint32_t numero_int){
	
	float numero_float = (float)numero_int / 10; //Lo convierto a float y lo divido por 10 para manterner la coma
	return numero_float;
}
*/

void pcb_asignar_estimacion_rafaga_inicial(t_PCB* pcb, char* ESTIMACION_DATO){

	uint32_t ESTIMACION_INT = atoi(ESTIMACION_DATO);
	pcb->ESTIMACION_RAFAGA = ESTIMACION_INT;

}


void pcb_asignar_lista_tamanio(t_PCB* pcb, t_instructions_list* lista_instrucciones){

    pcb->TAMANIO_PROCESO = lista_instrucciones->process_size;

}

void pcb_asignar_id_unico(t_PCB* pcb, t_log* logger){
    
    static short int id_unico = 0;
    pcb->PID = id_unico;
    id_unico++;

}

uint32_t pcb_serializar(t_PCB* pcb, char* output){

	uint32_t offset = 0;

    offset += serialize_int(output + offset, &(pcb->PID));
    offset += serialize_int(output + offset, &(pcb->TAMANIO_PROCESO));
    offset += serialize_instructions_list (output + offset, pcb->LISTA_INSTRUCCIONES);
    offset += serialize_int(output + offset, &(pcb->PC));
    offset += serialize_int(output + offset, &(pcb->TABLA_DE_PAGINAS));
    offset += serialize_int(output + offset, &(pcb->ESTIMACION_RAFAGA));

    return offset;
}

uint32_t bytes_PCB(t_PCB* pcb){
    return sizeof(pcb->PID) + sizeof(pcb->TAMANIO_PROCESO) + sizeof(pcb->PC) + sizeof(pcb->TABLA_DE_PAGINAS) + sizeof(pcb->ESTIMACION_RAFAGA) + bytes_instructions_list(pcb->LISTA_INSTRUCCIONES);
}

uint32_t pcb_deserializar(t_PCB* destino, char* fuente){ 

	uint32_t offset=0;

	offset += deserialize_int(&(destino->PID), fuente + offset);
	offset += deserialize_int(&(destino->TAMANIO_PROCESO), fuente + offset);
    offset += deserialize_instructions_list(destino->LISTA_INSTRUCCIONES, fuente + offset);
	offset += deserialize_int(&(destino->PC), fuente + offset);
    offset += deserialize_int(&(destino->TABLA_DE_PAGINAS), fuente + offset);
	offset += deserialize_int(&(destino->ESTIMACION_RAFAGA), fuente + offset);    

    return 0;
}


char* pcb_creat_paquete(t_PCB* pcb, uint32_t bytes){

	char * output  = malloc(bytes);
	memset(output, 0, bytes);

	uint32_t offset = 0;
	t_op_code* opcode = PCB;

	offset += serialize_opcode(output + offset, opcode);
	offset += pcb_serializar(pcb, output + offset);
	
	return output;

}





	/*  ------------------------TESTING SERIALIZACION DE PCB -------------------------

	log_info(logger, "PRUEBA PCB - CREO UN PCB", logger);
	t_PCB* testing = malloc(sizeof(t_PCB));
	testing = pcb_create();
	testing->PID = 3;
	testing->PC = 3;
	testing->ESTIMACION_RAFAGA = 3;
	testing->TAMANIO_PROCESO = 3;

	char* id =string_new();
	string_append(&id, "READ");
	char* id2 =string_new();
	string_append(&id2, "WRITE");

	t_list* parameters = list_create();  //Creo la lista de instrucciones
	t_parameter* parameter1 = new_parameter(2);
	t_parameter* parameter2 = new_parameter(100);
	list_add(parameters, parameter1);
	list_add(parameters, parameter2);

	t_list* parameters2 = list_create();
	t_parameter* parameter3 = new_parameter(22);
	t_parameter* parameter4 = new_parameter(33);
	list_add(parameters2, parameter3);
	list_add(parameters2, parameter4);

	t_instruction* instruction = new_instruction(id, parameters);
	t_instruction* instruction2 = new_instruction(id2, parameters2);

	list_add(testing->LISTA_INSTRUCCIONES->instructions, instruction);

	testing->LISTA_INSTRUCCIONES->process_size = 1000;

	pcb_imprimir(testing, logger);
	log_info(logger, "PRUEBA PCB - SERIALIZO PCB", logger);
	int bytes = bytes_PCB(testing); //Tamanio de PCB
	char* stream_de_datos = malloc (bytes);	
	pcb_serializar(testing, logger, stream_de_datos);
	// --------- PARA CPU JOACO ----------------
	t_PCB* testing2; // Creo un PCB
	testing2 = pcb_create_with_size(bytes); // ESTOS BYTES SON LOS QUE VAN DENTRO DEL STREAM DE DATOS QUE MANDO
	pcb_deserializar(testing2, stream_de_datos); // DESERIALIZO Y GUARDO EN TESTING2 (PCB)
	pcb_imprimir(testing2, logger); // IMPRIMO EL PCB QUE RECIBI
	// --------- PARA CPU JOACO ----------------

	*/
