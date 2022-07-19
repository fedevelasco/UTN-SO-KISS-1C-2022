#ifndef RAM_FUNCTIONS_H_
#define RAM_FUNCTIONS_H_

#include <stdbool.h>
#include <tads/process.h>
#include <libreriaConexiones.h>

static bool create_process_in_memory(t_process* process);
bool process_table_exists(int32_t pid);

#endif /* RAM_FUNCTIONS_H_ */


