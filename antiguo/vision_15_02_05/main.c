#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    if (argc < 1) {
	printf("Faltan argumentos.\n");
	return -1;
    }
    
    else {
	elemento_t * e = pipeline_cargar(argv[1]);
	pipeline_iniciar_todo(e);	
	while (1) {
	  pipeline_ciclo(e, 0);
	}
	pipeline_cerrar_todo(e);
	pipeline_borrar(e);
	return 0;
    }
}


