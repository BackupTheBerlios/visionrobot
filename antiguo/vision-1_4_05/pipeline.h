/*! \file pipeline.h
    \brief Cabecera del módulo de pipelines
    \author Carlos León
    \version 0.1
 */  
    
/*! \mainpage
  \section intro Introducción
  Esta parte del proyecto es una biblioteca que dota de la funcionalidad
  de la arquitectura en tubería, de forma que los diferentes módulos
  puedan interactuar de una forma autónoma.
  \section uso Uso
  Para poder usar la biblioteca simplemente es necesario enlazarla
  En el programa que se está compilando e incluir la librería con un simple
  <code>#include "pipeline.h"</code>.
  \section plataforma Plataforma
  El código de esta biblioteca ha sido programado para compilar sin problemas
  (a priori) en plataformas Win32 y Unix.
  \section licencia Licencia
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/ 
    
#ifndef _PIPELINE_H_
#define _PIPELINE_H_
    
#ifdef __cplusplus
extern "C"  {
    
#endif

#include "pipeline_sdk.h"
  
#include <gmodule.h>
  
  
  typedef void (*funcion_error_t)(const char *nombre, const char *texto);
  
  typedef struct {
    GString * m_nombre;
    GModule * m_handler;
    modulo_t * m_modulo;
    funcion_error_t m_funcion_error;
    GHashTable *m_argumentos;
    GSList *m_enlaces;
  } elemento_t;

  typedef GSList pipeline_t;

  /*int  pipeline_cerrar(elemento_t * elemento);
  int  pipeline_borrar(elemento_t * elemento);
  int  pipeline_guardar(const elemento_t * elemento, const char *ruta);
  elemento_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error);
  int  pipeline_ciclo(const elemento_t * elemento);
  int  pipeline_iniciar(const elemento_t * elemento);*/
  int  pipeline_cerrar(pipeline_t * p) ;
  int  pipeline_borrar(pipeline_t * p) ;
  pipeline_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error) ;
  int pipeline_ciclo(const pipeline_t * p);
  int  pipeline_iniciar(const pipeline_t * p);
  
#ifdef __cplusplus
} 
#endif
#endif
