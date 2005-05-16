#include "filtro_gestos_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <sane/sane.h>
#include <unistd.h>

#include "imagenes.h"

//#if HAY_XINE == 1
#include <xine.h>
//! Estructura de datos para el video.


//#endif


//! Tipo enumerado que define qué tipo de función va a realizar el módulo en una instancia determinada
typedef enum {CAMARA,		/*!< Si se usa el método de captura directa de cámara. */
	      IMAGEN,		/*!< Si se carga una imagen desde un archivo. */
	      ALEATORIO,	/*!< Si se generan imágenes aleatorias. */
	      COLOR,		/*!< Si se plasma un color definido en la pantalla. */
	      VIDEO		/*!< Si se reproduce un video. */
} tipo_imagen_t;


/*! \brief La estructura de datos que tiene cada módulo. */
typedef struct {
  filtro_gestos_in_imagen_t m_filtro; /*!< La imagen que enviaremos al módulo del filtro. Es lo que está en el puerto de salida. */
  
  
  
  
  char m_buffer_error[128];	/*!< El buffer donde guardamos los mensajes de error, para devolverlos. */
  tipo_imagen_t m_tipo;         /*!< El tipo de función que hemos elegido. */

} dato_imagenes_t;

/*! \brief El nombre del puerto de salida de una imagen */
#define PUERTO_IMAGEN "salida_imagen"


/*! \brief Genera una imagen según los parámetros del módulo (tipo y demás), y la guarda en el buffer del módulo.
  
\param modulo Un puntero a un módulo, para obtener los datos.

\return Una cadena de error (que puede ser cero).
*/
static char* imagenes_generar_imagen(modulo_t *modulo) {
  int i, j;
  char *devolver = 0;
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t*)&dato->m_filtro;
  int ancho_for = imagen->m_ancho * imagen->m_bytes;
  SANE_Int len;
  SANE_Parameters parametros;
  int tam;
  int width, height, ratio_code, format;

  //#if HAY_XINE == 1
  uint8_t *im;   
  uint8_t *fin = &imagen->m_imagen[imagen->m_ancho * imagen->m_alto * imagen->m_bytes];
  uint8_t *yu;
  uint8_t y, u, v;
  //#endif
  
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
	    for(j = 0; j < tam; j += imagen->m_bytes) {
	      imagen->m_imagen[j + tam * i] = dato->m_buffer_camara[j + 2];
	      imagen->m_imagen[j + 1 + tam * i] = dato->m_buffer_camara[j + 1];
	      imagen->m_imagen[j + 2 + tam * i] = dato->m_buffer_camara[j];
	    }
	    i++;
	  }
	}
      }
    }
    break;
  case IMAGEN:break;
  case ALEATORIO:
    for(i = 0; i < imagen->m_alto; i++) {
      for(j = 0; j < ancho_for; j += 1) {
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j] =
	  (int)(g_random_double() * 255);
      }
    }
    break;
  case VIDEO:
    //#if HAY_XINE == 1
    xine_get_current_frame (dato->m_video.stream,			    
			    &width, &height,
			    &ratio_code, &format,
			    dato->m_video.yuv);
    switch(format) {
    case XINE_IMGFMT_YV12:
      yu = dato->m_video.yuv;
      im = imagen->m_imagen;
      while(im != fin) {
	/*y = ((*yu) & 0xF0) >> 4;
	u = ((*yu) & 0x0C) >> 2;
	v = ((*yu) & 0x03);*/
	/**im = y + 1.403 * v;
	*(im + 1) = y - 0.344 * u - 0.714 * v;
	*(im + 2) = y + 1.770 * u;*/
	/**im = y + 1.402 * (v - 128);
	*(im + 1) = y - 0.34414 * (u - 128) - 0.71414 * (v - 128);
	*(im + 2) = y + 1.772 * (u - 128);*/
	*im = *yu;
	*(im + 1) = *yu;
	*(im + 2) = *yu;
	im += 3; yu++;
      }
      break;
    case XINE_IMGFMT_YUY2:
      break;
    case XINE_IMGFMT_XVMC:
      break;
    case XINE_IMGFMT_XXMC:
      break;
    }
    //#endif
    devolver = 0;
    break;
  case COLOR:    
    break;
  }
  return devolver;
}

