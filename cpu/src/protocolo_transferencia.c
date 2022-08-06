#include <protocolo_transferencia.h>


// --------- serializa el stream y devuelve el offset ------------
void* pcb_serializar_estruc(void* stream, void* estructura, int offset){
	
	t_pcb* pcb = (t_pcb*) estructura;

    offset += serialize_int(stream + offset, &(pcb->id));
    offset += serialize_int(stream + offset, &(pcb->tamanioProceso));
    offset += serialize_int(stream + offset, &(pcb->PC));
 	offset += serialize_int(stream + offset, &(pcb->tablaDePaginas));
    offset += serialize_int(stream + offset, &(pcb->estimacionRafaga));
	offset += serialize_int(stream + offset, &(pcb->lengthUltimaRafaga));
	offset += serialize_int(stream + offset, &(pcb->sizeInstrucciones));
	offset += serialize_instructions_list (stream + offset, pcb->instrucciones);

    return stream;
}
// ------------- Devuelve un pcb deserializado a partir de un stream de datos ----------------
t_pcb* pcb_deserializar_estrucs(void* stream, int offset){
	
	t_pcb* pcb = malloc(sizeof(t_pcb));
	offset += deserialize_int(&(pcb->id), stream + offset);
	offset += deserialize_int(&(pcb->tamanioProceso), stream + offset);
	offset += deserialize_int(&(pcb->PC), stream + offset);
    offset += deserialize_int(&(pcb->tablaDePaginas), stream + offset);
	offset += deserialize_int(&(pcb->estimacionRafaga), stream + offset);
	offset += deserialize_int(&(pcb->lengthUltimaRafaga), stream + offset);
	offset += deserialize_int(&(pcb->sizeInstrucciones), stream + offset);

	pcb->instrucciones = create_instructions_list_with_size(pcb->sizeInstrucciones);

	offset += deserialize_instructions_list(pcb->instrucciones, stream + offset);    

	return pcb;
}
// --------- Caculo los bytes para el pcb ------------
uint32_t bytes_PCB(t_pcb* pcb){
    return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
}
// --------- armo un pcb serializado y me arma el stream a mandar de una ------------
t_paquete * pcb_create_package_with_opcode(t_pcb* pcb, t_op_code opcode){
    t_paquete * paquete = crearPaquete(opcode);
    paquete->buffer->size = bytes_PCB(pcb);
    pcb_serializar_estruc(paquete->buffer->stream, pcb, 0); 
    
	// char * output  = malloc(bytes);
	// memset(output, 0, bytes);

	// int32_t offset = 0;

	// offset += serialize_opcode(output + offset, opcode);
	// //offset += pcb_serializar(pcb, output + offset);
	// pcb_serializar_estruc(output, pcb, offset);

	return paquete;
}
// ----------- Creo pcb para la deserializacion -----------
t_pcb* pcb_create_with_size(int32_t size){ // Uso el pcb create con size para cuando deserealizo un PCB
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->id= 0;
    pcb->tamanioProceso = 0;
	pcb->PC = 0;
    pcb->tablaDePaginas = 0;
    pcb->estimacionRafaga = 0;
	pcb->lengthUltimaRafaga = 0;
	pcb->lengthUltimaRafaga = 0;
    pcb->instrucciones = create_instructions_list_with_size(size);
	return pcb;
}
// ----------- recibo stream de datos post opcode y me genera un pcb ----------------
t_pcb* pcb_recibir_package(int32_t socket){
	int32_t bytes;
	recv(socket, &bytes, sizeof(int32_t), 0);
	char* a_deserializar = malloc(bytes);
	recv(socket, a_deserializar, bytes, 0);
	t_pcb* temp = pcb_create_with_size(bytes);
	temp = pcb_deserializar_estrucs(a_deserializar, 0);
	return temp;
}

void freePCB(t_pcb * pcb) {
	instructions_list_destroy(pcb->instrucciones);
	free(pcb);
}

