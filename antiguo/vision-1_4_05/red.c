#include "red_neuronal.h"
#include "red_neuronal_sdk.h"

#include "pipeline_sdk.h"
#include <stdio.h>
#include <string.h>

static modulo_t modulo_pipeline;
static red_neuronal_t *red = 0;
char *ciclo(const void *in, void **out)
{
    red_neuronal_in_t * red_in = (red_neuronal_in_t *) in;
    sprintf(modulo_pipeline.m_error, "Entrada = %p, imagen %p, de %ix%i",
	     red_in, red_in->m_orden, red_in->m_ancho, red_in->m_alto);
    if (red) {
	*out =
	    red_neuronal_reconocer(red, red_in->m_orden, red_in->m_ancho,
				   red_in->m_alto, red_in->m_bytes, ORDEN);
	sprintf(modulo_pipeline.m_error, "%s - ORDEN[%4f, %4f, %4f, %4f]", modulo_pipeline.m_error, red->capaSalida[1], 
		red->capaSalida[2],red->capaSalida[3], red->capaSalida[4]);
	free(red_in->m_orden);
	free(red_in->m_tipo_orden);	
    }
    
    else {
	*out = 0;
    }
    return modulo_pipeline.m_error;
}
char *iniciar(int argc, const char **argv)
{
    if (argc < 2) {
	return "faltan el nombre de archivo para cargar";
    }
    if (!strcmp(argv[0], "archivo")) {
	red = red_neuronal_abrir(argv[1]);
    }
    if (!red)
	return "no se ha cargado el archivo";
    
    else
	sprintf(modulo_pipeline.m_error, "Se ha cargado %s", argv[1]);
    return modulo_pipeline.m_error;
}
char *cerrar()
{
    red_neuronal_borrar(&red);
    return "cerrado";
}

modulo_t * get_modulo()
{
    return &modulo_pipeline;
}
void __attribute__ ((constructor)) init(void)
{
    modulo_pipeline.m_nombre = "Módulo de red neuronal";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
} 
void __attribute__ ((destructor)) fini(void)
{
} 
