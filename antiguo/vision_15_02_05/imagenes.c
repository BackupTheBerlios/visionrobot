/*! \file imagenes.c
    \brief Genera imágenes que sustituyen a las capturas de cámara
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct {
  filtro_gestos_in_imagen_t m_filtro;
  ventana_imagen_in_t m_imagen;
  unsigned char m_rojo;
  unsigned char m_verde;
  unsigned char m_azul;
  GdkPixbuf *m_imagen_archivo;
  char m_buffer_error[128];
  enum {IMAGEN, ALEATORIO, COLOR} m_tipo;
} dato_imagenes_t;

#define PUERTO_FILTRO "salida_filtro"
#define PUERTO_VENTANA "salida_ventana"


static void imagenes_generar_imagen(modulo_t *modulo) {
  int i, j;
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t*)&dato->m_filtro;
  int ancho_for = imagen->m_ancho * imagen->m_bytes;
  guchar * p;
  guchar *aux;
  guchar * b;

  switch(dato->m_tipo) {
  case IMAGEN:
    p = gdk_pixbuf_get_pixels (dato->m_imagen_archivo);
    b = imagen->m_imagen;
    for(j = 0; j < imagen->m_alto; j++) {
      aux = &p[(j + 1) * ancho_for];
      for(i = 0; i < ancho_for; i += 1) {
	*b++ = *--aux;
      }
    }    
    break;
  case ALEATORIO:
    for(i = 0; i < imagen->m_alto; i++) {
      for(j = 0; j < ancho_for; j += 1) {
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j] =
	  (int)(g_random_double() * 255);
      }
    }
    break;
  case COLOR:
    for(i = 0; i < imagen->m_alto; i++) {
      for(j = 0; j < ancho_for; j += 3) {
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j] = dato->m_azul;
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j + 1] = dato->m_verde;
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j + 2] = dato->m_rojo;
      }
    }
    break;
  }
}

static char *imagenes_ciclo(modulo_t* modulo, const char *puerto, const void *value){
  imagenes_generar_imagen(modulo);
  return 0;//"saco imagen";
}

static char *imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{
  char *devolver = "iniciado";
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = &dato->m_filtro;
  dato->m_imagen_archivo = 0;
  char * rojo = g_hash_table_lookup(argumentos,"rojo");
  char * verde = g_hash_table_lookup(argumentos,"verde");
  char * azul = g_hash_table_lookup(argumentos,"azul");
  char * archivo_imagen = g_hash_table_lookup(argumentos,"archivo");
  if(archivo_imagen) {
    dato->m_tipo = IMAGEN;
    dato->m_imagen_archivo = gdk_pixbuf_new_from_file(archivo_imagen, 0);    
    sprintf(dato->m_buffer_error, "Cargado %s", archivo_imagen);
    devolver = dato->m_buffer_error;
  }
  if(!dato->m_imagen_archivo) {
    if(rojo && verde && azul) {
      dato->m_rojo = (unsigned char)atoi(rojo);
      dato->m_verde = (unsigned char)atoi(verde);
      dato->m_azul = (unsigned char)atoi(azul);
      dato->m_tipo = COLOR;
    }
    else {
      dato->m_tipo = ALEATORIO;
    }
  }
    
  imagen->m_alto = atoi(g_hash_table_lookup(argumentos,"alto"));
  imagen->m_ancho = atoi(g_hash_table_lookup(argumentos,"ancho"));
  imagen->m_bytes = atoi(g_hash_table_lookup(argumentos,"bytes"));
  imagen->m_imagen =
    (char*)malloc(sizeof(char) *
		  imagen->m_alto *
		  imagen->m_ancho *
		  imagen->m_bytes);
  
  dato->m_imagen.m_imagen = imagen->m_imagen;
  dato->m_imagen.m_alto = imagen->m_alto;
  dato->m_imagen.m_ancho = imagen->m_ancho;
  dato->m_imagen.m_bytes = imagen->m_bytes;
  g_hash_table_insert(modulo->m_tabla, PUERTO_VENTANA, &dato->m_imagen);
  g_hash_table_insert(modulo->m_tabla, PUERTO_FILTRO, imagen);

  imagenes_generar_imagen(modulo);
  return devolver;
}

static char *imagenes_cerrar(modulo_t* modulo)
{
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = &dato->m_filtro;
  free(imagen->m_imagen);
  if(dato->m_imagen_archivo) {
    gdk_pixbuf_unref(dato->m_imagen_archivo);
  }
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Imagenes";
  modulo->m_iniciar = imagenes_iniciar;
  modulo->m_cerrar = imagenes_cerrar;
  modulo->m_ciclo = imagenes_ciclo;
  modulo->m_dato = (dato_imagenes_t*)malloc(sizeof(dato_imagenes_t));  
  return modulo;
}
