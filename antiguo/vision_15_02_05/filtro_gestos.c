#include "filtro.h"
#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
 static modulo_t modulo_pipeline;
static filtro_t *filtro = 0;
int i = 0;
void escribe(char *imagen, int ancho, int alto, int bytes)
{
    i++;
    if (i != 3)
	return;
    int i, j;
    for (i = 0; i < alto * ancho; i++) {
	
	    //for(j = 0; j < 10/*ancho * bytes*/; j++) {
	    /*if(imagen[(10 * i) + j]) */ printf("%i", imagen[i]);
	
	    //if(imagen[(alto * i) + j] != 0){
	    //fprintf(archivo, "JOOOOOOOODER");
	    //}
	    //}
    }
}
char *ciclo(const void *in, void **out)
{
    if (filtro) {
	filtro->m_buffer = (filtro_gestos_in_t *) in;
	
	    //escribe(filtro->m_buffer->m_imagen, filtro->m_buffer->m_ancho, filtro->m_buffer->m_alto,filtro->m_buffer->m_bytes);
	    *out = filtro_gestos_filtrar(filtro);
	
	    //escribe(filtro->m_salida->m_orden, filtro->m_salida->m_ancho, filtro->m_salida->m_alto,filtro->m_salida->m_bytes);
	    sprintf(modulo_pipeline.m_error,
		    "Ciclo. IN [%ix%i, %i bytes], OUT [%ix%i, %i bytes]",
		    filtro->m_buffer->m_ancho, filtro->m_buffer->m_alto,
		    filtro->m_buffer->m_bytes, filtro->m_salida->m_ancho,
		    filtro->m_salida->m_alto, filtro->m_salida->m_bytes);
	return modulo_pipeline.m_error;
    }
    return "fallo en el filtro";
}
int valor(const char *tipo, int argc, const char **argv)
{
    int i;
    for (i = 0; i < argc; i += 2) {
	if (!strcmp(tipo, argv[i])) {
	    return atoi(argv[i + 1]);
	}
    }
    return 0;
}
char *iniciar(int argc, const char **argv)
{
    if (argc < 12) {
	return "faltan parámetros";
    }
    filtro = filtro_gestos_crear();
    filtro_gestos_set_color(filtro,
			      valor("orden_superior_rojo", argc, argv),
			      valor("orden_inferior_rojo", argc, argv),
			      valor("orden_superior_verde", argc, argv),
			      valor("orden_inferior_verde", argc, argv),
			      valor("orden_superior_azul", argc, argv),
			      valor("orden_inferior_azul", argc, argv),
			      valor("param_superior_rojo", argc, argv),
			      valor("param_inferior_rojo", argc, argv),
			      valor("param_superior_verde", argc, argv),
			      valor("param_inferior_verde", argc, argv),
			      valor("param_superior_azul", argc, argv),
			      valor("param_inferior_azul", argc, argv));
    return "iniciado";
}
char *cerrar()
{
    filtro_gestos_borrar(&filtro);
    return "cerrado";
}

modulo_t * get_modulo()
{
    return &modulo_pipeline;
}
void __attribute__ ((constructor)) init(void)
{
    modulo_pipeline.m_nombre = "Módulo de filtro de imágenes";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
} void __attribute__ ((destructor)) fini(void)
{
} 
