/*! \file imagenes.c
    \brief Genera imágenes para el filtro del pipeline.
    
           \section modulo Descripción del módulo
           Este módulo puede generar imágenes de varios tipos:
	   <ul>
	      <li>Imágenes aleatorias, creando puntos diseminados de diferentes colores por la imagen
	      <li>Una imagen plana de un color fijo
	      <li>Cargar un archivo de imagen
	   </ul>

	   \section puertos Puertos
	   El módulo no tiene puertos de entrada y disponde de dos puertos de salida:
	   <ul>
	      <li><em>salida_filtro</em>: Una estructura de tipo <code>filtro_gestos_in_imagen_t</code>, para llevarla directamente al filtro.
	      <li><em>salida_ventana</em>: Una estructura <code>filtro_gestos_in_parametros_t</code>, para llevarla a una ventana de imágenes y mostrarla directamente.
	   </ul>
	   Estas estructuras las crea el módulo de imágenes directamente, y también se encarga de destruirlas.

	   \section argumentos Argumentos
	   Los argumentos que soporta el módulo son:
	   <ul>
	     <li><em>camara</em>: Si este atributo existe y es igual a 1, se considera que se toma la imagen de un dispositivo externo.
	     <li><em>alto</em>: El alto de la imagen, obligatorio.
	     <li><em>ancho</em>: El ancho de la imagen, obligatorio.
	     <li><em>bytes</em>: El número de bytes por punto, obligatorio.
	     <li><em>archivo</em>: El archivo que se carga. Si no se especifica, se supone que se quiere generar una imagen de un color.
	     <li><em>rojo</em>: La componente roja de la imagen generada.
	     <li><em>verde</em>: La componente verde de la imagen generada. 
	     <li><em>azul</em>: La componente azul de la imagen generada. Si alguno de los tres colores anteriores no se especifica, se supone que se quiere generar una imagen de colores aleatorios.
	   </ul>
    \author Carlos León
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <sane/sane.h>
#include <unistd.h>

/*! \brief La estructura de datos que tiene cada módulo */
typedef struct {
  filtro_gestos_in_imagen_t m_filtro;
  ventana_imagen_in_t m_imagen;
  unsigned char m_rojo;
  unsigned char m_verde;
  unsigned char m_azul;
  int m_ancho_anterior;
  color_t *m_buffer_camara;
  GdkPixbuf *m_imagen_archivo;
  char m_buffer_error[128];
  enum {CAMARA, IMAGEN, ALEATORIO, COLOR} m_tipo;
  SANE_Handle m_handle;
} dato_imagenes_t;

/*! \brief El nombre del puerto de salida para el filtro de gestos */
#define PUERTO_FILTRO "salida_filtro"
/*! \brief El nombre del puerto de salida para la ventana de imágenes */
#define PUERTO_VENTANA "salida_ventana"


static char* imagenes_generar_imagen(modulo_t *modulo) {
  int i, j;
  char *devolver = 0;
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t*)&dato->m_filtro;
  int ancho_for = imagen->m_ancho * imagen->m_bytes;
  guchar * p;
  guchar *aux;
  guchar * b;
  SANE_Int len;
  SANE_Parameters parametros;
  int tam;
  switch(dato->m_tipo) {
  case CAMARA:    
    if(sane_start(dato->m_handle) != SANE_STATUS_GOOD) {
      devolver = "No se puede iniciar la captura";
    }
    else {
      if(sane_set_io_mode(dato->m_handle, SANE_FALSE) != SANE_STATUS_GOOD) {
	devolver = "No se puede establecer el modo de captura";
      }
      else {
	if(sane_get_parameters(dato->m_handle, &parametros) != SANE_STATUS_GOOD) {
	  devolver = "No se pueden coger parametros";
	}
	else {
	  if(dato->m_ancho_anterior != parametros.bytes_per_line) {
	    dato->m_ancho_anterior = parametros.bytes_per_line;
	    dato->m_buffer_camara = (color_t *)malloc(sizeof(color_t) * dato->m_ancho_anterior);
	  }
	  i = 0;
	  tam = imagen->m_ancho * imagen->m_bytes;
	  while(i < imagen->m_alto && sane_read(dato->m_handle, dato->m_buffer_camara
						, parametros.bytes_per_line, &len) != SANE_STATUS_EOF) {
	    for(j = tam; j >= 0; j -= imagen->m_bytes) {
	      int k;
	      for(k = 0; k < imagen->m_bytes; ++k) {
		imagen->m_imagen[j + k + tam * i] = dato->m_buffer_camara[tam - j + k];
	      }
	    }
	    i++;
	  }	  
	}
      }
    }
    break;
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
  return devolver;
}

static char *imagenes_ciclo(modulo_t* modulo, const char *puerto, const void *value){
  return imagenes_generar_imagen(modulo);
}

static char *imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{
  char *devolver = "iniciado";
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = &dato->m_filtro;
  dato->m_buffer_camara = 0;
  dato->m_imagen_archivo = 0;
  char *camara = (char*)g_hash_table_lookup(argumentos,"camara");
  int cam = 0;
  SANE_Int info;
  if(camara) cam = atoi(camara);
  if(cam) {
    dato->m_tipo = CAMARA;
    dato->m_ancho_anterior = -1;
    if(sane_init(0, 0) != SANE_STATUS_GOOD) {
      devolver = "No se puede iniciar SANE";
    }
    else {
      const SANE_Device **lista;
      sane_get_devices(&lista, SANE_TRUE);
      
      if (!lista[0]) {
	devolver = "No exite el dispositivo de cámara";
      }
      else {
	if(sane_open(lista[0]->name, &dato->m_handle) != SANE_STATUS_GOOD) {
	  devolver = "No se puede abrir el dispositivo";
	}
	else {
	  SANE_Int numero_opciones;
	  if(sane_control_option (dato->m_handle, 0, SANE_ACTION_GET_VALUE,
				  &numero_opciones, 0) != SANE_STATUS_GOOD) {
	    devolver = "No se pueden establecer las opciones del dispositivo";
	  }
	  else {
	    int i;
	    const SANE_Option_Descriptor *opt;
	    for(i = 0; i < numero_opciones; ++i) {
	      opt = sane_get_option_descriptor (dato->m_handle, i);
	      if(opt && opt->name && !strcmp(opt->name, "mode")) {
		sane_control_option (dato->m_handle, i, SANE_ACTION_SET_VALUE,
				     (void*)"Color", &info);
	      }	      
	    }
	  }
	}

      }
    }
  }
  else {
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
      char * rojo = (char*)g_hash_table_lookup(argumentos,"rojo");
      char * verde = (char*)g_hash_table_lookup(argumentos,"verde");
      char * azul = (char*)g_hash_table_lookup(argumentos,"azul");

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

  return devolver;
}

static char *imagenes_cerrar(modulo_t* modulo)
{
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  if(CAMARA == dato->m_tipo) {
    sane_close(dato->m_handle);
    sane_exit();
  }
  if(dato->m_buffer_camara) {
    free(dato->m_buffer_camara);
  }
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
