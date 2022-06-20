#include "serialization.h"

int32_t serialize_int(char* output, int32_t* input) {
	int32_t size = sizeof(int32_t);
	memcpy(output, input, sizeof(int32_t));
	return size;
}
int32_t deserialize_int(int32_t* output, char* input) {
	int32_t size = sizeof(int32_t);
	memcpy(output, input, sizeof(int32_t));
	return size;
}


int32_t serialize_list(char* output,t_list* input, int32_t element_size) {
	int32_t i, offset = 1;
	// Se va a guardar en la primera posicion la cantidad de elementos
	output[0] = list_size(input);
	for (i = 0; i < output[0]; i++) {
		memcpy(output + offset, list_get(input, i), element_size);
		offset += element_size;
	}
	return offset;
}

int32_t deserialize_list(t_list* output,char* input, int32_t element_size) {
	int32_t i, offset = 1;
	// Se saca de la primera posicion la cantidad de elementos
//	void* buffer;
	for (i = 0; i < input[0]; i++) {
		void* buffer = malloc(element_size);
		memcpy(buffer, input + offset, element_size);
		list_add(output, buffer);
		offset += element_size;
		free(buffer);
	}


	return offset;
}

int32_t serialize_string(char* output,char* input) {
	int32_t offset = 0;
	// Se va a guardar en la primera posicion la cantidad de chars
	output[0] = string_length(input)+1;
	offset += sizeof(char);
	memcpy(output + offset, input, output[0]);
	offset += output[0];

	return offset;
}

int32_t deserialize_string(char* output,char* input) {
	int32_t offset = 1;
	// Se saca de la primera posicion la cantidad de chars
	memcpy(output, input + offset, input[0]);
	offset += input[0];

	return offset;
}

int32_t serialize_instruction(char* output, t_instruction* input) {
	int32_t offset = 0;
	offset += serialize_string(output + offset, input->id);
	offset += serialize_list(output + offset, input->parameters, sizeof(t_parameter));

	return offset;
}

int32_t serialize_instructions_list(char* output, t_instructions_list* input) {
	int32_t i, offset = 1;
	// Cantidad de instrucciones
	output[0] = list_size(input->instructions);
	for (i = 0; i < output[0]; i++) {
		// Serializacion de cada instruccion
		offset += serialize_instruction(output + offset, list_get(input->instructions, i));
	}
	offset += serialize_int(output + offset, &(input->process_size));
	return offset;
}

int32_t deserialize_instruction(t_instruction* output, char* input) {
	int32_t offset = 0;
	offset += deserialize_string(output->id, input);
	offset += deserialize_list(output->parameters, input + offset, sizeof(t_parameter));

	return offset;
}

int32_t deserialize_instructions_list(t_instructions_list* output, char* input) {
	int32_t i, offset = 1;
	// Cantidad de instrucciones en input[0]
	for (i = 0; i < input[0]; i++) {
		t_instruction* instruction = create_instruction();
		offset += deserialize_instruction(instruction, input + offset);
		// Agrego la instruccion recibida a la lista
		list_add(output->instructions, instruction);
	}
	offset += deserialize_int(&output->process_size, input + offset);
	return offset;
}

void print_buffer(char* buffer, int32_t size) {
	int32_t i;
	printf("\nbuffer=");
	for (i = 0; i < size; i++) {
		printf("[%d]", buffer[i]);
	}
	printf("\n");
}


//Utils

int bytes_list(t_list* input, int32_t element_size){
	return list_size(input)*element_size;
}

//Instruction

t_parameter* create_parameter(){
	t_parameter* parameter = malloc(sizeof(t_parameter));
	memset(parameter, 0, sizeof(t_parameter));
	parameter->value=0;
	return parameter;
}

t_parameter* new_parameter(int32_t value){
	t_parameter* parameter = create_parameter();
	parameter->value = value;
	return parameter;
}

void parameter_destroy(t_parameter* parameter){
	if (parameter != NULL){
		free(parameter);
		parameter = NULL;
	}
}



t_instruction* create_instruction(){
	t_instruction* instruction = malloc(sizeof(t_instruction));
	memset(instruction, 0, sizeof(t_instruction));
	instruction->id = string_new();
	instruction->parameters = list_create();
	return instruction;
}

