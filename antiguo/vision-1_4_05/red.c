#include "red_neuronal.h"
#include "red_neuronal_sdk.h"
#include "pipeline_sdk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUERTO_TEXTO "salida_texto"
#define PUERTO_IMAGEN "entrada_imagen"

typedef struct {
  char m_buffer_error[128];
  char m_salida[128];
  red_neuronal_t *m_red_ordenes;
  red_neuronal_t *m_red_parametros;
  //  char *m_error;
} red_dato_t;

static char *red_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  if(!strcmp(puerto, PUERTO_IMAGEN)) {
    red_dato_t * dato = (red_dato_t*)modulo->m_dato;
    red_neuronal_t *red_o = dato->m_red_ordenes;
    red_neuronal_t *red_p = dato->m_red_parametros;
    red_neuronal_in_t * red_in = (red_neuronal_in_t *)value;
    if(red_o && red_p) {
      sprintf(dato->m_salida, "orden = %s, parametro = %s", 
	      red_neuronal_reconocer(red_o, red_in->m_tipo_orden,
				     red_in->m_ancho, red_in->m_alto,
				     red_in->m_bytes, ORDEN),
	      red_neuronal_reconocer(red_p, red_in->m_orden,
				     red_in->m_ancho, red_in->m_alto,
				     red_in->m_bytes, PARAM));
    }
  }
  return 0;
}
static char *red_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
    if (g_hash_table_size(argumentos) < 2) {
	return "falta algun nombre de archivo para cargar";
    }
    red_dato_t * dato = (red_dato_t*)modulo->m_dato;
    char *archivo_ordenes = g_hash_table_lookup(argumentos, "archivo_ordenes");
    char *archivo_parametros = g_hash_table_lookup(argumentos, "archivo_parametros");
    dato->m_red_ordenes = red_neuronal_abrir(archivo_ordenes);
    dato->m_red_parametros = red_neuronal_abrir(archivo_parametros);
    g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, &dato->m_salida);
    red_neuronal_t *red_o = dato->m_red_ordenes;
    red_neuronal_t *red_p = dato->m_red_parametros;
    if (!red_o || !red_p) {
      return "no se ha cargado algun archivo";
    }    
    else {
	sprintf(dato->m_buffer_error, "ordenes = %s, parametros = %s", archivo_ordenes, archivo_parametros);
	return dato->m_buffer_error;
    }    
}
static char *red_cerrar(modulo_t *modulo)
{
  red_dato_t * dato = (red_dato_t*)modulo->m_dato;
  red_neuronal_t *red_o = dato->m_red_ordenes;
  red_neuronal_t *red_p = dato->m_red_parametros;
  red_neuronal_borrar(&red_o);
  red_neuronal_borrar(&red_p);
  free(dato);
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
  modulo->m_dato = (red_dato_t *)malloc(sizeof(red_dato_t));
  return modulo;
}

