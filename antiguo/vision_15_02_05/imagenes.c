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


typedef struct {
  filtro_gestos_in_t m_filtro;
  ventana_imagen_in_t m_imagen;
  unsigned char m_rojo;
  unsigned char m_verde;
  unsigned char m_azul;
  char m_aleatorio;
} dato_imagenes_t;


static void imagenes_generar_imagen(modulo_t *modulo) {
  int i, j;
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_t* imagen = (filtro_gestos_in_t*)&dato->m_filtro;

  if(dato->m_aleatorio) {
    for(i = 0; i < imagen->m_dato.imagen.m_alto; i++) {
      for(j = 0; j < imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes; j += 1) {
	imagen->m_dato.imagen.m_imagen[i * imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes + j] =
	  (int)(g_random_double() * 255);
      }
    }
  }
  else {
    for(i = 0; i < imagen->m_dato.imagen.m_alto; i++) {
      for(j = 0; j < imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes; j += 3) {
	imagen->m_dato.imagen.m_imagen[i * imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes + j] = dato->m_azul;
	imagen->m_dato.imagen.m_imagen[i * imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes + j + 1] = dato->m_verde;
	imagen->m_dato.imagen.m_imagen[i * imagen->m_dato.imagen.m_ancho * imagen->m_dato.imagen.m_bytes + j + 2] = dato->m_rojo;
      }
    }
  }
}

static char *imagenes_ciclo(modulo_t* modulo, char tipo, GHashTable *lista)
{
  imagenes_generar_imagen(modulo);
  return "saco imagen";
}

static char *imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{
  if(g_hash_table_size(argumentos) < 3) return "faltan parametros";
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_t* imagen = &dato->m_filtro;

  char * rojo = g_hash_table_lookup(argumentos,"rojo");
  char * verde = g_hash_table_lookup(argumentos,"verde");
  char * azul = g_hash_table_lookup(argumentos,"azul");
  if(rojo && verde && azul) {
    dato->m_rojo = (unsigned char)atoi(rojo);
    dato->m_verde = (unsigned char)atoi(verde);
    dato->m_azul = (unsigned char)atoi(azul);
    dato->m_aleatorio = 0;
  }
  else {
    dato->m_aleatorio = 1;
  }
    
  imagen->m_tipo = PIPELINE_FILTRO_GESTOS_IMAGEN;
  imagen->m_dato.imagen.m_alto = atoi(g_hash_table_lookup(argumentos,"alto"));
  imagen->m_dato.imagen.m_ancho = atoi(g_hash_table_lookup(argumentos,"ancho"));
  imagen->m_dato.imagen.m_bytes = atoi(g_hash_table_lookup(argumentos,"bytes"));
  imagen->m_dato.imagen.m_imagen =
    (char*)malloc(sizeof(char) *
		  imagen->m_dato.imagen.m_alto *
		  imagen->m_dato.imagen.m_ancho *
		  imagen->m_dato.imagen.m_bytes);
  
  dato->m_imagen.m_imagen = imagen->m_dato.imagen.m_imagen;
  dato->m_imagen.m_alto = imagen->m_dato.imagen.m_alto;
  dato->m_imagen.m_ancho = imagen->m_dato.imagen.m_ancho;
  dato->m_imagen.m_bytes = imagen->m_dato.imagen.m_bytes;
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_VENTANA_IMAGEN), &dato->m_imagen);
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_FILTRO_GESTOS), imagen);

  imagenes_generar_imagen(modulo);
  return "iniciado";
}

static char *imagenes_cerrar(modulo_t* modulo)
{
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_t* imagen = &dato->m_filtro;
  free(imagen->m_dato.imagen.m_imagen);
  //  free(imagen);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_tipo = PIPELINE_IMAGENES;
  modulo->m_nombre = "Imagenes";
  modulo->m_iniciar = imagenes_iniciar;
  modulo->m_cerrar = imagenes_cerrar;
  modulo->m_ciclo = imagenes_ciclo;
  modulo->m_dato = (dato_imagenes_t*)malloc(sizeof(dato_imagenes_t));  
  //  ((dato_imagenes_t*)modulo->m_dato)->m_filtro = (filtro_gestos_in_t*)malloc(sizeof(filtro_gestos_in_t));  
  return modulo;
}
