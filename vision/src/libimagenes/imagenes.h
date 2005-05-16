#ifndef _IMAGENES_H_
#define _IMAGENES_H_

typedef enum {
  CAMARA = 0,
  VIDEO,
  IMAGEN_FIJA,
  ALEATORIO,
  COLOR_PLANO
}imagenes_fuente_t;

typedef unsigned char color_t;

typedef struct imagenes_s  imagenes_t;

typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} frame_imagen_t;

imagenes_t* imagenes_iniciar(imagenes_fuente_t fuente, int ancho, int alto, int bytes,
			     const char *archivo_imagen, int camara, const char *video, color_t r, color_t v, color_t a);

frame_imagen_t * imagenes_frame(imagenes_t * i);

void imagenes_cerrar(imagenes_t ** i);

#endif

