#ifndef _FILTRO_H_
#define _FILTRO_H_

#include "filtro_gestos_sdk.h"
#include "red_neuronal_sdk.h"
#include <lua.h>

typedef struct {
  unsigned char m_r, m_v, m_a;
} color3_t;

typedef struct {
  color3_t m_superior;
  color3_t m_inferior;
} rango_t;

typedef struct {
  red_neuronal_in_t m_salida;
  filtro_gestos_in_imagen_t * m_buffer;
  color_t * m_tipo_orden;
  color_t * m_orden_param;
  char m_iniciado;
  rango_t m_orden;
  const char *m_iniciar;
  const char *m_filtrar;
  rango_t m_parametro;
  lua_State *m_lua;
} filtro_t;

filtro_t * filtro_gestos_crear(const char *ruta, const char *iniciar, const char *filtrar);
void filtro_gestos_borrar(filtro_t ** filtro);
red_neuronal_in_t * filtro_gestos_filtrar(filtro_t * filtro);
void filtro_gestos_set_color(filtro_t * filtro, color_t rs, color_t ri, color_t vs,
			     color_t vi, color_t as, color_t ai, color_t rs2, color_t ri2,
			     color_t vs2, color_t vi2, color_t as2, color_t ai2);
char *filtro_gestos_centrar(filtro_t * filtro, char *dibujo, int difY,
			    int difX);

#endif	
