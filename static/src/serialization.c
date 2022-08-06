#include <serialization.h>

uint32_t serialize_int(char* output, uint32_t* input) {
	uint32_t size = sizeof(uint32_t);
	memcpy(output, input, size);
	return size;
}
uint32_t deserialize_int(uint32_t* output, char* input) {
	uint32_t size = sizeof(uint32_t);
	memcpy(output, input, size);
	return size;
}

uint32_t serialize_opcode(char* output, t_op_code* input) {
	uint32_t size = sizeof(t_op_code);
	memcpy(output, input, sizeof(t_op_code));
	return size;
}
uint32_t deserialize_opcode(t_op_code* output, char* input) {
	uint32_t size = sizeof(t_op_code);
	memcpy(output, input, size);
	return size;
}


uint32_t serialize_list(char* output,t_list* input, uint32_t element_size) {
	uint32_t i, offset = 1;
	// Se va a guardar en la primera posicion la cantidad de elementos
	output[0] = list_size(input);
	for (i = 0; i < output[0]; i++) {
		memcpy(output + offset, list_get(input, i), element_size);
		offset += element_size;
	}
	return offset;
}

uint32_t deserialize_list(t_list* output,char* input, uint32_t element_size) {
	uint32_t i, offset = 1;
	// Se saca de la primera posicion la cantidad de elementos
	for (i = 0; i < input[0]; i++) {
		void* buffer = malloc(element_size);
		memcpy(buffer, input + offset, element_size);
		list_add(output, buffer);
		offset += element_size;
		free(buffer);
	}

	return offset;
}

uint32_t deserialize_parameters(t_list* output,char* input, uint32_t element_size) {
	uint32_t i, offset = 1;
	// Se saca de la primera posicion la cantidad de elementos
	for (i = 0; i < input[0]; i++) {
		t_parameter* parameter = create_parameter();
		memcpy(&parameter->value, input + offset, element_size);
		list_add(output, parameter);
		offset += element_size;
	}
	// for(int i=0;i<list_size(output);i++){
	// 	// printf("\ndeserialize_parameters - parameter value: %d\n", ((t_parameter*) list_get(output,i))->value);
	// }

	return offset;
}

uint32_t serialize_string(char* output,char* input) {
	uint32_t offset = 0;
	// Se va a guardar en la primera posicion la cantidad de chars
	output[0] = string_length(input)+1;
	offset += sizeof(char);
	memcpy(output + offset, input, output[0]);
	offset += output[0];

	return offset;
}

uint32_t deserialize_string(char* output,char* input) {
	uint32_t offset = 1;
	// Se saca de la primera posicion la cantidad de chars
	memcpy(output, input + offset, input[0]);
	offset += input[0];

	return offset;
}

uint32_t serialize_instruction(char* output, t_instruction* input) {
	uint32_t offset = 0;
	offset += serialize_string(output + offset, input->id);
	offset += serialize_list(output + offset, input->parameters, sizeof(t_parameter));

	return offset;
}

uint32_t serialize_instructions_list(char* output, t_instructions_list* input) {
	uint32_t i, offset = 0, cant;
	// Cantidad de instrucciones

	cant = list_size(input->instructions);
	offset += serialize_int(output + offset, &(cant));
	for (i = 0; i < cant; i++) {
		// Serializacion de cada instruccion
		offset += serialize_instruction(output + offset, list_get(input->instructions, i));
	}
	offset += serialize_int(output + offset, &(input->process_size));
	return offset;
}

uint32_t deserialize_instruction(t_instruction* output, char* input) {
	uint32_t offset = 0;
	offset += deserialize_string(output->id, input);
	offset += deserialize_parameters(output->parameters, input + offset, sizeof(t_parameter));
	// for(int i=0;i<list_size(output->parameters);i++){
	// 	printf("\ndeserialize_instruction - Instruction: parameter value: %d\n", ((t_parameter*) list_get(output->parameters,i))->value);
	// 	}

	return offset;
}

