#include "red_neuronal.h"
#include "red_neuronal_sdk.h"
#include "salida_sdk.h"
#include "pipeline_sdk.h"
#include <stdio.h>
#include <string.h>

static char buffer_error[128];

static char *ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
    red_neuronal_in_t * red_in = (red_neuronal_in_t *) in->m_dato;
    if(red_in) {
      sprintf(buffer_error, "Entrada = %p, [%p, %ix%ix%i]",
	      red_in, red_in->m_orden, red_in->m_ancho, red_in->m_alto, red_in->m_bytes);
      red_neuronal_t *red = (red_neuronal_t *)modulo->m_dato;
      if (red) {
	out->m_tipo = PIPELINE_RED_NEURONAL;
	out->m_dato = (salida_in_t)
	  red_neuronal_reconocer(red, red_in->m_orden, red_in->m_ancho,
				 red_in->m_alto, red_in->m_bytes, ORDEN);
	sprintf(buffer_error, "%s, ORDEN = [%4f, %4f, %4f, %4f]", buffer_error, red->capaSalida[1], 
		red->capaSalida[2],red->capaSalida[3], red->capaSalida[4]);
	free(red_in->m_orden);
	free(red_in->m_tipo_orden);	
      }      
      else {
	out->m_dato = 0;
      }
    }
    else {
      return "imagen vacía";
    }
    return buffer_error;
}
static char *iniciar(modulo_t *modulo, int argc, const char **argv)
{
    if (argc < 2) {
	return "falta el nombre de archivo para cargar";
    }
    if (!strcmp(argv[0], "archivo")) {
	modulo->m_dato = red_neuronal_abrir(argv[1]);
    }
    red_neuronal_t *red = (red_neuronal_t *)modulo->m_dato;
    if (!red) {
      return "no se ha cargado el archivo";
    }    
    else {
	sprintf(buffer_error, "Se ha cargado %s", argv[1]);
	return buffer_error;
    }    
}
static char *cerrar(modulo_t *modulo)
{
  red_neuronal_t *red = (red_neuronal_t *)modulo->m_dato;
  red_neuronal_borrar(&red);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Red";
  modulo->m_iniciar = iniciar;
  modulo->m_cerrar = cerrar;
  modulo->m_ciclo = ciclo;
  modulo->m_dato = 0;
  return modulo;
}

