#ifndef RAM_FUNCTIONS_H_
#define RAM_FUNCTIONS_H_

#include <stdbool.h>
#include <tads/process.h>
#include <libreriaConexiones.h>
#include <ram_utils.h>

bool create_process_in_memory(t_process* process);
bool process_table_not_exists(int32_t pid);
first_level_page_table_t* get_process_table(int32_t pid);
second_level_page_tables_t* create_empty_second_level_page_table();
page_t* create_empty_page();
int32_t get_pid_first_level_table_number(int32_t pid);


#endif /* RAM_FUNCTIONS_H_ */


