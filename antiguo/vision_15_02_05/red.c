#include "red_neuronal.h"
#include "red_neuronal_sdk.h"
#include "salida_sdk.h"
#include "pipeline_sdk.h"
#include <stdio.h>
#include <string.h>

static char buffer_error[128];

static char *red_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
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
static char *red_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
    if (g_hash_table_size(argumentos) < 1) {
	return "falta el nombre de archivo para cargar";
    }
    modulo->m_dato = red_neuronal_abrir(g_hash_table_lookup(argumentos, "archivo"));
    red_neuronal_t *red = (red_neuronal_t *)modulo->m_dato;
    if (!red) {
      return "no se ha cargado el archivo";
    }    
    else {
	sprintf(buffer_error, "Se ha cargado %s", g_hash_table_lookup(argumentos, "archivo"));
	return buffer_error;
    }    
}
static char *red_cerrar(modulo_t *modulo)
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
  modulo->m_iniciar = red_iniciar;
  modulo->m_cerrar = red_cerrar;
  modulo->m_ciclo = red_ciclo;
  modulo->m_dato = 0;
  return modulo;
}

