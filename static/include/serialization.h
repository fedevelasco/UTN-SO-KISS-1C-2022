#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <commons/collections/list.h>
#include <commons/string.h>

#include <CUnit/Basic.h>

#include "list_utils.h"

#include "tads/parameter.h"
#include "tads/instruction.h"
#include "tads/instructions_list.h"
#include "tads/op_code.h"
#include "tads/buffer.h"
#include "tads/package.h"
#include "tads/process.h"
#include "tads/memory_config.h"
#include "tads/page_table_request.h"
#include "tads/memory_write_request.h"



// Serializacion y deserializacion tipos de datos comunes
/**
* @NAME: serialize_int
* @DESC: Serializa un uint32_t en input hacia un buffer output y retorna el offset.
*/
uint32_t serialize_int(char* output,uint32_t* input);

/**
* @NAME: deserialize_int
* @DESC: Deserializa un buffer input en un output uint32_t y retorna el offset.
*/
uint32_t deserialize_int(uint32_t* output, char* input);

/**
* @NAME: serialize_opcode
* @DESC: Serializa un op_code en input hacia un buffer output y retorna el offset.
*/
uint32_t serialize_opcode(char* output, t_op_code* input);

/**
* @NAME: deserialize_opcode
* @DESC: Deserializa un buffer input en un output op_code y retorna el offset.
*/
uint32_t deserialize_opcode(t_op_code* output, char* input);

/**
* @NAME: serialize_list
* @DESC: Serializa una lista t_list* en input hacia un buffer output y retorna el offset. Hay que pasarle el element_size.
*/
uint32_t serialize_list(char* output,t_list* input, uint32_t element_size);

/**
* @NAME: deserialize_list
* @DESC: Deserializa un buffer input en una lista output de elementos con tamanio element_size y retorna el offset.
*/
uint32_t deserialize_list(t_list* output,char* input, uint32_t element_size);

/**
* @NAME: serialize_string
* @DESC: Serializa una string en input hacia un buffer output y retorna el offset.
*/
uint32_t serialize_string(char* output,char* input);

/**
* @NAME: deserialize_string
* @DESC: Deserializa un buffer input en un output string y retorna el offset.
*/
uint32_t deserialize_string(char* output,char* input);

// Serializacion y deserializacion instrucciones
/**
* @NAME: deserialize_parameters
* @DESC: Deserializa un buffer en input hacia una lista de parameters en output y retorna el offset.
*/
uint32_t deserialize_parameters(t_list* output,char* input, uint32_t element_size);

/**
* @NAME: serialize_instruction
* @DESC: Serializa una instruction en input hacia un buffer output y retorna el offset.
*/
uint32_t serialize_instruction(char* output, t_instruction* input);

/**
* @NAME: serialize_instructions_list
* @DESC: Serializa una instructions_list en input hacia un buffer output y retorna el offset.
*/
uint32_t serialize_instructions_list(char* output, t_instructions_list* input);

/**
* @NAME: deserialize_instruction
* @DESC: Deserializa un buffer en input hacia una instruction en output y retorna el offset.
*/
uint32_t deserialize_instruction(t_instruction* output, char* input);

/**
* @NAME: deserialize_instructions_list
* @DESC: Deserializa un buffer en input hacia una instructions_list en output y retorna el offset.
*/
uint32_t deserialize_instructions_list(t_instructions_list* output, char* input);

/**
* @NAME: serialize_buffer_stream
* @DESC: Serializa el stream de un buffer en output y retorna el offset.
*/
uint32_t serialize_buffer_stream(char* output, t_buffer* input);

/**
* @NAME: serialize_buffer
* @DESC: Serializa un t_buffer en output y retorna el offset.
*/
uint32_t serialize_buffer(char* output, t_buffer* input);

/**
* @NAME: serialize_package
* @DESC: Serializa un t_package en char* output y lo retorna. 
*/
char* serialize_package(t_package* package, uint32_t bytes);

/**
* @NAME: new_instruction_buffer
* @DESC: Crea una instancia de "t_buffer" que contiene la instructions_list serializada. Ademas, logea el proceso.
*/
t_buffer* new_instruction_buffer(t_instructions_list* instructions_list, t_log* logger);

/**
* @NAME: serialize_process
* @DESC: Serializa un t_process en output y retorna el offset.
*/
uint32_t serialize_process(char* output, t_process* input);

/**
* @NAME: deserialize_process
* @DESC: Deserializa un buffer en input hacia un t_process en output y retorna el offset.
*/
uint32_t deserialize_process(t_process* output, char* input);

/**
* @NAME: serialize_memory_config
* @DESC: Serializa un t_memory_config en output y retorna el offset.
*/
uint32_t serialize_memory_config(char* output, t_memory_config* input);

/**
* @NAME: deserialize_memory_config
* @DESC: Deserializa un buffer en input hacia un t_memory_config en output y retorna el offset.
*/
uint32_t deserialize_memory_config(t_memory_config* output, char* input);

/**
* @NAME: serialize_page_table_request
* @DESC: Serializa un t_page_table_request en output y retorna el offset.
*/
uint32_t serialize_page_table_request(char* output, t_page_table_request* input);

/**
* @NAME: deserialize_page_table_request
* @DESC: Deserializa un buffer en input hacia un t_page_table_request en output y retorna el offset.
*/
uint32_t deserialize_page_table_request(t_page_table_request* output, char* input);

/**
* @NAME: serialize_memory_write_request
* @DESC: Serializa un t_memory_write_request en output y retorna el offset.
*/
uint32_t serialize_memory_write_request(char* output, t_memory_write_request* input);

/**
* @NAME: deserialize_memory_write_request
* @DESC: Deserializa un buffer en input hacia un t_memory_write_request en output y retorna el offset.
*/
uint32_t deserialize_memory_write_request(t_memory_write_request* output, char* input);



// Debug
void print_buffer(char* buffer, uint32_t size);

// Test Call
int test_serialization();

// Tests
void test_serialize_int();
void test_serialize_pint();
void test_serialize_list();
void test_serialize_string();
void test_serialize_instruction();
void test_serialize_instruction_list();
void test_serialize_process();


#endif /* SERIALIZATION_H_ */


