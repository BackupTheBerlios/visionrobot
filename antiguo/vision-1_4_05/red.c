#include "red_neuronal.h"
#include "red_neuronal_sdk.h"
#include "salida_sdk.h"
#include "pipeline_sdk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char m_buffer_error[128];
  char m_salida[128];
  red_neuronal_t *m_red;
} red_dato_t;


static void red_ciclo_aux(gpointer key, gpointer value, gpointer user_data) { 
  if(GPOINTER_TO_INT(key) == PIPELINE_RED_NEURONAL) {
    modulo_t *modulo = (modulo_t*)user_data;
    red_dato_t * dato = (red_dato_t*)modulo->m_dato;
    red_neuronal_t *red = dato->m_red;
    red_neuronal_in_t * red_in = (red_neuronal_in_t *)value;
    if(red) {
      sprintf(dato->m_buffer_error, "Entrada = %p, [%p, %ix%ix%i]",
	      red_in, red_in->m_orden, red_in->m_ancho, red_in->m_alto, red_in->m_bytes);
      
      strcpy(dato->m_salida, red_neuronal_reconocer(red, red_in->m_tipo_orden, red_in->m_ancho,
						    red_in->m_alto, red_in->m_bytes, ORDEN));
      sprintf(dato->m_buffer_error, "%s, ORDEN = [%4f, %4f, %4f, %4f]", dato->m_buffer_error, red->capaSalida[1], 
	      red->capaSalida[2],red->capaSalida[3], red->capaSalida[4]);
    }

  }
}

static char *red_ciclo(modulo_t *modulo, char tipo, GHashTable *lista)//, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  red_dato_t * dato = (red_dato_t*)modulo->m_dato;
  g_hash_table_foreach(lista, red_ciclo_aux, modulo);      
  return dato->m_buffer_error;
}
static char *red_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
    if (g_hash_table_size(argumentos) < 1) {
	return "falta el nombre de archivo para cargar";
    }
    red_dato_t * dato = (red_dato_t*)modulo->m_dato;
    dato->m_red = red_neuronal_abrir(g_hash_table_lookup(argumentos, "archivo"));
    g_hash_table_insert(modulo->m_tabla, PIPELINE_SALIDA, &dato->m_salida);
    red_neuronal_t *red = dato->m_red;
    if (!red) {
      return "no se ha cargado el archivo";
    }    
    else {
	sprintf(dato->m_buffer_error, "Se ha cargado %s", (char *)g_hash_table_lookup(argumentos, "archivo"));
	return dato->m_buffer_error;
    }    
}
static char *red_cerrar(modulo_t *modulo)
{
  red_dato_t * dato = (red_dato_t*)modulo->m_dato;
  red_neuronal_t *red = dato->m_red;
  red_neuronal_borrar(&red);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Red";
  modulo->m_tipo = PIPELINE_RED_NEURONAL;
  modulo->m_iniciar = red_iniciar;
  modulo->m_cerrar = red_cerrar;
  modulo->m_ciclo = red_ciclo;
  modulo->m_dato = (red_dato_t *)malloc(sizeof(red_dato_t));
  return modulo;
}

