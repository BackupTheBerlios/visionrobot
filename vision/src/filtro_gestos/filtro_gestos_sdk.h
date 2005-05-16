#ifndef _FILTRO_GESTOS_SDK_H_
#define _FILTRO_GESTOS_SDK_H_

/*
typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} filtro_gestos_in_imagen_t;*/
typedef frame_imagen_t filtro_gestos_in_imagen_t;

typedef struct {
  color_t m_rojo_sup;
  color_t m_rojo_inf;
  color_t m_verde_sup;
  color_t m_verde_inf; 
  color_t m_azul_sup; 
  color_t m_azul_inf;
} filtro_gestos_in_parametros_t;

#endif