uint32_t deserialize_instructions_list(t_instructions_list* output, char* input) {
	uint32_t i, offset = 0, cant;

	offset += deserialize_int(&cant, input);

	// Cantidad de instrucciones en input[0]
	for (i = 0; i < cant; i++) {
		t_instruction* instruction = create_instruction();
		offset += deserialize_instruction(instruction, input + offset);
		// Agrego la instruccion recibida a la lista
		list_add(output->instructions, instruction);

	}
	offset += deserialize_int(&output->process_size, input + offset);
	return offset;
}

//No necesito deserializar el package y buffer. Tengo funciones que reciben el op_code y el extraen el buffer.
uint32_t serialize_buffer_stream(char* output, t_buffer* input) {
	uint32_t offset = 0;
	memcpy(output + offset, input->stream, input->size);
	offset+= input->size;

	return offset;
}

uint32_t serialize_buffer(char* output, t_buffer* input) {
	uint32_t offset = 0;
	offset += serialize_int(output + offset, &(input->size));
	offset += serialize_buffer_stream(output + offset, input);

	return offset;
}

//Hago return del output directamente por que es lo ultimo en serializar antes de enviar.
char* serialize_package(t_package* package, uint32_t bytes)
{
	char * output  = malloc(bytes);
	memset(output, 0, bytes);

	uint32_t offset = 0;

	offset += serialize_opcode(output + offset, &(package->operation_code));
	offset += serialize_buffer(output + offset, package->buffer);
	
	return output;
}

t_buffer* new_instruction_buffer(t_instructions_list* instructions_list, t_log* logger){

	for(int i=0; i<list_size(instructions_list->instructions); i++){
			log_debug(logger, "create_instruction_buffer - Instruction %s\n", ((t_instruction*) list_get(instructions_list->instructions,i))->id );
		}

	t_buffer* buffer = create_buffer();

	buffer->size = bytes_instructions_list(instructions_list);

	buffer->stream = malloc(buffer->size);
	int offset = serialize_instructions_list(buffer->stream, instructions_list);

	instructions_list_destroy(instructions_list);
//	log_debug(logger, "create_instruction_buffer - size: %d\n", offset);

	return buffer;
}

uint32_t serialize_process(char* output, t_process* input) {
	uint32_t offset = 0;
	offset += serialize_int(output + offset, &(input->pid));
	offset += serialize_int(output + offset, &(input->process_size));
	offset += serialize_int(output + offset, &(input->first_level_table_number));

	return offset;
}

uint32_t deserialize_process(t_process* output, char* input) {
	uint32_t offset = 0;
	offset += deserialize_int(&output->pid, input);
	offset += deserialize_int(&output->process_size, input + offset);
	offset += deserialize_int(&output->first_level_table_number, input + offset);

	return offset;
}

uint32_t serialize_memory_config(char* output, t_memory_config* input) {
	uint32_t offset = 0;
	offset += serialize_int(output + offset, &(input->page_size));
	offset += serialize_int(output + offset, &(input->pages_per_table));

	return offset;
}

uint32_t deserialize_memory_config(t_memory_config* output, char* input) {
	uint32_t offset = 0;
	offset += deserialize_int(&output->page_size, input);
	offset += deserialize_int(&output->pages_per_table, input + offset);

	return offset;
}

uint32_t serialize_page_table_request(char* output, t_page_table_request* input) {
	uint32_t offset = 0;
	offset += serialize_int(output + offset, &(input->table_number));
	offset += serialize_int(output + offset, &(input->entry_number));
	offset += serialize_int(output + offset, &(input->pid));

	return offset;
}

uint32_t deserialize_page_table_request(t_page_table_request* output, char* input) {
	uint32_t offset = 0;
	offset += deserialize_int(&output->table_number, input);
	offset += deserialize_int(&output->entry_number, input + offset);
	offset += deserialize_int(&output->pid, input + offset);

	return offset;
}

