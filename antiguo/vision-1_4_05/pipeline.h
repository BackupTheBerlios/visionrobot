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
    
#endif	/*  */
    
#ifdef WIN32
#include <windows.h>
    typedef HMODULE tipo_handler_t;
    
#define pipeline_free_library(x) FreeLibrary((x))
#define pipeline_load_library(x) LoadLibrary((x))
#define pipeline_get_symbol(x, y) GetProcAddress((x), TEXT((y)))
#define pipeline_error() 0
#else	/*  */
#include <dlfcn.h>
    typedef void *tipo_handler_t;
    
#define pipeline_free_library(x) dlclose((x))
#define pipeline_load_library(x) dlopen((x), RTLD_LAZY)
#define pipeline_get_symbol(x, y) dlsym((x),(y))
#define pipeline_error() dlerror()
#endif	/*  */
    
#include "pipeline_sdk.h"
  typedef modulo_t *(*funcion_get_modulo) ();
     struct elemento_s  {
	tipo_handler_t m_handler;	/*!< El interfaz con la biblioteca dinámica */
	char **m_argumentos;	/*!< Una lista de cadenas, que representan argumentos */
	 int m_numero_argumentos;
	 modulo_t * m_modulo;
	 char *m_ruta;		/*!< Una cadena que indica la ruta de la biblioteca */
	 struct elemento_s **m_destino;	/*!< Una lista de punteros a otros elementos, para la conexión */
	struct elemento_s *m_padre;
	int m_numero_conexiones;	/*!< El número de conexiones actuales */
    };
     
/*! \brief Declaración del tipo de los elementos para su uso */ 
    typedef struct elemento_s elemento_t;
  elemento_t * pipeline_nuevo(elemento_t * padre, const char *ruta);
  int  pipeline_cerrar_biblioteca(elemento_t * elemento);
  int  pipeline_cerrar_todo(elemento_t * elemento);
  int  pipeline_borrar(elemento_t * elemento);
  int  pipeline_guardar(const elemento_t * elemento, const char *ruta);
  elemento_t * pipeline_cargar(const char *ruta);
  int  pipeline_ciclo(const elemento_t * elemento, const pipeline_dato_t *dato);
  int  pipeline_iniciar(const elemento_t * elemento);
  int  pipeline_iniciar_todo(const elemento_t * elemento);
  void  pipeline_borrar_argumentos(elemento_t * elemento);
  void 
  pipeline_set_argumentos(elemento_t * elemento, 
				const char **argumentos,
				int numero_argumentos);
    int  pipeline_set_ruta(elemento_t * elemento, const char *ruta);
    
#ifdef __cplusplus
} 
#endif	/*  */
#endif	/*  */
