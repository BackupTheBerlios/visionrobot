/*! \file imagenes.c
    \brief Genera im�genes para el filtro del pipeline.
    
           \section modulo Descripci�n del m�dulo
           Este m�dulo puede generar im�genes de varios tipos:
	   <ul>
	      <li>Im�genes aleatorias, creando puntos diseminados de diferentes colores por la imagen
	      <li>Una imagen plana de un color fijo
	      <li>Cargar un archivo de imagen
	   </ul>

	   \section puertos Puertos
	   El m�dulo no tiene puertos de entrada y disponde de dos puertos de salida:
	   <ul>
	      <li><em>salida_filtro</em>: Una estructura de tipo <code>filtro_gestos_in_imagen_t</code>, para llevarla directamente al filtro.
	      <li><em>salida_ventana</em>: Una estructura <code>filtro_gestos_in_parametros_t</code>, para llevarla a una ventana de im�genes y mostrarla directamente.
	   </ul>
	   Estas estructuras las crea el m�dulo de im�genes directamente, y tambi�n se encarga de destruirlas.

	   \section argumentos Argumentos
	   Los argumentos que soporta el m�dulo son:
	   <ul>
	     <li><em>alto</em>: El alto de la imagen, obligatorio
	     <li><em>ancho</em>: El ancho de la imagen, obligatorio
	     <li><em>bytes</em>: El n�mero de bytes por punto, obligatorio
	     <li><em>archivo</em>: El archivo que se carga. Si no se especifica, se supone que se quiere generar una imagen de un color.
	     <li><em>rojo</em>: La componente roja de la imagen generada.
	     <li><em>verde</em>: La componente verde de la imagen generada. 
	     <li><em>azul</em>: La componente azul de la imagen generada. Si alguno de los tres colores anteriores no se especifica, se supone que se quiere generar una imagen de colores aleatorios.
	   </ul>
    \author Carlos Le�n
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*! \brief La estructura de datos que tiene cada m�dulo */
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

/*! \brief El nombre del puerto de salida para el filtro de gestos */
#define PUERTO_FILTRO "salida_filtro"
/*! \brief El nombre del puerto de salida para la ventana de im�genes */
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
  return 0;
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
    if(dato->m_imagen_archivo) {
      sprintf(dato->m_buffer_error, "Cargado %s", archivo_imagen);
    }
    else {
      sprintf(dato->m_buffer_error, "No se pudo cargar %s", archivo_imagen);
    }
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
