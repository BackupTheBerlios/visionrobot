#ifndef _VENTANA_IMAGEN_SDK_H_
#define _VENTANA_IMAGEN_SDK_H_

#define PIPELINE_VENTANA_IMAGEN 0x04

typedef struct {
  char * m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} ventana_imagen_in_t;

#endif