//! Realiza un ciclo de imágenes. El pipeline se encarga de llamar a esta función.
/*! 
  
\param modulo El módulo en el que se realiza en ciclo.
\param puerto El puerto por el que le llega la información (0 si no recibe nada).
\param value El valor que le llega a través del puerto (0 si no recibe nada).

\return Una cadena que representa un mensaje de respuesta, depuración, información o error. También puede ser 0.
*/
static char *imagenes_ciclo(modulo_t* modulo, const char *puerto, const void *value){
  return imagenes_generar_imagen(modulo);
}
//! Función que llama el pipeline y sirve para iniciar el interfaz de imágnenes.
/*!  Crea la memoria necesaria y ajusta los parámetros pertinenetes del módulo, según esté especificado en el XML.
  
\param modulo El módulo que se inicia.
\param argumentos Una <code>GHashTable</code> de GLib, que contiene los argumentos en forma de "nombre de argumento" -> valor.

\return Una cadena que representa un mensaje de respuesta, depuración, información o error. También puede ser 0.
*/
static char *imagenes_iniciar(modulo_t* modulo, GHashTable *argumentos)
{

  char *devolver = "iniciado";
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  filtro_gestos_in_imagen_t* imagen = &dato->m_filtro;
  dato->m_buffer_camara = 0;
  dato->m_ancho_anterior = -1;
  dato->m_imagen_archivo = 0;
  char *video = (char*)g_hash_table_lookup(argumentos,"video");
  char *camara = (char*)g_hash_table_lookup(argumentos,"camara");
  imagen->m_alto = atoi(g_hash_table_lookup(argumentos,"alto"));
  imagen->m_ancho = atoi(g_hash_table_lookup(argumentos,"ancho"));
  imagen->m_bytes = atoi(g_hash_table_lookup(argumentos,"bytes"));
  imagen->m_imagen =
    (char*)malloc(sizeof(char) *
		  imagen->m_alto *
		  imagen->m_ancho *
		  imagen->m_bytes);
  int cam = 0;
  SANE_Int info;
  if(camara) cam = atoi(camara);
  if(video) {
    //#if HAY_XINE == 1
    dato->m_tipo = VIDEO;    
    char *vo_driver    = "auto";
    char *ao_driver    = "auto";
    char configfile[2048];
    dato->m_video.xine = xine_new();
    sprintf(configfile, "%s%s", xine_get_homedir(), "/.xine/config");
    xine_config_load(dato->m_video.xine, configfile);
    xine_init(dato->m_video.xine);
    /* WARNING: Aquí hay un 2 por el esquema YUV 4:2:2 si no me equivoco: (4+2+2) * 2 = 16 bits, "2" bytes */
    dato->m_video.yuv = (uint8_t*)malloc(sizeof(uint8_t) * imagen->m_alto * imagen->m_ancho * 2);
    dato->m_video.vo_port = xine_open_video_driver(dato->m_video.xine, 
						   vo_driver, XINE_VISUAL_TYPE_NONE, 0);
    dato->m_video.ao_port     = xine_open_audio_driver(dato->m_video.xine , ao_driver, NULL);
    dato->m_video.stream      = xine_stream_new(dato->m_video.xine, dato->m_video.ao_port, dato->m_video.vo_port);
    dato->m_video.event_queue = xine_event_new_queue(dato->m_video.stream);
    xine_open(dato->m_video.stream, video);
    xine_play(dato->m_video.stream, 0, 0);
    //#endif
  }
  else if(cam) {
    dato->m_tipo = CAMARA;
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
	guchar * p;
	guchar *aux;
	guchar * b;
	int j, i;
	sprintf(dato->m_buffer_error, "Cargado %s", archivo_imagen);
	p = gdk_pixbuf_get_pixels (dato->m_imagen_archivo);
	guchar *fin = &p[imagen->m_ancho *
			 imagen->m_alto *
			 imagen->m_bytes];
	b = imagen->m_imagen;
	while(p != fin) {
	  *b++ = *p++;
	}
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
	guchar *fin = &imagen->m_imagen[imagen->m_ancho *
			 imagen->m_alto *
			 imagen->m_bytes];
	guchar *aux = imagen->m_imagen;
	while(aux != fin) {
	  *aux = dato->m_azul;
	  *(aux + 1) = dato->m_verde;
	  *(aux + 2) = dato->m_rojo;
	  aux += 3;
	}
      }
      else {
	dato->m_tipo = ALEATORIO;
      }
    }
  }
    
  
  g_hash_table_insert(modulo->m_tabla, PUERTO_IMAGEN, imagen);

  return devolver;
}

