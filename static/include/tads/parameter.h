#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <stdlib.h>
#include <stdint.h>

    typedef struct {
	uint32_t value;
    } t_parameter;

    /**
    * @NAME: create_parameter
    * @DESC: Crea una instancia vacia de "t_parameter".
    */
    t_parameter* create_parameter();

    /**
    * @NAME: new_parameter
    * @DESC: Crea una instancia de "t_parameter".
    */
    t_parameter* new_parameter(uint32_t value);


    /**
    * @NAME: parameter_destroy
    * @DESC: Libera la memoria ocupada por una instancia de "t_parameter".
    */
    void parameter_destroy(t_parameter* parameter);

#endif
