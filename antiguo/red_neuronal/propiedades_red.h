/*! \file preferencias_red.h
    \brief Abre un cuadro de diálogo para establecer las opciones de la red
    \author Carlos
    \version 0.1
*/
#ifndef _PROPIEDADES_RED_H_INCLUIDO_
#define _PROPIEDADES_RED_H_INCLUIDO_

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

#include "CImagen.h"

/*! \brief El tipo que define qué orden ha elegido el usuario */
//typedef enum {ORDEN = 0, ARGUMENTO = 1} orden_t;

/*! \brief Define la estructura de datos de las opciones */
typedef struct {
  char ruta[128];
  tipoFoto tipo;
} red_t;


/*! \brief Abre un cuadro de diálogo en el que se pueden establecer las opciones de la red
    \param respuesta Un puntero a una estructura de datos previamente creada en memoria
    \return TRUE si el usuario ha apretado Aceptar, false en caso contrario
*/
gboolean abrir_propiedades_red(red_t * red);

#ifdef __cplusplus
}
#endif

#endif
