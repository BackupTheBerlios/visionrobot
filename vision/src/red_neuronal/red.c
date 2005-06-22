/*!
  \file   red.c
  \author Carlos León
  \date   Sun Mar 13 22:12:21 2005
  
  \brief  Módulo de red neuronal.
  
  
*/
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
  red_neuronal_t *m_red;
} red_dato_t;

static char *red_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  if(!strcmp(puerto, PUERTO_IMAGEN)) {
    red_dato_t * dato = (red_dato_t*)modulo->m_dato;
    red_neuronal_t *red = dato->m_red;
    red_neuronal_in_t * red_in = (red_neuronal_in_t *)value;
    if(red && red_in) {
      sprintf(dato->m_salida, "%s", 
	      red_neuronal_reconocer(red, red_in->m_imagen,
				     red_in->m_ancho, red_in->m_alto,
				     red_in->m_bytes));
	  printf("La red ha reconocido %s.\n", dato->m_salida);
      g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, &dato->m_salida);
    }
    else {
      g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
    }
  }
  return 0;
}
static char *red_iniciar(modulo_t *modulo, GHashTable *argumentos)
{  
  red_neuronal_t *red; 
  const char *no_gesto; 
  red_dato_t * dato = (red_dato_t*)modulo->m_dato;
  char *archivo = g_hash_table_lookup(argumentos, "archivo");
  const char *salida[4];
  if (g_hash_table_size(argumentos) < 2) {
    return "falta algun nombre de archivo para cargar";
  }
  salida[0] = g_hash_table_lookup(argumentos, "salida1");
  salida[1] = g_hash_table_lookup(argumentos, "salida2");
  salida[2] = g_hash_table_lookup(argumentos, "salida3");
  salida[3] = g_hash_table_lookup(argumentos, "salida4");
  no_gesto = g_hash_table_lookup(argumentos, "no_gesto");
  dato->m_red = red_neuronal_abrir(archivo, salida, no_gesto);
  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
  red = dato->m_red;
    if (!red) {
      return "no se ha cargado el archivo";
    }    
    else {
	sprintf(dato->m_buffer_error, "archivo cargado = %s", archivo);
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
  modulo->m_iniciar = red_iniciar;
  modulo->m_cerrar = red_cerrar;
  modulo->m_ciclo = red_ciclo;
  modulo->m_dato = (red_dato_t *)malloc(sizeof(red_dato_t));
  return modulo;
}

