/*! \file pipeline.h
    \brief Cabecera del módulo de pipelines
    \author Carlos León
    \version 1.0
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
  (a priori) en todas las plataformas que soporte la librería GLib.
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

  /*! \brief Define una función de retrollamada que procesa un error.

             Esta función será llamada por el pipeline para controlar los errores. La función debe aceptar que cualquiera de los parámetros sea 0.
      \param nombre El causante del mensaje, generalmente el nombre del elemento
      \param modulo El nombre del módulo
      \param texto El texto que define el mensaje
  */
  typedef void (*funcion_error_t)(const char *nombre, const char *modulo, const char *texto);
  
  /*! \brief La estructura de datos del pipeline. Es privada y dependiente de la implementación,
    para acceder a ella hay que usar los métodos provistos.
  */
  typedef struct pipeline_s pipeline_t;
  
  /*! \brief Cierra todas las bibliotecas abiertas en el pipeline y libera toda la memoria usada. 
    \param p El pipeline que queremos eliminar
    \return 0 en caso de éxito, -1 si algo hay ido mal
  */
  int  pipeline_borrar(pipeline_t * p) ;

  /*! \brief Carga un archivo XML que define un proyecto de pipeline. Realiza también la validación con el archivo
             <em>pipeline.dtd</em>. Si es válido, lo carga, creando el pipeline correspondiente.
      \param ruta La ruta del archivo XML que carga
      \param dir El pipeline carga bibliotecas dinámicas como módulos. Para ello, sólo se especifica el nombre,
                 sin el prefijo lib* ni la extensión (dll, so...). Para que se puedan cargar los módulos correctamente,
		 es necesario indicar la ruta en la que se encuentran los módulos. Este parámetro se encarga de ello.
      \param funcion_error Un puntero a una función de retrollamada que gestiona los mensajes de salida (no sólo de error
                           del pipeline
  */
  pipeline_t * pipeline_cargar(const char *ruta, const char *dir, funcion_error_t funcion_error) ;

  /*! \brief Realiza un ciclo completo en el pipeline. Esto corresponde a llamar al ciclo de las funciones
            marcadas como <em>iniciales</em> (incio="1" en el XML), y llamar al ciclo de todos los módulos
	    que estén conectados a la salida de los mismos.
      \param p El pipeline en el que realizamos el ciclo
  */
  int pipeline_ciclo(const pipeline_t * p);

  /*! \brief Llama a la función <em>iniciar</em> de todos los módulos que se han cargado en el pipeline.
      \param p El pipeline cuyos módulos inciamos
  */
  int  pipeline_iniciar(const pipeline_t * p);
  
#ifdef __cplusplus
} 
#endif
#endif
