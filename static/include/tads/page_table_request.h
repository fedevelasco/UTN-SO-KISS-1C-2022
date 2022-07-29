#ifndef PAGE_TABLE_REQUEST_H_
#define PAGE_TABLE_REQUEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct page_table_request {
	uint32_t table_number;
	uint32_t entry_number;
	uint32_t pid;
} t_page_table_request;


/**
* @NAME: create_page_table_request
* @DESC: Crea una instancia vacia de "t_page_table_request".
*/
t_page_table_request* create_page_table_request();



/**
* @NAME: page_table_request_destroy
* @DESC: Libera la memoria ocupada por una instancia de "t_page_table_request".
*/
void page_table_request_destroy(t_page_table_request* page_table_request);


#endif
