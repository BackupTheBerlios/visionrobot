#ifndef _FILTRO_H_
#define _FILTRO_H_

#include "filtro_gestos_sdk.h"
#include "red_neuronal_sdk.h"

typedef struct {
    int m_r, m_v, m_a;
} color_t;

typedef struct {
    color_t m_superior;
    color_t m_inferior;
} rango_t;

typedef struct {
    red_neuronal_in_t * m_salida;
    filtro_gestos_in_t *m_buffer;
    rango_t m_orden;
    rango_t m_parametro;
} filtro_t;

filtro_t * filtro_gestos_crear();
void filtro_gestos_borrar(filtro_t **filtro);
red_neuronal_in_t* filtro_gestos_filtrar(filtro_t * filtro);
void filtro_gestos_set_color(filtro_t *filtro, int rs,int ri,int vs,int vi,int as,int ai,int rs2,int ri2,int vs2,int vi2,int as2,int ai2);
char * filtro_gestos_centrar(filtro_t *filtro, char *dibujo, int difY, int difX);

#endif //_FILTRO_H_
