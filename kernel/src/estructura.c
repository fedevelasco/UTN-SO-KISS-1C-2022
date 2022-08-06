#include <estructura.h>

char* codOPtoString(t_op_code cod_op){

	switch (cod_op){
		case PROCESO:
			return "PROCESO";
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:
			return "REQ_PCB_A_EJECUTAR_KERNEL_CPU";
		case PCB_EJECUTADO_IO_CPU_KERNEL:
			return "PCB_EJECUTADO_IO_CPU_KERNEL";
		case PCB_EJECUTADO_EXIT_CPU_KERNEL:
			return "PCB_EJECUTADO_EXIT_CPU_KERNEL";
		case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:
			return "PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL";
		case REQ_INTERRUPCION_KERNEL_CPU: //HILO INTERRUP:
			return "REQ_INTERRUPCION_KERNEL_CPU";
		case RES_INTERRUPCION_CPU_KERNEL: //HILO DISPATC:
			return "RES_INTERRUPCION_CPU_KERNEL";
		case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:
			return "REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA";
		case RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU:
			return "RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU";
		case RES_FIN_PROCESO_KERNEL_CONSOLA:
			return "RES_FIN_PROCESO_KERNEL_CONSOLA";
		case ALGO:
			return "ALGO";
		default:{
			char * error = string_new();
            string_append_with_format(&error,"Código de operacion no contemplado");
            perror(error);
            exit(1);
		}
			break;
	}
}
void crearBuffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer_new));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crearPaquete(t_op_code cod_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	crearBuffer(paquete);
	return paquete;
}


t_paquete* armarPaqueteCon(void* estructura, t_op_code cod_op){ 

	t_paquete* paquete = crearPaquete(cod_op);
	paquete->buffer->size = tamanioEstructura(estructura, cod_op);
	paquete->buffer->stream = serializarEstructura(estructura, paquete->buffer->size, cod_op);

	//printf("Paquete %d creado \n", paquete->codigoOperacion);
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


void*  serializarMensaje(void* stream, void* estructura){
	t_mensaje* mensaje = (t_mensaje*) estructura;
    
	int offset = 0;
	memcpy(stream + offset, &(mensaje->longitud),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, mensaje->texto,mensaje->longitud);
	return stream;
}
t_mensaje*  deserializarMensaje(void* stream){
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
    int offset = 0;
	memcpy(&(mensaje->longitud), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	mensaje->texto = malloc(sizeof(char)*(mensaje->longitud));
    memcpy(mensaje->texto, stream + offset, mensaje->longitud);
	return mensaje;
}


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
int bytes_PCB_struc(t_pcb* pcb){
    return sizeof(uint32_t)*7 + bytes_instructions_list(pcb->instrucciones);
}
// --------- armo un pcb serializado y me arma el stream a mandar de una ------------
// REQ_PCB_A_EJECUTAR_KERNEL_CPU,  ejecutar pcb

// PCB_EJECUTADO_IO_CPU_KERNEL, pcb necesita IO

// PCB_EJECUTADO_EXIT_CPU_KERNEL, pcb ejecutado, puede finalizarce

// PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL, el pcb que me devolvieron fue interrumpido
char* pcb_create_package_with_opcode(t_pcb* pcb, int32_t bytes, t_op_code opcode){
	char * output  = malloc(bytes);
	memset(output, 0, bytes);

	int32_t offset = 0;

	switch(opcode){
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
			offset += serialize_opcode(output + offset, (t_op_code*) REQ_PCB_A_EJECUTAR_KERNEL_CPU);
			pcb_serializar_estruc(output, pcb, offset);
			return output;
		}
		case PCB_EJECUTADO_IO_CPU_KERNEL:{
			offset += serialize_opcode(output + offset, (t_op_code*) PCB_EJECUTADO_IO_CPU_KERNEL);
			pcb_serializar_estruc(output, pcb, offset);
			return output;
		}
		case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
			offset += serialize_opcode(output + offset, (t_op_code*) PCB_EJECUTADO_EXIT_CPU_KERNEL);
			pcb_serializar_estruc(output, pcb, offset);
			return output;
		}
		case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
			offset += serialize_opcode(output + offset, (t_op_code*) PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
			pcb_serializar_estruc(output, pcb, offset);
			return output;
		}
		default:{
			fprintf(stderr,"Kernel - Código de operacion %d no contemplado", opcode);
			exit(EXIT_FAILURE);
		}
	}
	
	//offset += pcb_serializar(pcb, output + offset);
}
// ----------- Creo pcb para la deserializacion -----------
 t_pcb* pcb_create_with_size_for_des(int32_t size){ // Uso el pcb create con size para cuando deserealizo un PCB
 	t_pcb* pcb = malloc(size);
 	pcb->id= 0;
     pcb->tamanioProceso = 0;
 	pcb->PC = 0;
     pcb->tablaDePaginas = 0;
     pcb->estimacionRafaga = 0;
 	pcb->lengthUltimaRafaga = 0;
 	pcb->sizeInstrucciones = 0;
     pcb->instrucciones = create_instructions_list();
 	return pcb;
 }
// ----------- recibo stream de datos post opcode y me genera un pcb ----------------
t_pcb* pcb_recibir_package(int32_t socket){
	int32_t bytes;
	recv(socket, &bytes, sizeof(int32_t), 0);
	char* a_deserializar = malloc(bytes);
	recv(socket, a_deserializar, bytes, 0);
	t_pcb* temp = pcb_create_with_size_for_des(bytes);
	temp = pcb_deserializar_estrucs(a_deserializar, 0);
	return temp;
}

