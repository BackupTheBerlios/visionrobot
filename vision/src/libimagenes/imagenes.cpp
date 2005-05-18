#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include "Captura.h"
#include "imagenes.h"

#ifndef G_OS_WIN32
#include <xine.h>
#include <sane/sane.h>
#endif

#include <gdk-pixbuf/gdk-pixbuf.h>

#ifndef G_OS_WIN32
typedef struct {
  xine_t              *xine;
  xine_stream_t       *stream;
  xine_video_port_t   *vo_port;
  xine_audio_port_t   *ao_port;
  xine_event_queue_t  *event_queue;
  uint8_t *yuv;
} video_t;
#endif

typedef struct {
#ifndef G_OS_WIN32
  int m_ancho_anterior;       /*!< El ancho del frame anterior, para la cámara. Si es diferente del actual, se cambia la imagen. De este modo controlamos el tamaño que tomamos. */
  color_t *m_buffer_camara;   /*!< Un buffer en el que guardamos una línea de la imagen de la cámara. Lo usamos porque la imagen sale invertida. Se guarda la línea y luego se vuelca a la imagen que pasamos por el puerto de salida. */  
  SANE_Handle m_handle;       /*!< El <em>handle</em> de la interfaz con SANE. */
#else
	Captura* m_captura;
#endif
} camara_t;

typedef union {
#ifndef G_OS_WIN32
  video_t video;
#endif
  camara_t camara;    
} info_imagen_t;

struct imagenes_s{
  frame_imagen_t m_frame;
  imagenes_fuente_t m_fuente;
  info_imagen_t m_info;
};

