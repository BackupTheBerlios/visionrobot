#include "filtro.h"
#include "pipeline_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
  filtro_t *m_filtro;
  ventana_imagen_in_t m_imagen;
  char m_buffer_error[128];  
} dato_filtro_t;

static void filtro_ciclo_aux(gpointer key, gpointer value, gpointer user_data) {
  if(GPOINTER_TO_INT(key) == PIPELINE_FILTRO_GESTOS) {
    modulo_t *modulo = (modulo_t*)user_data;
    filtro_t *filtro = (filtro_t*)((dato_filtro_t *)modulo->m_dato)->m_filtro;
    filtro->m_buffer = (filtro_gestos_in_t *) value;
    dato_filtro_t* dato = (dato_filtro_t* )modulo->m_dato;
    switch(filtro->m_buffer->m_tipo) {
    case PIPELINE_FILTRO_GESTOS_IMAGEN:

      if(filtro->m_buffer) {
	filtro_gestos_filtrar(filtro);
	
	dato->m_imagen.m_imagen =  filtro->m_salida->m_tipo_orden;
	dato->m_imagen.m_alto =  filtro->m_salida->m_alto;
	dato->m_imagen.m_ancho =  filtro->m_salida->m_ancho;
	dato->m_imagen.m_bytes =  filtro->m_salida->m_bytes;

	sprintf(((dato_filtro_t *)modulo->m_dato)->m_buffer_error,
		"IN [%ix%ix%i], OUT [%ix%ix%i]",
		filtro->m_buffer->m_dato.imagen.m_ancho, filtro->m_buffer->m_dato.imagen.m_alto,
		filtro->m_buffer->m_dato.imagen.m_bytes, filtro->m_salida->m_ancho,
		filtro->m_salida->m_alto, filtro->m_salida->m_bytes);
      }
      break;
    case PIPELINE_FILTRO_GESTOS_PARAMETROS:
      break;
    }
  }
}

static char *filtro_ciclo(modulo_t *modulo, char tipo, GHashTable *lista)//, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  filtro_t *filtro = (filtro_t*)((dato_filtro_t *)modulo->m_dato)->m_filtro;
  if (filtro) {
    g_hash_table_foreach(lista, filtro_ciclo_aux, modulo);
    return ((dato_filtro_t *)modulo->m_dato)->m_buffer_error;
  }
  else {
    return "fallo en el filtro";
  }
}
static char *filtro_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  
  if (g_hash_table_size(argumentos) < 12) {
    return "faltan parámetros";
  }
  ((dato_filtro_t *)modulo->m_dato)->m_filtro = filtro_gestos_crear();
  filtro_t *filtro = ((dato_filtro_t *)modulo->m_dato)->m_filtro;
  GHashTable *tabla = modulo->m_tabla;

  g_hash_table_insert(tabla, GINT_TO_POINTER(PIPELINE_RED_NEURONAL),filtro->m_salida);
  g_hash_table_insert(tabla, GINT_TO_POINTER(PIPELINE_VENTANA_IMAGEN), &(((dato_filtro_t *)modulo->m_dato)->m_imagen));

  filtro_gestos_set_color(filtro,
			      atoi(g_hash_table_lookup(argumentos, "orden_superior_rojo" )),
			      atoi(g_hash_table_lookup(argumentos, "orden_inferior_rojo")),
			      atoi(g_hash_table_lookup(argumentos, "orden_superior_verde")),
			      atoi(g_hash_table_lookup(argumentos, "orden_inferior_verde")),
			      atoi(g_hash_table_lookup(argumentos, "orden_superior_azul")),
			      atoi(g_hash_table_lookup(argumentos, "orden_inferior_azul")),
			      atoi(g_hash_table_lookup(argumentos, "param_superior_rojo")),
			      atoi(g_hash_table_lookup(argumentos, "param_inferior_rojo")),
			      atoi(g_hash_table_lookup(argumentos, "param_superior_verde")),
			      atoi(g_hash_table_lookup(argumentos, "param_inferior_verde")),
			      atoi(g_hash_table_lookup(argumentos, "param_superior_azul")),
			      atoi(g_hash_table_lookup(argumentos, "param_inferior_azul")));
    return "iniciado";
}
static char *filtro_cerrar(modulo_t *modulo)
{
  filtro_gestos_borrar((filtro_t**)&(((dato_filtro_t *)modulo->m_dato)->m_filtro));
  free((dato_filtro_t *)modulo->m_dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));  
  modulo->m_nombre = "Filtro";
  modulo->m_tipo = PIPELINE_FILTRO_GESTOS;
  modulo->m_iniciar = filtro_iniciar;
  modulo->m_cerrar = filtro_cerrar;
  modulo->m_ciclo = filtro_ciclo;
  modulo->m_dato = (dato_filtro_t *)malloc(sizeof(dato_filtro_t));
  return modulo;

}
