#ifndef _IMAGENES_H_
#define _IMAGENES_H_

typedef enum {
	CAMARA = 0,
	VIDEO,
	IMAGEN_FIJA,
	ALEATORIO,
	COLOR_PLANO
};

typedef unsigned char color_t;

typedef struct {
  xine_t              *xine;
  xine_stream_t       *stream;
  xine_video_port_t   *vo_port;
  xine_audio_port_t   *ao_port;
  xine_event_queue_t  *event_queue;
  uint8_t *yuv;
} video_t;

typedef struct {
  color_t m_rojo;		/*!< La componenete roja del color primario que puede generar el módulo. */
  color_t m_verde;	/*!< La componenete verde del color primario que puede generar el módulo. */
  color_t m_azul;		/*!< La componenete azul del color primario que puede generar el módulo. */
} tres_colores_t;

typedef struct {
	int m_ancho_anterior;		/*!< El ancho del frame anterior, para la cámara. Si es diferente del actual, se cambia la imagen. De este modo controlamos el tamaño que tomamos. */
	color_t *m_buffer_camara;	/*!< Un buffer en el que guardamos una línea de la imagen de la cámara. Lo usamos porque la imagen sale invertida. Se guarda la línea y luego se vuelca a la imagen que pasamos por el puerto de salida. */  
	SANE_Handle m_handle;		/*!< El <em>handle</em> de la interfaz con SANE. */
} camara_t;

typedef union {
	video_t video;
	tres_colores_t colores;
	camara_t camara;	
	GdkPixbuf *m_imagen_archivo;	/*!< Un GdkPixbuf que almacena la imagen que hemos abierto (si es que hemos abierto alguna). */
} info_imagen_t;

typedef struct {
	frame_imagen_t m_frame;
	imagenes_fuente_t m_fuente;
	info_imagen_t m_info;
}imagenes_t;

typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} frame_imagen_t;

imagenes_t* imagenes_iniciar(imagenes_fuente_t fuente, int ancho, int alto, int bytes);

frame_imagen_t * imagenes_frame(imagenes_t * i);

void imagenes_cerrar(imagenes_t * i);

#endif

