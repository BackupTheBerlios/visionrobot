/*! \file pipeline.h
    \brief Cabecera del m�dulo de pipelines
    \author Carlos Le�n
    \version 1.0
 */  
    
/*! \mainpage
  \section intro Introducci�n
  Esta parte del proyecto es una biblioteca que dota de la funcionalidad
  de la arquitectura en tuber�a, de forma que los diferentes m�dulos
  puedan interactuar de una forma aut�noma.
  \section uso Uso
  Para poder usar la biblioteca simplemente es necesario enlazarla
  En el programa que se est� compilando e incluir la librer�a con un simple
  <code>#include "pipeline.h"</code>.
  \section plataforma Plataforma
  El c�digo de esta biblioteca ha sido programado para compilar sin problemas
  (a priori) en todas las plataformas que soporte la librer�a GLib.
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

  /*! \brief Define una funci�n de retrollamada que procesa un error.

             Esta funci�n ser� llamada por el pipeline para controlar los errores. La funci�n debe aceptar que cualquiera de los par�metros sea 0.
      \param nombre El causante del mensaje, generalmente el nombre del elemento
      \param modulo El nombre del m�dulo
      \param texto El texto que define el mensaje
      \param dato El dato auxiliar que ha definido el usuario en <code>pipeline_cargar()</code>
  */
  typedef void (*funcion_error_t)(const char *nombre, const char *modulo, const char *texto, void *dato);
  
  /*! \brief La estructura de datos del pipeline. Es privada y dependiente de la implementaci�n,
    para acceder a ella hay que usar los m�todos provistos.
  */
  typedef struct pipeline_s pipeline_t;
  
  /*! \brief Cierra todas las bibliotecas abiertas en el pipeline y libera toda la memoria usada. 
    \param p El pipeline que queremos eliminar
    \return 0 en caso de �xito, -1 si algo hay ido mal
  */
  int  pipeline_borrar(pipeline_t * p) ;

  /*! \brief Carga un archivo XML que define un proyecto de pipeline. Realiza tambi�n la validaci�n con el archivo
             <em>pipeline.dtd</em>. Si es v�lido, lo carga, creando el pipeline correspondiente.
      \param ruta La ruta del archivo XML que carga      
      \param funcion_error Un puntero a una funci�n de retrollamada que gestiona los mensajes de salida (no s�lo de error
                           del pipeline
      \param dato El dato auxiliar que se le pasa a la funci�n de error. Puede ser una estructura de cualquier tipo.
  */
  pipeline_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error, const void *dato) ;

  /*! \brief Realiza un ciclo completo en el pipeline. Esto corresponde a llamar al ciclo de las funciones
            marcadas como <em>iniciales</em> (inicio="1" en el XML), y llamar al ciclo de todos los m�dulos
	    que est�n conectados a la salida de los mismos.
      \param p El pipeline en el que realizamos el ciclo
  */
  int pipeline_ciclo(const pipeline_t * p);

  /*! \brief Llama a la funci�n <em>iniciar</em> de todos los m�dulos que se han cargado en el pipeline.
      \param p El pipeline cuyos m�dulos inciamos
  */
  int  pipeline_iniciar(const pipeline_t * p);

  //! Devuelve una lista con los nombres de los m�dulos
  /*! 
    
  \param p El pipeline del que sacamos los nombres.
  
  \return Un array terminado en 0 de char *, que contiene los nombres de los m�dulos. No se debe borrar.
  */
  char ** pipeline_nombres(pipeline_t *p);

  //! Establece si un elemento del pipeline est� activo o no.
  /*! 
    
  \param p El pipeline que contiene el elemento.
  \param e El elemento contenido por el pipeline.
  \param activo "1" si ponemos a activo, y "0" si queremos que est� inactivo.
  */
  void pipeline_set_activo(const pipeline_t *p, const char *e, char activo);

  //! Devuelve la informaci�n sobre la actividad de un elemento.
  /*! 
    
  \param p El pipeline que contiene al elemento.
  \param e El nombre del elemento que estamos examinando.
  
  \return "1" si est� activo, y "0" si est� inactivo.
  */
  char pipeline_get_activo(const pipeline_t *p, const char *e);

#ifdef __cplusplus
} 
#endif
#endif
