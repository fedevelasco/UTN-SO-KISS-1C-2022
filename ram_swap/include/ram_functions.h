#ifndef RAM_FUNCTIONS_H_
#define RAM_FUNCTIONS_H_

#include <stdbool.h>
#include <pthread.h>
#include <tads/process.h>
#include <tads/page_table_request.h>
#include <libreriaConexiones.h>
#include <protocol.h>
#include <swap_utils.h>

uint32_t create_pid_memory(t_process* process);
uint32_t process_total_pages_needed(uint32_t tamanioProceso);
uint32_t process_complete_second_level_tables(uint32_t total_pages_needed);
first_level_entries_t* create_first_level_entry(uint32_t second_level_entries, uint32_t process_pid, uint32_t *swap_id);
second_level_page_table_t* create_second_level_table(uint32_t pages, uint32_t process_pid, uint32_t *swap_id);
page_t* create_second_level_page(uint32_t process_pid, uint32_t *swap_id);
void create_process_state(uint32_t pid_first_level_table_number, t_process* process);
bool process_table_not_exists(uint32_t pid);

uint32_t suspend_pid(t_process* process);
void swap_first_level_entry(void* entry);
void swap_second_level_entry(void* entry);

void* read_frame(uint32_t frame_number);
void free_memory(uint32_t first_level_table_number);
void free_first_level_entry(void* entry);
void free_second_level_entry(void* entry);
uint32_t process_frames(uint32_t process_size);
uint32_t get_second_level_page_table(t_page_table_request* page_table_request);
uint32_t read_fisical_address(uint32_t fisical_address);
uint32_t first_level_page(uint32_t swap_page_id);
uint32_t second_level_page(uint32_t swap_page_id);
void write_memory_data(t_memory_write_request* write);
uint32_t get_frame_number(t_page_table_request* page_table_request, bool isWrite);
page_t* find_victim_page_to_replace(process_state_t* process_state, page_t* page);
void write_page_in_memory(uint32_t frame, void* swap_page_data);
t_list* get_second_level_pages(uint32_t first_level_page_table_id);
page_t* replace_with_clock(process_state_t* process_state, t_list* all_process_pages_list, page_t* page);
page_t* replace_with_clock_m(process_state_t* process_state, t_list* all_process_pages_list, page_t* page);


#endif /* RAM_FUNCTIONS_H_ */