void freePCB(t_pcb * pcb) {
	free(pcb->instrucciones);
	free(pcb);
}
void*  serializarIO(void* stream, void* estructura){
	t_IO* IO = (t_IO*) estructura;
	int offset = 0;
	memcpy(stream + offset, &(IO->tiempoBloqueo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(IO->tiempoEspera),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	pcb_serializar_estruc(stream, (void*) IO->pcb, offset);
	return stream;
}

t_IO* deserializarIO(void* stream){
	t_IO* IO = malloc(sizeof(t_IO));
	int offset = 0;
	memcpy(&(IO->tiempoBloqueo), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(IO->tiempoEspera), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	IO->pcb = pcb_deserializar_estrucs(stream, offset);
	return IO;
}

void* serializarTraduccionDirecciones(void* stream, void* estructura){
	t_traduccion_direcciones* traduccion_direcciones = (t_traduccion_direcciones*) estructura;
    int offset = 0;
    memcpy(stream + offset, &(traduccion_direcciones->tamanio_pagina),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, &(traduccion_direcciones->paginas_por_tabla),sizeof(uint32_t));
	return stream;
}

t_traduccion_direcciones* deserializarTraduccionDirecciones(void* stream){
	t_traduccion_direcciones* traduccion_direcciones = malloc(sizeof(t_traduccion_direcciones));
    int offset = 0;
    memcpy(&(traduccion_direcciones->tamanio_pagina), stream + offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(traduccion_direcciones->paginas_por_tabla), stream + offset,sizeof(uint32_t));
	return traduccion_direcciones;
}
void* serializarUINT32_T(void* stream, void* estructura) {
	uint32_t* number = (uint32_t*) estructura;
    memcpy(stream, number,sizeof(uint32_t));
	return stream;
}

uint32_t* deserializarUINT32_T(void* stream) {
	uint32_t* number = malloc(sizeof(uint32_t));
    memcpy(number, stream, sizeof(uint32_t));
	return number;
}

void* serializarConsultaTablaPagina(void* stream, void* estructura) {
	int offset = 0;
	t_consultaTablaPagina* consulta = (t_consultaTablaPagina*) estructura;
    memcpy(stream + offset, &(consulta->entradaPagina),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, &(consulta->tablaDePaginas),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, &(consulta->id),sizeof(uint32_t));
	return stream;
}

t_consultaTablaPagina* deserializarConsultaTablaPagina(void* stream) {
	int offset = 0;
	t_consultaTablaPagina* consulta = malloc(sizeof(t_consultaTablaPagina));
    memcpy(&(consulta->entradaPagina), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(&(consulta->tablaDePaginas), stream + offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(&(consulta->id), stream + offset,sizeof(uint32_t));
	return consulta;
}

void* serializartPeticionEscritura(void* stream, void* estructura) {
	int offset = 0;
	t_peticionEscritura* peticion = (t_peticionEscritura*) estructura;
    memcpy(stream + offset, &(peticion->direccionFisica),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, &(peticion->dato),sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(stream + offset, &(peticion->id),sizeof(uint32_t));
	return stream;
}

t_peticionEscritura* deserializarPeticionEscritura(void* stream) {
	int offset = 0;
	t_peticionEscritura* peticion = malloc(sizeof(t_peticionEscritura));
    memcpy(&(peticion->direccionFisica), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(&(peticion->dato), stream + offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
    memcpy(&(peticion->id), stream + offset,sizeof(uint32_t));
	return peticion;
}

void* serializarEstructura(void* estructura, int tamanio, t_op_code cod_op){

	void* stream = malloc(tamanio);

	switch(cod_op){
		case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:{
			return serializarMensaje(stream,estructura);
		}
		case RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU:{
			return serializarTraduccionDirecciones(stream,estructura);
		}
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
			return pcb_serializar_estruc(stream,estructura,0); //mi serializacion
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
		case REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA:{
			return serializarConsultaTablaPagina(stream,estructura);
		}
		case RES_TABLA_SEGUNDO_NIVEL_MEMORIA_CPU: {
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_MARCO_ESCRITURA_CPU_MEMORIA:{
			return serializarConsultaTablaPagina(stream,estructura);
		}
		case REQ_MARCO_LECTURA_CPU_MEMORIA:{
			return serializarConsultaTablaPagina(stream,estructura);
		}
		case RES_MARCO_MEMORIA_CPU:{
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_READ_CPU_MEMORIA:{
			return serializarUINT32_T(stream,estructura);
		}
		case RES_READ_MEMORIA_CPU:{
			return serializarUINT32_T(stream,estructura);
		}
		case REQ_WRITE_CPU_MEMORIA :{
			return serializartPeticionEscritura(stream, estructura);
		}
		case RES_WRITE_CPU_MEMORIA :{
			return serializarUINT32_T(stream,estructura);
		}

		default:{
			fprintf(stderr,"Kernel - Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
		}
	}
}

int tamanioEstructura(void* estructura ,t_op_code cod_op){

	switch(cod_op){

		case PROCESO:{
			t_proceso * proceso = (t_proceso *) estructura;
			return sizeof(uint32_t)*2 + bytes_instructions_list(proceso->instrucciones);
		}
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
			fprintf(stderr,"Kernel - Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
		}	
	}
}

