#ifndef _FILTRO_H_
#define _FILTRO_H_

#include "filtro_gestos_sdk.h"
#include "red_neuronal_sdk.h"

typedef struct {
  unsigned char m_r, m_v, m_a;
} color3_t;

typedef struct {
  color3_t m_superior;
  color3_t m_inferior;
} rango_t;

typedef struct {
  red_neuronal_in_t * m_salida;
  filtro_gestos_in_imagen_t * m_buffer;
  color_t * m_tipo_orden;
  color_t * m_orden_param;
  rango_t m_orden;
  rango_t m_parametro;
} filtro_t;

filtro_t * filtro_gestos_crear();
void filtro_gestos_borrar(filtro_t ** filtro);
red_neuronal_in_t * filtro_gestos_filtrar(filtro_t * filtro, int difuminado, int reduccion);
void filtro_gestos_set_color(filtro_t * filtro, unsigned char rs, unsigned char ri, unsigned char vs,
			      unsigned char vi, unsigned char as, unsigned char ai, unsigned char rs2, unsigned char ri2,
			     unsigned char vs2, unsigned char vi2, unsigned char as2, unsigned char ai2);
char *filtro_gestos_centrar(filtro_t * filtro, char *dibujo, int difY,
			    int difX);

#endif				//_FILTRO_H_