void *  serializarIO(void* stream, void* estructura){
	t_IO* IO = (t_IO*) estructura;
	int offset = 0;
	memcpy(stream + offset, &(IO->tiempoBloqueo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(IO->tiempoEspera),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	pcb_serializar_estruc(stream, (void*) IO->pcb, offset);
	return stream;
}

t_IO * deserializarIO(void* stream){
	t_IO* IO = malloc(sizeof(t_IO));
	int offset = 0;
	memcpy(&(IO->tiempoBloqueo), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(IO->tiempoEspera), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	IO->pcb = pcb_deserializar_estrucs(stream, offset);
	return IO;
}


void * serializarUINT32_T(void* stream, void* estructura) {
	uint32_t* number = (uint32_t*) estructura;
    memcpy(stream, number,sizeof(uint32_t));
	return stream;
}

uint32_t * deserializarUINT32_T(void* stream) {
	uint32_t* number = malloc(sizeof(uint32_t));
    memcpy(number, stream, sizeof(uint32_t));
	return number;
}

void *  serializarMensaje(void* stream, void* estructura){
	t_mensaje* mensaje = (t_mensaje*) estructura;

	int offset = 0;
	memcpy(stream + offset, &(mensaje->longitud),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, mensaje->texto,mensaje->longitud);
	return stream;
}
t_mensaje *  deserializarMensaje(void* stream){
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
    int offset = 0;
	memcpy(&(mensaje->longitud), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	mensaje->texto = malloc(sizeof(char)*(mensaje->longitud));
    memcpy(mensaje->texto, stream + offset, mensaje->longitud);
	return mensaje;
}




void* serializarEstructura(void* estructura, int tamanio, t_op_code cod_op){

	void* stream = malloc(tamanio);

	switch(cod_op){
		case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:{
			return serializarMensaje(stream,estructura);
		}
//		case RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU:{
//			return serializarTraduccionDirecciones(stream,estructura);
//		}
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case PCB_EJECUTADO_IO_CPU_KERNEL:{
			return serializarIO(stream,estructura);
		}
		case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case REQ_INTERRUPCION_KERNEL_CPU:{
			return serializarUINT32_T(stream,estructura);
		}
		case EXIT_CONSOLE:{
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_FIN_PROCESO_KERNEL_MEMORIA:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case REQ_CREAR_PROCESO_KERNEL_MEMORIA:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case RES_CREAR_PROCESO_KERNEL_MEMORIA:{
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_SUSP_PROCESO_KERNEL_MEMORIA:{
			return pcb_serializar_estruc(stream,estructura,0);
		}
		case RES_SUSP_PROCESO_KERNEL_MEMORIA:{
			return serializarUINT32_T(stream, estructura);
		}
//		case REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA:{
//			return serializarConsultaTablaPagina(stream,estructura);
//		}
		case RES_TABLA_SEGUNDO_NIVEL_MEMORIA_CPU: {
			return serializarUINT32_T(stream,estructura);
		}
//		case REQ_MARCO_ESCRITURA_CPU_MEMORIA:{
//			return serializarConsultaTablaPagina(stream,estructura);
//		}
//		case REQ_MARCO_LECTURA_CPU_MEMORIA:{
//			return serializarConsultaTablaPagina(stream,estructura);
//		}
		case RES_MARCO_MEMORIA_CPU:{
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_READ_CPU_MEMORIA:{
			return serializarUINT32_T(stream,estructura);
		}
		case RES_READ_MEMORIA_CPU:{
			return serializarUINT32_T(stream,estructura);
		}
//		case REQ_WRITE_CPU_MEMORIA :{
//			return serializartPeticionEscritura(stream, estructura);
//		}
		case RES_WRITE_CPU_MEMORIA :{
			return serializarUINT32_T(stream,estructura);
		}

		default:{
			fprintf(stderr,"serializarEstructura - Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
		}
	}
}



void crearBuffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crearPaquete(t_op_code cod_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;

	return paquete;
}

t_paquete* armarPaqueteCon(void* estructura,t_op_code cod_op){

	t_paquete* paquete = crearPaquete(cod_op);
	paquete->buffer->size = tamanioEstructura(estructura,cod_op);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	paquete->buffer->stream = serializarEstructura(estructura,paquete->buffer->size,cod_op);

	printf("Paquete %d creado \n", paquete->codigo_operacion);
	return  paquete;

}

void* serializarPaquete(t_paquete* paquete, uint32_t bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(uint32_t));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(uint32_t), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(uint32_t);
}

void enviarPaquete(t_paquete* paquete, int socket_cliente)
{
	uint32_t bytes = paquete->buffer->size + 2*sizeof(uint32_t);
	void* a_enviar = serializarPaquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);
	free(a_enviar);
}

void eliminarPaquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

uint32_t tamanioEstructura(void* estructura ,t_op_code cod_op){

	switch(cod_op){

//		case PROCESO:{
//			t_proceso * proceso = (t_proceso *) estructura;
//			return sizeof(uint32_t)*2 + proceso->sizeInstrucciones*(sizeof(uint32_t)*2 + sizeof(instruccion_id));
//		}
		case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:{
			t_mensaje * msg = (t_mensaje*) estructura;
			return msg->longitud + sizeof(uint32_t);
		}
		case RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU:{
			return sizeof(uint32_t)*2;
		}
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
			t_pcb* pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case PCB_EJECUTADO_IO_CPU_KERNEL:{
			t_IO * io = (t_IO *) estructura;
			return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)*7 + bytes_instructions_list(io->pcb->instrucciones);
		}
		case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
			t_pcb * pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
			t_pcb * pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case REQ_INTERRUPCION_KERNEL_CPU:{
			return sizeof(uint32_t);
		}
		case EXIT_CONSOLE:{
			return sizeof(uint32_t);
		}
		case REQ_FIN_PROCESO_KERNEL_MEMORIA:{
			t_pcb * pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case REQ_CREAR_PROCESO_KERNEL_MEMORIA:{
			t_pcb * pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case RES_CREAR_PROCESO_KERNEL_MEMORIA:{
			return sizeof(uint32_t);
		}
		case REQ_SUSP_PROCESO_KERNEL_MEMORIA:{
			t_pcb * pcb = (t_pcb *) estructura;
			return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
		}
		case RES_SUSP_PROCESO_KERNEL_MEMORIA:{
			return sizeof(uint32_t);
		}
		case REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA:{
			return sizeof(uint32_t)*3;
		}
		case RES_TABLA_SEGUNDO_NIVEL_MEMORIA_CPU: {
			return sizeof(uint32_t);
		}
		case REQ_MARCO_ESCRITURA_CPU_MEMORIA:{
			return sizeof(uint32_t)*3;
		}
		case REQ_MARCO_LECTURA_CPU_MEMORIA:{
			return sizeof(uint32_t)*3;
		}
		case RES_MARCO_MEMORIA_CPU:{
			return sizeof(uint32_t);
		}
		case REQ_READ_CPU_MEMORIA:{
			return sizeof(uint32_t);
		}
		case RES_READ_MEMORIA_CPU:{
			return sizeof(uint32_t);
		}
		case REQ_WRITE_CPU_MEMORIA:{
			return sizeof(uint32_t)*3;
		}
		case RES_WRITE_CPU_MEMORIA:{
			return sizeof(uint32_t);
		}
		default: {
			fprintf(stderr,"tamanioEstructura - Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
		}
	}
}

// 1: int32_t cod_op = recibir_operacion(cliente_fd)
// 2: t_pcb pcb = pcb_recibir_package(int32_t socket)
