/*! \file preferencias.h
    \brief Abre un cuadro de diálogo para establecer las opciones
    \author Carlos
    \version 0.1
*/
#ifndef _PREFERENCIAS_H_INCLUIDO_
#define _PREFERENCIAS_H_INCLUIDO_

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

/*! \brief Define la estructura de datos de una orden */
typedef struct {
  GdkColor color;
  float tolerancia;
} dato_t;

/*! \brief Define la estructura de datos de las opciones del filtro */
typedef struct {
  dato_t orden;
  dato_t argumento;
} respuesta_t;

/*! \brief Abre un cuadro de diálogo en el que se pueden establecer las opciones del filtro 
    \param respuesta Un puntero a una estructura de datos previamente creada en memoria
    \return TRUE si el usuario ha apretado Aceptar, false en caso contrario
*/
gboolean abrir_preferencias(respuesta_t * respuesta);

#ifdef __cplusplus
}
#endif

#endif
