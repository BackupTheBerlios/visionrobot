/*! \file imagenes.c
    \brief Genera imágenes para el filtro del pipeline.
    
           \section modulo Descripción del módulo
           Este módulo puede generar imágenes de varios tipos:
	   <ul>
	      <li>Reproducir un video.
	      <li>Imágenes aleatorias, creando puntos diseminados de diferentes colores por la imagen
	      <li>Una imagen plana de un color fijo
	      <li>Cargar un archivo de imagen
	      <li>A través de un interfaz de video, generar las imagenes
	   </ul>

	   \section puertos Puertos
	   El módulo no tiene puertos de entrada y disponde de dos puertos de salida:
	   <ul>
	      <li><em>salida_imagen</em>: Una estructura de tipo <code>filtro_gestos_in_imagen_t</code>, para un módulo que la acepte.
	   </ul>
	   Estas estructuras las crea el módulo de imágenes directamente, y también se encarga de destruirlas.

	   \section argumentos Argumentos
	   Los argumentos que soporta el módulo son:
	   <ul>
	     <li><em>video</em>: Si este atributo existe, se carga el video correspondiente.
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
#include <xine.h>
#include <xine/xineutils.h>


//! Tipo enumerado que define qué tipo de función va a realizar el módulo en una instancia determinada
typedef enum {CAMARA,		/*!< Si se usa el método de captura directa de cámara. */
	      IMAGEN,		/*!< Si se carga una imagen desde un archivo. */
	      ALEATORIO,	/*!< Si se generan imágenes aleatorias. */
	      COLOR,		/*!< Si se plasma un color definido en la pantalla. */
	      VIDEO		/*!< Si se reproduce un video. */
} tipo_imagen_t;

//! Estructura de datos para el video.
typedef struct {
  xine_t              *xine;
  xine_stream_t       *stream;
  xine_video_port_t   *vo_port;
  xine_audio_port_t   *ao_port;
  xine_event_queue_t  *event_queue;
  int running;
} video_t;

/*! \brief La estructura de datos que tiene cada módulo. */
typedef struct {
  filtro_gestos_in_imagen_t m_filtro; /*!< La imagen que enviaremos al módulo del filtro. Es lo que está en el puerto de salida. */
  unsigned char m_rojo;		/*!< La componenete roja del color primario que puede generar el módulo. */
  unsigned char m_verde;	/*!< La componenete verde del color primario que puede generar el módulo. */
  unsigned char m_azul;		/*!< La componenete azul del color primario que puede generar el módulo. */
  color_t *m_buffer_camara;	/*!< Un buffer en el que guardamos una línea de la imagen de la cámara. Lo usamos porque la imagen sale invertida. Se guarda la línea y luego se vuelca a la imagen que pasamos por el puerto de salida. */  
  int m_ancho_anterior;		/*!< El ancho del frame anterior, para la cámara. Si es diferente del actual, se cambia la imagen. De este modo controlamos el tamaño que tomamos. */
  GdkPixbuf *m_imagen_archivo;	/*!< Un GdkPixbuf que almacena la imagen que hemos abierto (si es que hemos abierto alguna). */
  char m_buffer_error[128];	/*!< El buffer donde guardamos los mensajes de error, para devolverlos. */
  tipo_imagen_t m_tipo;         /*!< El tipo de función que hemos elegido. */
  SANE_Handle m_handle;		/*!< El <em>handle</em> de la interfaz con SANE. */
  video_t m_video;
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
    xine_gui_send_vo_data(dato->m_video.stream, XINE_GUI_SEND_EXPOSE_EVENT, imagen->m_imagen);
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

//! Funcion de retrollamada para Xine.
/*! 
  
\param user_data Información de usuario.
\param event Evento que se le pasa.
*/
static void event_listener(void *user_data, const xine_event_t *event) {
  dato_imagenes_t *dato = (dato_imagenes_t *)user_data;
  switch(event->type) { 
  case XINE_EVENT_UI_PLAYBACK_FINISHED:
    dato->m_video.running = 0;
    break;
  }
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
    char *vo_driver    = "auto";
    char *ao_driver    = "auto";
    char configfile[2048];
    x11_visual_t vis;
    dato->m_video.xine = xine_new();
    sprintf(configfile, "%s%s", xine_get_homedir(), "/.xine/config");
    xine_config_load(dato->m_video.xine, configfile);
    xine_init(dato->m_video.xine);

    vis.display           = display;
    vis.screen            = screen;
    vis.d                 = window;
    vis.dest_size_cb      = dest_size_cb;
    vis.frame_output_cb   = frame_output_cb;
    vis.user_data         = NULL;
    pixel_aspect          = res_v / res_h;

    dato->m_video.running = 0;
    dato->m_video.vo_port = xine_open_video_driver(dato->m_video.xine, 
						   vo_driver, XINE_VISUAL_TYPE_X11, (void *) &vis);
    dato->m_video.ao_port     = xine_open_audio_driver(dato->m_video.xine , ao_driver, NULL);
    dato->m_video.stream      = xine_stream_new(dato->m_video.xine, dato->m_video.ao_port, dato->m_video.vo_port);
    dato->m_video.event_queue = xine_event_new_queue(dato->m_video.stream);
    xine_event_create_listener_thread(dato->m_video.event_queue, event_listener, dato);
    
    /*xine_gui_send_vo_data(dato->m_video.stream, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) window);
      xine_gui_send_vo_data(dato->m_video.stream, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void *) 1);*/

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
	b = imagen->m_imagen;
	int ancho_for = imagen->m_ancho * imagen->m_bytes;
	for(j = 0; j < imagen->m_alto; j++) {
	  aux = &p[(j + 1) * ancho_for];
	  for(i = 0; i < ancho_for; i += 1) {
	    *b++ = *--aux;
	  }
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
    xine_close(dato->m_video.stream);
    xine_event_dispose_queue(dato->m_video.event_queue);
    xine_dispose(dato->m_video.stream);
    if(dato->m_video.ao_port)
      xine_close_audio_driver(dato->m_video.xine, dato->m_video.ao_port);  
    xine_close_video_driver(dato->m_video.xine, dato->m_video.vo_port);  
    xine_exit(dato->m_video.xine);
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