//! Cierra un módulo de imágenes
/*! Libera toda la memoria creada por el módulo, y el mismo módulo.
  
\param modulo El módulo que cerramos

\return Una cadena que representa un mensaje de respuesta, depuración, información o error. También puede ser 0.
*/
static char *imagenes_cerrar(modulo_t* modulo)
{
  dato_imagenes_t *dato = (dato_imagenes_t*)modulo->m_dato;
  if(CAMARA == dato->m_tipo) {
    sane_close(dato->m_handle);
    sane_exit();
  }
  else if(VIDEO == dato->m_tipo) {
    //#if HAY_XINE == 1
    xine_close(dato->m_video.stream);
    xine_event_dispose_queue(dato->m_video.event_queue);
    xine_dispose(dato->m_video.stream);
    if(dato->m_video.ao_port)
      xine_close_audio_driver(dato->m_video.xine, dato->m_video.ao_port);  
    xine_close_video_driver(dato->m_video.xine, dato->m_video.vo_port);  
    xine_exit(dato->m_video.xine);
    free(dato->m_video.yuv);
    //#endif
  }
  free(dato->m_buffer_camara);
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
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


imagenes_t* imagenes_iniciar(imagenes_fuente_t fuente, int ancho, int alto, int bytes) {

	imagenes_t * imagenes = (imagenes_t *)malloc(sizeof(imagenes_t));	
	imagenes->m_fuente = fuente;
	imagenes->m_frame.m_alto = alto;
	imagenes->m_frame.m_ancho = ancho;
	imagenes->m_frame.m_bytes = bytes;

  imagen->m_imagen =
    (char*)malloc(sizeof(char) *
		  imagen->m_alto *
		  imagen->m_ancho *
		  imagen->m_bytes);
  int cam = 0;
  SANE_Int info;
  if(camara) cam = atoi(camara);
  if(video) {
    //#if HAY_XINE == 1
    dato->m_tipo = VIDEO;    
    char *vo_driver    = "auto";
    char *ao_driver    = "auto";
    char configfile[2048];
    dato->m_video.xine = xine_new();
    sprintf(configfile, "%s%s", xine_get_homedir(), "/.xine/config");
    xine_config_load(dato->m_video.xine, configfile);
    xine_init(dato->m_video.xine);
    /* WARNING: Aquí hay un 2 por el esquema YUV 4:2:2 si no me equivoco: (4+2+2) * 2 = 16 bits, "2" bytes */
    dato->m_video.yuv = (uint8_t*)malloc(sizeof(uint8_t) * imagen->m_alto * imagen->m_ancho * 2);
    dato->m_video.vo_port = xine_open_video_driver(dato->m_video.xine, 
						   vo_driver, XINE_VISUAL_TYPE_NONE, 0);
    dato->m_video.ao_port     = xine_open_audio_driver(dato->m_video.xine , ao_driver, NULL);
    dato->m_video.stream      = xine_stream_new(dato->m_video.xine, dato->m_video.ao_port, dato->m_video.vo_port);
    dato->m_video.event_queue = xine_event_new_queue(dato->m_video.stream);
    xine_open(dato->m_video.stream, video);
    xine_play(dato->m_video.stream, 0, 0);
    //#endif
  }
  else if(cam) {
    dato->m_tipo = CAMARA;
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
	guchar * p;
	guchar *aux;
	guchar * b;
	int j, i;
	sprintf(dato->m_buffer_error, "Cargado %s", archivo_imagen);
	p = gdk_pixbuf_get_pixels (dato->m_imagen_archivo);
	guchar *fin = &p[imagen->m_ancho *
			 imagen->m_alto *
			 imagen->m_bytes];
	b = imagen->m_imagen;
	while(p != fin) {
	  *b++ = *p++;
	}
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
	guchar *fin = &imagen->m_imagen[imagen->m_ancho *
			 imagen->m_alto *
			 imagen->m_bytes];
	guchar *aux = imagen->m_imagen;
	while(aux != fin) {
	  *aux = dato->m_azul;
	  *(aux + 1) = dato->m_verde;
	  *(aux + 2) = dato->m_rojo;
	  aux += 3;
	}
      }
      else {
	dato->m_tipo = ALEATORIO;
      }
    }
  }
    
  
  g_hash_table_insert(modulo->m_tabla, PUERTO_IMAGEN, imagen);

  return devolver;
}

frame_imagen_t * imagenes_frame(imagenes_t * i) {
}

void imagenes_cerrar(imagenes_t * i) {
}
