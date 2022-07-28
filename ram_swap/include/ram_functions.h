#ifndef RAM_FUNCTIONS_H_
#define RAM_FUNCTIONS_H_

#include <stdbool.h>
#include <pthread.h>
#include <tads/process.h>
#include <tads/page_table_request.h>
#include <libreriaConexiones.h>
#include <protocol.h>
#include <swap_utils.h>

int32_t create_pid_memory(t_process* process);
int32_t process_total_pages_needed(int32_t tamanioProceso);
int32_t process_complete_second_level_tables(int32_t total_pages_needed);
first_level_entries_t* create_first_level_entry(int32_t second_level_entries, int32_t process_pid, int32_t *swap_id);
second_level_page_table_t* create_second_level_table(int32_t pages, int32_t process_pid, int32_t *swap_id);
page_t* create_second_level_page(int32_t process_pid, int32_t *swap_id);
void create_process_state(int32_t pid_first_level_table_number, t_process* process);
bool process_table_not_exists(int32_t pid);

int32_t suspend_pid(t_process* process);
void swap_first_level_entry(void* entry);
void swap_second_level_entry(void* entry);

void* read_frame(int32_t frame_number);
void free_memory(int32_t first_level_table_number);
void free_first_level_entry(void* entry);
void free_second_level_entry(void* entry);
int32_t get_second_level_page_table(t_page_table_request* page_table_request);


#endif /* RAM_FUNCTIONS_H_ */