uint32_t serialize_memory_write_request(char* output, t_memory_write_request* input) {
	uint32_t offset = 0;
	offset += serialize_int(output + offset, &(input->fisical_address));
	offset += serialize_int(output + offset, &(input->data));
	offset += serialize_int(output + offset, &(input->pid));

	return offset;
}

uint32_t deserialize_memory_write_request(t_memory_write_request* output, char* input) {
	uint32_t offset = 0;
	offset += deserialize_int(&output->fisical_address, input);
	offset += deserialize_int(&output->data, input + offset);
	offset += deserialize_int(&output->pid, input + offset);

	return offset;
}





void print_buffer(char* buffer, uint32_t size) {
	uint32_t i;
	printf("\nbuffer=");
	for (i = 0; i < size; i++) {
		printf("[%d]", buffer[i]);
	}
	printf("\n");
}


/* INICIO DE TESTS */
int test_serialization(){
	CU_initialize_registry();
	CU_pSuite suite_serialization = CU_add_suite("Suite de Serializacion", NULL, NULL);
	CU_add_test(suite_serialization, "Test de Serializacion de int", test_serialize_int);
	CU_add_test(suite_serialization, "Test de Serializacion de list", test_serialize_list);
	CU_add_test(suite_serialization, "Test de Serializacion de string", test_serialize_string);
	CU_add_test(suite_serialization, "Test de Serializacion de instruction", test_serialize_instruction);
	CU_add_test(suite_serialization, "Test de Serializacion de instruction_list", test_serialize_instruction_list);
	CU_add_test(suite_serialization, "Test de Serializacion de instruction_list", test_serialize_process);


	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
void test_serialize_int(){
	uint32_t a=57,b;
	char* buffer;
	buffer = malloc(sizeof(uint32_t));
	serialize_int(buffer,&a);
	print_buffer(buffer,sizeof(uint32_t));
	deserialize_int(&b,buffer);
	CU_ASSERT_EQUAL(a,b);
	CU_ASSERT_EQUAL(b,57);
	free(buffer);
}

void test_serialize_list(){
	t_list* listA = list_create();
	t_list* listB = list_create();
	t_instruction* instruction = create_instruction();
	string_append(&instruction->id, "NO_OP");
	t_parameter* parameter = new_parameter(2);
	list_add(instruction->parameters, parameter);
	list_add(listA,instruction);
	char *buffer = malloc(bytes_list(listA,sizeof(t_instruction)));
	memset(buffer, 0, bytes_list(listA,sizeof(t_instruction)));
	serialize_list(buffer,listA,bytes_instruction(instruction));
	print_buffer(buffer,bytes_instruction(instruction));
	deserialize_list(listB,buffer,sizeof(t_instruction));
	CU_ASSERT_EQUAL( ((t_parameter*)list_get(((t_instruction*)list_get(listB,0))->parameters, 0))->value ,2);

	instruction_destroy(instruction);
	list_destroy(listA);
	list_destroy(listB);

	free(buffer);
}

void test_serialize_string(){
	char* stringA = string_new();
	char* stringB = string_new();
	string_append(&stringA, "Hello");

	char* buffer;
	buffer = malloc(string_length(stringA)+1);
	memset(buffer, 0, string_length(stringA)+1);
	serialize_string(buffer,stringA);
	print_buffer(buffer,string_length(stringA)+1);
	deserialize_string(stringB,buffer);
	bool equal = string_equals_ignore_case(stringA, stringB);
	CU_ASSERT_EQUAL(equal,true);

	free(stringA);
	free(stringB);
	free(buffer);
}

void test_serialize_instruction(){
	t_instruction* instructionA = create_instruction();
	t_instruction* instructionB = create_instruction();

	string_append(&instructionA->id, "NO_OP");
	t_parameter* parameter = new_parameter(2);
	t_parameter* parameter2 = new_parameter(33);
	list_add(instructionA->parameters, parameter);
	list_add(instructionA->parameters, parameter2);

	char* buffer = malloc(bytes_instruction(instructionA));
	memset(buffer, 0, bytes_instruction(instructionA));
	serialize_instruction(buffer,instructionA);
	print_buffer(buffer,bytes_instruction(instructionA));
	deserialize_instruction(instructionB,buffer);
	bool equal = string_equals_ignore_case(instructionB->id, instructionA->id);
	CU_ASSERT_EQUAL(equal,true);
	CU_ASSERT_EQUAL( ((t_parameter*)list_get(instructionB->parameters,0))->value, ((t_parameter*)list_get(instructionA->parameters,0))->value );
	CU_ASSERT_EQUAL( ((t_parameter*)list_get(instructionB->parameters,1))->value, ((t_parameter*)list_get(instructionA->parameters,1))->value );


	instruction_destroy(instructionA);
	instruction_destroy(instructionB);
	free(buffer);
}
void test_serialize_instruction_list(){

	char* id =string_new();
	string_append(&id, "READ");
	char* id2 =string_new();
	string_append(&id2, "WRITE");

	t_list* parameters = list_create();
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

	t_instructions_list* instructions_listA = create_instructions_list();
	t_instructions_list* instructions_listB = create_instructions_list();

	list_add(instructions_listA->instructions, instruction);
	list_add(instructions_listA->instructions, instruction2);
	instructions_listA->process_size = 1000;


	char* buffer = malloc(bytes_instructions_list(instructions_listA));
	memset(buffer, 0, bytes_instructions_list(instructions_listA));
	serialize_instructions_list(buffer, instructions_listA);
	print_buffer(buffer,bytes_instructions_list(instructions_listA));
	deserialize_instructions_list(instructions_listB, buffer);

	printf("\nInstructions_listA: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listA->instructions,0))->parameters,0))->value);
	printf("Instructions_listA: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listA->instructions,0))->parameters,1))->value);
	printf("Instructions_listA: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listA->instructions,1))->parameters,0))->value);
	printf("Instructions_listA: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listA->instructions,1))->parameters,1))->value);

	printf("\nInstructions_listB: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,0))->value);
	printf("Instructions_listB: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,1))->value);
	printf("Instructions_listB: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,1))->parameters,0))->value);
	printf("Instructions_listB: parameter value: %d\n", ((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,1))->parameters,1))->value);



	CU_ASSERT_EQUAL(list_size(instructions_listB->instructions),list_size(instructions_listA->instructions));
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,0))->value,2);
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,1))->value,100);
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,1))->parameters,0))->value,22);
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,1))->parameters,1))->value,33);
	/*Liberar las estructuras internas*/
	free(id);
	free(id2);


	instructions_list_destroy(instructions_listA);
	instructions_list_destroy(instructions_listB);
	list_destroy_and_destroy_elements(parameters, (void*) parameter_destroy);
	list_destroy_and_destroy_elements(parameters2, (void*) parameter_destroy);
	free(buffer);
}

void test_serialize_process(){
	t_process* processA = create_process();
	t_process* processB = create_process();

	processA->pid = 2;
	processA->process_size = 1024;
	uint32_t size = (2 * (sizeof(uint32_t)));

	char* buffer = malloc (size);
	memset(buffer, 0, size);
	serialize_process(buffer,processA);
	print_buffer(buffer,size);
	deserialize_process(processB,buffer);

	CU_ASSERT_EQUAL(processA->pid, processB->pid);
	CU_ASSERT_EQUAL(processA->process_size, processB->process_size);

	process_destroy(processA);
	process_destroy(processB);
	free(buffer);
}

//TODO: falta agregar tests de package y buffer

//int main(uint32_t argc, char** argv){
//	test_serialization();
//	return 0;
//}




