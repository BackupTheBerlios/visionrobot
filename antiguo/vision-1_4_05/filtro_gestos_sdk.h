#ifndef _FILTRO_GESTOS_SDK_H_
#define _FILTRO_GESTOS_SDK_H_

const unsigned char NEGRO= 0;
const unsigned char BLANCO= 255;
const unsigned char SEMI_NEGRO= 30;
const int TOL=1;

typedef unsigned char color_t;

typedef struct {
  color_t *m_imagen;
  int m_alto;
  int m_ancho;
  int m_bytes;
} filtro_gestos_in_imagen_t;

typedef struct {
  color_t m_rojo_sup;
  color_t m_rojo_inf;
  color_t m_verde_sup;
  color_t m_verde_inf; 
  color_t m_azul_sup; 
  color_t m_azul_inf;
} filtro_gestos_in_parametros_t;


//***********************************************************************************
//Nuevas estructuras para el filtro de carteles


//*****************************************************************************


#endif