imagenes_t* imagenes_iniciar(imagenes_fuente_t fuente, int ancho, int alto, int bytes,
			     const char *archivo_imagen, int cam, const char *video, color_t r, color_t v, color_t a) {
  guchar * p;
#ifndef G_OS_WIN32
  char configfile[2048];
#endif
  camara_t * dato_camara = 0;
  guchar *aux;
  guchar *fin;
  GdkPixbuf * imagen_archivo = 0;
#ifndef G_OS_WIN32
  video_t *dato_video = 0;
#endif
  char *vo_driver    = "auto";
  char *ao_driver    = "auto";      
  guchar * b;
  imagenes_t * imagenes = (imagenes_t *)malloc(sizeof(imagenes_t));	
  imagenes->m_fuente = fuente;
  imagenes->m_frame.m_alto = alto;
  imagenes->m_frame.m_ancho = ancho;
  imagenes->m_frame.m_bytes = bytes;

#ifdef G_OS_WIN32
  if(imagenes->m_fuente != CAMARA) {
#endif
  imagenes->m_frame.m_imagen =
    (color_t*)malloc(sizeof(color_t) *
		  imagenes->m_frame.m_alto *
		  imagenes->m_frame.m_ancho *
		  imagenes->m_frame.m_bytes);
#ifdef G_OS_WIN32
  }
#endif

#ifndef G_OS_WIN32
  SANE_Int info;
#endif
  
  switch (imagenes->m_fuente) {
  case CAMARA:
#ifndef G_OS_WIN32
    dato_camara = &imagenes->m_info.camara;
    imagenes->m_info.camara.m_buffer_camara = 0;
    imagenes->m_info.camara.m_ancho_anterior = -1;
    if(sane_init(0, 0) != SANE_STATUS_GOOD) {
      return 0;//devolver = "No se puede iniciar SANE";
    }
    else {
      const SANE_Device **lista;
      sane_get_devices(&lista, SANE_TRUE);
      
      if (!lista[0]) {
	return 0;//devolver = "No exite el dispositivo de cámara";
      }
      else {
	if(sane_open(lista[0]->name, &dato_camara->m_handle) != SANE_STATUS_GOOD) {
	  return 0;//devolver = "No se puede abrir el dispositivo";
	}
	else {
	  SANE_Int numero_opciones;
	  if(sane_control_option (dato_camara->m_handle, 0, SANE_ACTION_GET_VALUE,
				  &numero_opciones, 0) != SANE_STATUS_GOOD) {
	    return 0;//devolver = "No se pueden establecer las opciones del dispositivo";
	  }
	  else {
	    int i;
	    const SANE_Option_Descriptor *opt;
	    for(i = 0; i < numero_opciones; ++i) {
	      opt = sane_get_option_descriptor (dato_camara->m_handle, i);
	      if(opt && opt->name && !strcmp(opt->name, "mode")) {
		sane_control_option (dato_camara->m_handle, i, SANE_ACTION_SET_VALUE,
				     (void*)"Color", &info);
	      }
	    }
	  }
	}
	
      }
    }
#else
	imagenes->m_info.camara.m_captura = new Captura(); 
	imagenes->m_info.camara.m_captura->Iniciar(cam,
		   0, imagenes->m_frame.m_ancho,
		   imagenes->m_frame.m_alto); 
#endif
    break;
  case VIDEO:
#ifndef G_OS_WIN32
    dato_video = & imagenes->m_info.video;
    dato_video->xine = xine_new();
    sprintf(configfile, "%s%s", xine_get_homedir(), "/.xine/config");
    xine_config_load(dato_video->xine, configfile);
    xine_init(dato_video->xine);
    /* WARNING: Aquí hay un 2 por el esquema YUV 4:2:2 si no me equivoco: (4+2+2) * 2 = 16 bits, "2" bytes */
    dato_video->yuv = (uint8_t*)malloc(sizeof(uint8_t) * imagenes->m_frame.m_alto * imagenes->m_frame.m_ancho * 2);
    dato_video->vo_port = xine_open_video_driver(dato_video->xine, 
						 vo_driver, XINE_VISUAL_TYPE_NONE, 0);
    dato_video->ao_port     = xine_open_audio_driver(dato_video->xine , ao_driver, NULL);
    dato_video->stream      = xine_stream_new(dato_video->xine, dato_video->ao_port, dato_video->vo_port);
    dato_video->event_queue = xine_event_new_queue(dato_video->stream);
    xine_open(dato_video->stream, video);
    xine_play(dato_video->stream, 0, 0);
#endif
    break;
  case IMAGEN_FIJA:
    if(archivo_imagen) {        
      imagen_archivo = gdk_pixbuf_new_from_file(archivo_imagen, 0);    
      if(imagen_archivo) {
    	
	p = gdk_pixbuf_get_pixels (imagen_archivo);
	fin = &p[ imagenes->m_frame.m_ancho *
			  imagenes->m_frame.m_alto *
			  imagenes->m_frame.m_bytes];
	b = imagenes->m_frame.m_imagen;
	while(p != fin) {
	  *b++ = *p++;
	}
      }
    }
    break;
  case COLOR_PLANO:
    fin = & imagenes->m_frame.m_imagen[imagenes->m_frame.m_ancho *
				       imagenes->m_frame.m_alto *
				       imagenes->m_frame.m_bytes];
    aux = imagenes->m_frame.m_imagen;
    while(aux != fin) {
      *aux = r;
      *(aux + 1) = v;
      *(aux + 2) = a;
      aux += 3;
    }
    break;
  }

  return imagenes;   
}

