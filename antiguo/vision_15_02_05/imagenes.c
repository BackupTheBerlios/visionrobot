/*! \file imagenes.c
    \brief Genera imágenes que sustituyen a las capturas de cámara
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include "imagenes_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>

static char *imagenes_generar_imagen(modulo_t *modulo) {
  int i, j;
  filtro_gestos_in_t* imagen = (filtro_gestos_in_t*)modulo->m_dato;
  for(i = 0; i < imagen->m_alto; i++) {
    for(j = 0; j < imagen->m_ancho * imagen->m_bytes; j += 1) {
      float numero = ((float)rand()) / ((float)RAND_MAX);
      imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j] = (int)(numero * 255);
    }
  }
}

static char *imagenes_ciclo(modulo_t* modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  imagenes_generar_imagen(modulo);
  out->m_tipo = PIPELINE_IMAGENES;
  filtro_gestos_in_t* imagen = (filtro_gestos_in_t*)modulo->m_dato;
  out->m_dato = imagen;
  return "saco imagen";
}

static char *imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{
  if(g_hash_table_size(argumentos) < 3) return "faltan parámetros";
  srand(time(0));
  filtro_gestos_in_t* imagen = (filtro_gestos_in_t*)modulo->m_dato;
  imagen->m_alto = atoi(g_hash_table_lookup(argumentos,"alto"));
  imagen->m_ancho = atoi(g_hash_table_lookup(argumentos,"ancho"));
  imagen->m_bytes = atoi(g_hash_table_lookup(argumentos,"bytes"));
  imagen->m_imagen =
    (char*)malloc(sizeof(char) *
		  imagen->m_alto *
		  imagen->m_ancho *
		  imagen->m_bytes);
  imagenes_generar_imagen(modulo);
  return "iniciado";
}

static char *imagenes_cerrar(modulo_t* modulo)
{
  filtro_gestos_in_t* imagen = (filtro_gestos_in_t*)modulo->m_dato;
  free(imagen->m_imagen);
  free(imagen);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Imágenes";
  modulo->m_iniciar = imagenes_iniciar;
  modulo->m_cerrar = imagenes_cerrar;
  modulo->m_ciclo = imagenes_ciclo;
  modulo->m_dato = (filtro_gestos_in_t*)malloc(sizeof(filtro_gestos_in_t));  
  return modulo;
}
