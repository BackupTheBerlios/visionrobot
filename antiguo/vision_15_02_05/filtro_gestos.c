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
  char *m_error;
} dato_filtro_t;

#define PUERTO_IMAGEN "entrada_imagen"
#define PUERTO_PARAMETROS "entrada_parametros"
#define PUERTO_RED "salida_red"
#define PUERTO_VENTANA "salida_imagen"
/*
static void filtro_ciclo_aux(gpointer key, gpointer value, gpointer user_data) {
  if(GPOINTER_TO_INT(key) == PIPELINE_FILTRO_GESTOS) {
    modulo_t *modulo = (modulo_t*)user_data;
    dato_filtro_t* dato = (dato_filtro_t* )modulo->m_dato;
    filtro_t *filtro = (filtro_t*)dato->m_filtro;    
    filtro_gestos_in_t *parametros;
    filtro_gestos_in_t *filtro_in = (filtro_gestos_in_t *)value;
    switch(filtro_in->m_tipo) {
    case PIPELINE_FILTRO_GESTOS_IMAGEN:
      filtro->m_buffer = (filtro_gestos_in_t *) value;
      if(filtro->m_buffer) {
	filtro_gestos_filtrar(filtro);
	
	dato->m_imagen.m_imagen = filtro->m_salida->m_tipo_orden;
	dato->m_imagen.m_alto = filtro->m_salida->m_alto;
	dato->m_imagen.m_ancho = filtro->m_salida->m_ancho;
	dato->m_imagen.m_bytes = filtro->m_salida->m_bytes;
	dato->m_error = 0;
      }
      break;
    case PIPELINE_FILTRO_GESTOS_PARAMETROS:
      parametros = (filtro_gestos_in_t*)value;
      
      if(parametros->m_dato.parametros.m_cambio) {
	filtro_gestos_set_color(filtro,
				parametros->m_dato.parametros.m_rojo_sup_orden,
				parametros->m_dato.parametros.m_rojo_inf_orden,
				parametros->m_dato.parametros.m_verde_sup_orden,
				parametros->m_dato.parametros.m_verde_inf_orden,
				parametros->m_dato.parametros.m_azul_sup_orden,
				parametros->m_dato.parametros.m_azul_inf_orden,
				parametros->m_dato.parametros.m_rojo_sup_param,
				parametros->m_dato.parametros.m_rojo_inf_param,
				parametros->m_dato.parametros.m_verde_sup_param,
				parametros->m_dato.parametros.m_verde_inf_param,
				parametros->m_dato.parametros.m_azul_sup_param,
				parametros->m_dato.parametros.m_azul_inf_param);
	sprintf(dato->m_buffer_error,
		"Colores: ORDEN [%i, %i, %i, %i, %i, %i], PARAM [%i, %i, %i, %i, %i, %i]",
		parametros->m_dato.parametros.m_rojo_sup_orden,
		parametros->m_dato.parametros.m_rojo_inf_orden,
		parametros->m_dato.parametros.m_verde_sup_orden,
		parametros->m_dato.parametros.m_verde_inf_orden,
		parametros->m_dato.parametros.m_azul_sup_orden,
		parametros->m_dato.parametros.m_azul_inf_orden,
		parametros->m_dato.parametros.m_rojo_sup_param,
		parametros->m_dato.parametros.m_rojo_inf_param,
		parametros->m_dato.parametros.m_verde_sup_param,
		parametros->m_dato.parametros.m_verde_inf_param,
		parametros->m_dato.parametros.m_azul_sup_param,
		parametros->m_dato.parametros.m_azul_inf_param);
	dato->m_error = dato->m_buffer_error;
	parametros->m_dato.parametros.m_cambio = 0;
      }
      else {
	dato->m_error = 0;
      }
      break;
    }
  }
}
*/
static char *filtro_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  filtro_t *filtro = (filtro_t*)dato->m_filtro;

  if (filtro) {
    if(!strcmp(puerto, PUERTO_IMAGEN)) {
      filtro->m_buffer = (filtro_gestos_in_imagen_t *) value;
      if(filtro->m_buffer) {
	filtro_gestos_filtrar(filtro);	
	dato->m_imagen.m_imagen =  filtro->m_salida->m_tipo_orden;
	dato->m_imagen.m_alto =  filtro->m_salida->m_alto;
	dato->m_imagen.m_ancho =  filtro->m_salida->m_ancho;
	dato->m_imagen.m_bytes =  filtro->m_salida->m_bytes;
	dato->m_error = 0;
      }
    }
    else if(!strcmp(puerto, PUERTO_PARAMETROS)) {
      filtro_gestos_in_parametros_t* parametros = (filtro_gestos_in_parametros_t*)value;
      filtro_gestos_set_color(filtro,
			      parametros->m_rojo_sup_orden,
			      parametros->m_rojo_inf_orden,
			      parametros->m_verde_sup_orden,
			      parametros->m_verde_inf_orden,
			      parametros->m_azul_sup_orden,
			      parametros->m_azul_inf_orden,
			      parametros->m_rojo_sup_param,
			      parametros->m_rojo_inf_param,
			      parametros->m_verde_sup_param,
			      parametros->m_verde_inf_param,
			      parametros->m_azul_sup_param,
			      parametros->m_azul_inf_param);
      sprintf(dato->m_buffer_error,
	      "Colores: ORDEN [%i, %i, %i, %i, %i, %i], PARAM [%i, %i, %i, %i, %i, %i]",
	      parametros->m_rojo_sup_orden,
	      parametros->m_rojo_inf_orden,
	      parametros->m_verde_sup_orden,
	      parametros->m_verde_inf_orden,
	      parametros->m_azul_sup_orden,
	      parametros->m_azul_inf_orden,
	      parametros->m_rojo_sup_param,
	      parametros->m_rojo_inf_param,
	      parametros->m_verde_sup_param,
	      parametros->m_verde_inf_param,
	      parametros->m_azul_sup_param,
	      parametros->m_azul_inf_param);
      dato->m_error = dato->m_buffer_error;
      parametros->m_cambio = 0;
    }
    return dato->m_error;
  }
  else {
    return "fallo en el filtro";
  }
}
static char *filtro_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  dato->m_error = 0;
  dato->m_filtro = filtro_gestos_crear();
  filtro_t *filtro = dato->m_filtro;
  GHashTable *tabla = modulo->m_tabla;
  dato->m_imagen.m_imagen = 0;
  g_hash_table_insert(tabla, PUERTO_RED, filtro->m_salida);
  g_hash_table_insert(tabla, PUERTO_VENTANA, &dato->m_imagen);

  return "filtro iniciado";
}
static char *filtro_cerrar(modulo_t *modulo)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  filtro_gestos_borrar(&dato->m_filtro);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));  
  modulo->m_nombre = "Filtro";
  modulo->m_iniciar = filtro_iniciar;
  modulo->m_cerrar = filtro_cerrar;
  modulo->m_ciclo = filtro_ciclo;
  modulo->m_dato = (dato_filtro_t *)malloc(sizeof(dato_filtro_t));
  return modulo;

}
