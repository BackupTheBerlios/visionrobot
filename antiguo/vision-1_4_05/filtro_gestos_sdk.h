#ifndef _FILTRO_GESTOS_SDK_H_
#define _FILTRO_GESTOS_SDK_H_

typedef unsigned char color_t;

typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} filtro_gestos_in_imagen_t;

typedef struct {
  char m_cambio;
  color_t m_rojo_sup_orden;
  color_t m_rojo_inf_orden;
  color_t m_verde_sup_orden;
  color_t m_verde_inf_orden; 
  color_t m_azul_sup_orden; 
  color_t m_azul_inf_orden;
  color_t m_rojo_sup_param;
  color_t m_rojo_inf_param;
  color_t m_verde_sup_param;
  color_t m_verde_inf_param; 
  color_t m_azul_sup_param; 
  color_t m_azul_inf_param;
} filtro_gestos_in_parametros_t;

#endif				//_FILTRO_GESTOS_SDK_H_