t_instruction* new_instruction(char* id, t_list* parameters) {
	t_instruction* instruction = create_instruction();

	string_append(&instruction->id, id);

	for(int i=0; i<list_size(parameters);i++)
		list_add(instruction->parameters, list_get(parameters, i));

	return instruction;
}

void instruction_destroy(t_instruction* instruction){
	list_destroy_and_destroy_elements(instruction->parameters, (void*) parameter_destroy);
	free(instruction->id);
	free(instruction);
}

int32_t bytes_instruction(t_instruction* instruction) {
	return string_length(instruction->id)+1 + bytes_list(instruction->parameters, sizeof(int32_t));

}

//Instructions_list
t_instructions_list* create_instructions_list(){
	t_instructions_list* instructions_list = malloc(sizeof(t_instructions_list));
//	memset(instructions_list, 0, sizeof(t_instructions_list));
	instructions_list->instructions = list_create();
	instructions_list->process_size = 0;
	return instructions_list;
}

t_instructions_list* new_instructions_list(t_list* instructions, int32_t process_size) {
	t_instructions_list* instructions_list = create_instructions_list();

	for(int i=0; i<list_size(instructions);i++)
		list_add(instructions_list->instructions, list_get(instructions, i));

	instructions_list->process_size = process_size;

	return instructions_list;
}

void instructions_list_destroy(t_instructions_list* instructions_list){

	list_destroy_and_destroy_elements(instructions_list->instructions, (void*) instruction_destroy);
	free(instructions_list);
}

int32_t bytes_instructions_list(t_instructions_list* instructions_list) {
	int32_t bytes = 0;

	for(int i=0;i<list_size(instructions_list->instructions);i++){
		bytes += bytes_instruction(list_get(instructions_list->instructions, i));
	}

	bytes += sizeof(int32_t);


	return bytes;

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

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
void test_serialize_int(){
	int32_t a=57,b;
	char* buffer;
	buffer = malloc(sizeof(int32_t));
	serialize_int(buffer,&a);
	print_buffer(buffer,sizeof(int32_t));
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
	list_add(instructionA->parameters, parameter);

	char* buffer = malloc(bytes_instruction(instructionA));
	memset(buffer, 0, bytes_instruction(instructionA));
	serialize_instruction(buffer,instructionA);
	print_buffer(buffer,bytes_instruction(instructionA));
	deserialize_instruction(instructionB,buffer);
	bool equal = string_equals_ignore_case(instructionB->id, instructionA->id);
	CU_ASSERT_EQUAL(equal,true);
	CU_ASSERT_EQUAL( ((t_parameter*)list_get(instructionB->parameters,0))->value, ((t_parameter*)list_get(instructionA->parameters,0))->value );

	instruction_destroy(instructionA);
	instruction_destroy(instructionB);
	free(buffer);
}
void test_serialize_instruction_list(){

	char* id =string_new();
	string_append(&id, "READ");

	t_list* parameters = list_create();
	t_parameter* parameter1 = new_parameter(2);
	t_parameter* parameter2 = new_parameter(100);
	list_add(parameters, parameter1);
	list_add(parameters, parameter2);

	t_instruction* instruction = new_instruction(id, parameters);

	t_instructions_list* instructions_listA = create_instructions_list();
	t_instructions_list* instructions_listB = create_instructions_list();

	list_add(instructions_listA->instructions, instruction);
	instructions_listA->process_size = 1000;



	char* buffer = malloc(bytes_instructions_list(instructions_listA));
	memset(buffer, 0, bytes_instructions_list(instructions_listA));
	serialize_instructions_list(buffer, instructions_listA);
	print_buffer(buffer,bytes_instructions_list(instructions_listA));
	deserialize_instructions_list(instructions_listB, buffer);
	CU_ASSERT_EQUAL(list_size(instructions_listB->instructions),list_size(instructions_listA->instructions));
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,0))->value,2);
	CU_ASSERT_EQUAL(((t_parameter*) list_get( ((t_instruction*) list_get(instructions_listB->instructions,0))->parameters,1))->value,100);

	/*Liberar las estructuras internas*/
	free(id);


	instructions_list_destroy(instructions_listA);
	instructions_list_destroy(instructions_listB);
	list_destroy_and_destroy_elements(parameters, (void*) parameter_destroy);
	free(buffer);
}

int main(int32_t argc, char** argv){
	test_serialization();
	return 0;
}




