#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <commons/string.h>

#include "CUnit/Basic.h"

#include "tads/instruction.h"
#include "tads/instructions_list.h"
#include "tads/parameter.h"


// Serializacion y deserializacion tipos de datos comunes
/**
* @NAME: serialize_int
* @DESC: Serializa un int32_t en input hacia un buffer output.
*/
int32_t serialize_int(char* output,int32_t* input);

/**
* @NAME: deserialize_int
* @DESC: Deserializa un buffer input en un output int32_t.
*/
int32_t deserialize_int(int32_t* output, char* input);

/**
* @NAME: serialize_list
* @DESC: Serializa una lista t_list* en input hacia un buffer output. Hay que pasarle el element_size.
*/
int32_t serialize_list(char* output,t_list* input, int32_t element_size);

/**
* @NAME: deserialize_list
* @DESC: Deserializa un buffer input en una lista output de elementos con tamanio element_size.
*/
int32_t deserialize_list(t_list* output,char* input, int32_t element_size);

/**
* @NAME: serialize_string
* @DESC: Serializa una string en input hacia un buffer output.
*/
int32_t serialize_string(char* output,char* input);

/**
* @NAME: deserialize_string
* @DESC: Deserializa un buffer input en un output string.
*/
int32_t deserialize_string(char* output,char* input);

// Serializacion y deserializacion instrucciones
/**
* @NAME: deserialize_parameters
* @DESC: Deserializa un buffer en input hacia una lista de parameters en output.
*/
int32_t deserialize_parameters(t_list* output,char* input, int32_t element_size);

/**
* @NAME: serialize_instruction
* @DESC: Serializa una instruction en input hacia un buffer output.
*/
int32_t serialize_instruction(char* output, t_instruction* input);

/**
* @NAME: serialize_instructions_list
* @DESC: Serializa una instructions_list en input hacia un buffer output.
*/
int32_t serialize_instructions_list(char* output, t_instructions_list* input);

/**
* @NAME: deserialize_instruction
* @DESC: Deserializa un buffer en input hacia una instruction en output.
*/
int32_t deserialize_instruction(t_instruction* output, char* input);

/**
* @NAME: deserialize_instructions_list
* @DESC: Deserializa un buffer en input hacia una instructions_list en output.
*/
int32_t deserialize_instructions_list(t_instructions_list* output, char* input);

//Utils
/**
* @NAME: bytes_list
* @DESC: Retorna la cantidad de bytes ocupados por una lista con elementos de tamanio element_size.
*/
int32_t bytes_list(t_list* input, int32_t element_size);

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