frame_imagen_t * imagenes_frame(imagenes_t * imagenes) {
  int i, j;
  int tam;
  //char *devolver = 0;
  
  frame_imagen_t* imagen = (frame_imagen_t*)&imagenes->m_frame;
  
  int ancho_for = imagen->m_ancho * imagen->m_bytes;
#ifndef G_OS_WIN32
  SANE_Int len;
  SANE_Parameters parametros;
  int tam;
  int width, height, ratio_code, format;
#endif
  camara_t * dato_camara = &imagenes->m_info.camara;
#ifndef G_OS_WIN32
  video_t * dato_video = &imagenes->m_info.video;
  //#if HAY_XINE == 1
  uint8_t *im;   
  uint8_t *fin = &imagen->m_imagen[imagen->m_ancho * imagen->m_alto * imagen->m_bytes];
  uint8_t *yu;
  uint8_t y, u, v;
#endif
  //#endif
  switch (imagenes->m_fuente) {
  case CAMARA:
#ifndef G_OS_WIN32
    if(sane_start(dato_camara->m_handle) != SANE_STATUS_GOOD) {
      return 0;
      //devolver = "No se puede iniciar la captura";
    }
    else {
      if(sane_set_io_mode(dato_camara->m_handle, SANE_FALSE) != SANE_STATUS_GOOD) {
	return 0;
	// devolver = "No se puede establecer el modo de captura";
      }
      else {
	if(sane_get_parameters(dato_camara->m_handle, &parametros) != SANE_STATUS_GOOD) {
	  return 0;
	  //devolver = "No se pueden coger parametros";
	}
	else {
	  if(dato_camara->m_ancho_anterior != parametros.bytes_per_line) {
	    dato_camara->m_ancho_anterior = parametros.bytes_per_line;
	    dato_camara->m_buffer_camara = (color_t *)malloc(sizeof(color_t) * dato_camara->m_ancho_anterior);
	  }
	  i = 0;
	  tam = imagen->m_ancho * imagen->m_bytes;
	  while(i < imagen->m_alto && sane_read(dato_camara->m_handle, dato_camara->m_buffer_camara
						, parametros.bytes_per_line, &len) != SANE_STATUS_EOF) {
	    for(j = 0; j < tam; j += imagen->m_bytes) {
	      imagen->m_imagen[j + tam * i] = dato_camara->m_buffer_camara[j + 2];
	      imagen->m_imagen[j + 1 + tam * i] = dato_camara->m_buffer_camara[j + 1];
	      imagen->m_imagen[j + 2 + tam * i] = dato_camara->m_buffer_camara[j];
	    }
	    i++;
	  }
	}
      }
    }
#else
	imagenes->m_info.camara.m_captura->CogerFrame();
	tam = imagenes->m_info.camara.m_captura->GetFrame((BYTE**)&imagenes->m_frame.m_imagen);
  imagenes->m_frame.m_bytes = (tam / imagenes->m_frame.m_alto) / imagenes->m_frame.m_ancho;
#endif
    break;
  case VIDEO:
#ifndef G_OS_WIN32
    xine_get_current_frame (dato_video->stream,               
			    &width, &height,
			    &ratio_code, &format,
			    dato_video->yuv);
    switch(format) {
    case XINE_IMGFMT_YV12:
      yu = dato_video->yuv;
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
#endif
    break;
  case ALEATORIO:
    for(i = 0; i < imagen->m_alto; i++) {
      for(j = 0; j < ancho_for; j += 1) {
	imagen->m_imagen[i * imagen->m_ancho * imagen->m_bytes + j] =
	  (int)(g_random_double() * 255);
      }
    }
    break;
  }
  return &imagenes->m_frame;
}

void imagenes_cerrar(imagenes_t ** imagenes) {
#ifndef G_OS_WIN32
  video_t * dato;
#endif
  switch ((*imagenes)->m_fuente) {
  case CAMARA:
#ifndef G_OS_WIN32
    sane_close((*imagenes)->m_info.camara.m_handle);
    sane_exit();
#else 
	if((*imagenes)->m_info.camara.m_captura) delete (*imagenes)->m_info.camara.m_captura;
  (*imagenes)->m_info.camara.m_captura = 0; 
#endif
    break;
  case VIDEO:
#ifndef G_OS_WIN32
    dato = &((*imagenes)->m_info.video);
    xine_close(dato->stream);
    xine_event_dispose_queue(dato->event_queue);
    xine_dispose(dato->stream);
    if(dato->ao_port) {
      xine_close_audio_driver(dato->xine, dato->ao_port);  
    }
    xine_close_video_driver(dato->xine, dato->vo_port);  
    xine_exit(dato->xine);
    free(dato->yuv);
#endif
    break;        
  }

  #ifdef G_OS_WIN32
  if((*imagenes)->m_fuente != CAMARA) {
#endif
	  free((*imagenes)->m_frame.m_imagen);
#ifdef G_OS_WIN32
  }
#endif

  
  free(*imagenes);
  *imagenes = 0;
}
