#ifndef RAM_FUNCTIONS_H_
#define RAM_FUNCTIONS_H_

#include <stdbool.h>
#include <tads/process.h>
#include <libreriaConexiones.h>
#include <ram_utils.h>

int32_t create_pid_memory(t_process* process);
int32_t process_total_pages_needed(int32_t tamanioProceso);
int32_t process_complete_second_level_table_pages(total_pages_needed);
first_level_entries_t* create_first_level_entry(int32_t second_level_entries, int32_t process_pid, int32_t *swap_id);
second_level_page_table_t* create_second_level_table(int32_t pages, int32_t process_pid, int32_t *swap_id);
page_t* create_second_level_page(int32_t process_pid, int32_t *swap_id);
void process_state(int32_t pid_first_level_table_number, t_process* process);
bool process_table_not_exists(int32_t pid);

#endif /* RAM_FUNCTIONS_H_ */


