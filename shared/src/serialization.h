#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <commons/string.h>

#include "CUnit/Basic.h"

//Esto tiene que venir de console, hacer el include.

typedef struct {
	int32_t value;
} t_parameter;

typedef struct {
		char* id;
		t_list* parameters;
	} t_instruction;

typedef struct {
		t_list *instructions;
		int32_t process_size;
	} t_instructions_list;

// Serializacion y deserializacion tipos de datos comunes
int32_t serialize_int(char* output,int32_t* input);
int32_t deserialize_int(int32_t* output, char* input);
int32_t serialize_list(char* output,t_list* input, int32_t element_size);
int32_t deserialize_list(t_list* output,char* input, int32_t element_size);
int32_t serialize_string(char* output,char* input);
int32_t deserialize_string(char* output,char* input);

// Serializacion y deserializacion instrucciones
int32_t serialize_instruction(char* output, t_instruction* input);
int32_t serialize_instructions_list(char* output, t_instructions_list* input);
int32_t deserialize_instruction(t_instruction* output, char* input);
int32_t deserialize_instructions_list(t_instructions_list* output, char* input);

//Utils
int32_t bytes_list(t_list* input, int32_t element_size);

t_parameter* create_parameter();
t_parameter* new_parameter(int32_t value);
void parameter_destroy(t_parameter* parameter);
t_instruction* create_instruction();
t_instruction* new_instruction(char* id, t_list* parameters);
void instruction_destroy(t_instruction* instruction);
int32_t bytes_instruction(t_instruction* instruction);
t_instructions_list* create_instructions_list();
t_instructions_list* new_instructions_list(t_list* instructions, int32_t process_size);
void instructions_list_destroy(t_instructions_list* instructions_list);
int32_t bytes_instructions_list(t_instructions_list* instructions_list);

// Debug
void print_buffer(char* buffer, int size);

// Test Call
int test_serialization();

// Tests
void test_serialize_int();
void test_serialize_pint();
void test_serialize_list();
void test_serialize_string();
void test_serialize_instruction();
void test_serialize_instruction_list();


#endif /* SERIALIZATION_H_ */


