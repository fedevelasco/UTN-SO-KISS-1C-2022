#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>

// Inicia el config para poder tomar los datos del configuracion.txt
t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("configuracion.config");

	if (nuevo_config == NULL){
		printf("Error al abrir el configuracion.config\n");
		exit(2);
	}

	return nuevo_config;
}

void terminar_config(t_config* config)
{

	config_destroy(config);

}