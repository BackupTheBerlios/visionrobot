#ifndef _FILTRO_GESTOS_SDK_H_
#define _FILTRO_GESTOS_SDK_H_

#define PIPELINE_FILTRO_GESTOS 0x01

#define PIPELINE_FILTRO_GESTOS_IMAGEN 0x0
#define PIPELINE_FILTRO_GESTOS_PARAMETROS 0x01

typedef unsigned char color_t;

typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} filtro_gestos_in_imagen_t;

typedef struct {
  int m_rojo;
} filtro_gestos_in_parametros_t;

typedef union {
  filtro_gestos_in_imagen_t imagen;
  filtro_gestos_in_parametros_t parametros;
} filtro_gestos_union_t;

typedef struct {
  int m_tipo;
  filtro_gestos_union_t m_dato;
} filtro_gestos_in_t;

#endif				//_FILTRO_GESTOS_SDK_H_
