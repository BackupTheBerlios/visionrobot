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
  En el programa que se esté compilando e incluir la librería con un simple
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

#ifndef _PIPELINE_H_INCLUIDO_
#define _PIPELINE_H_INCLUIDO_

#ifdef __cplusplus
extern "C" {
#endif


#define F_CICLO "ciclo"
#define F_INICIAR "iniciar"
#define F_CERRAR "cerrar"
#define F_PROPIEDADES "propiedades"
#define F_SET_DATOS "set_datos"
#define F_GET_DATOS "get_datos"
#define F_ERROR "error"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

  /*! \brief Para el ciclo, iniciar, propiedades y cerrar */
    typedef int (*funcion_1) ();
  /*! \brief Para get_datos */
    typedef void *(*funcion_2) ();
  /*! \brief Para set_datos */
    typedef int (*funcion_3) (const void *);
  /*! \brief Para error */
    typedef char *(*funcion_4) ();

/*! \brief La estructura de datos de cada elemento del pipe */
    struct elemento_s {
      int m_id;  /*!< El identificador del elemento, coincidirá (esperemos) con su índice */
      
#ifdef WIN32
      HMODULE
#else
      void *
#endif
      m_handler; /*!< El interfaz con la biblioteca dinámica */

      funcion_1 m_funcion_ciclo;  /*!< La función para el ciclo */
      funcion_1 m_funcion_iniciar; /*!< La función para iniciar */
      funcion_1 m_funcion_propiedades; /*!< La función de propiedades */
      funcion_1 m_funcion_cerrar; /*!< La función para cerrar */
      funcion_2 m_funcion_get_datos; /*!< La función para devolver datos */
      funcion_3 m_funcion_set_datos; /*!< La función para establecer datos */
      funcion_4 m_funcion_error;  /*!< La función que devuelve un error */
      char m_iniciado; /*!< Indica si el elemento está iniciado o no */
      char m_ruta[64]; /*!< Una cadena que indica la ruta de la biblioteca */
      char m_nombre[64]; /*!< Un nombre descriptivo, nada más */
      struct elemento_s *m_destino[20]; /*!< Una lista de punteros a otros elementos, para la conexión */
      int m_numero_conexiones; /*!< El número de conexiones actuales */
    };

/*! \brief Declaración del tipo de los elementos para su uso */
    typedef struct elemento_s elemento_t;

/*! \brief La estructura de datos del pipeline */
    struct pipeline_s {
      char m_corriendo; /*!< Si el pipe está activo o no, más que nada para informar */
      int m_numero; /*!< El número de elementos */
      int m_error; /*!< El índice del elemento al que se le envían los mensajes de error */
      elemento_t m_elemento[20]; /*!< La lista de elementos */
    };

/*! \brief Declaración del tipo del pipeline para su uso */
    typedef struct pipeline_s pipeline_t;

/*! \brief Crea un pipe
    \return Un puntero al pipe creado
*/
    pipeline_t *pipeline_crear();
/*! \brief Crea un módulo en el pipe
    \param pipeline El pipe en el se crea
    \param nombre El nombre que aparecerá reflejado
    \param ruta La ruta a la biblioteca dinámica
    \return El nuevo elemento
*/
    elemento_t *pipeline_nuevo(pipeline_t * pipeline, const char *nombre, const char *ruta);
/*! \brief Borra un pipe
    \param pipeline El pipe que se borra
    \return Devuelve 0
*/
    int pipeline_vaciar(pipeline_t * pipeline);
/*! \brief Borra un módulo
    \param pipeline El pipe
    \param id El id del módulo
    \return Devuelve 0
*/
    int pipeline_borrar(pipeline_t * pipeline, int id);
/*! \brief Guarda un pipeline en XML
    \param pipeline El pipe
    \param archivo La ruta en la que se guarda
    \return Devuelve 0
*/
    int pipeline_guardar(const pipeline_t * pipeline, const char *archivo);
    /*! \brief Carga un pipeline
       \param ruta La ruta del archivo XML    
       \return El nuevo pipeline
     */
    pipeline_t *pipeline_cargar(const char *ruta);
    /*! \brief Carga los símbolos de la nueva biblioteca. Si no se puede abrir, las funciones se pondrán a 0
       \param elemento El elemento que va a variar su biblioteca
     */
    void pipeline_cambiar_biblioteca(elemento_t * elemento);
    /*! \brief Conecta dos módulos
       \param pipeline El pipeline que contiene los módulos
       \param origen El índice del elemento origen
       \param destino El índice del elemento destino
       \return 0 si todo va bien, -1 en caso de error
     */
    int pipeline_conectar(pipeline_t * pipeline, int origen,int destino);
  /*! \brief Cierra la comunicación con la biblioteca, llamando antes a "cerrar"
      \param elemento El elemento cuya biblioteca vamos a cerrar
      \return -1 en caso de error, 0 si todo va bien
  */
    int pipeline_cerrar_biblioteca(elemento_t * elemento);
  /*! \brief Cierra todas las bibliotecas. Llama secuencialmente a pipeline_cerrar_biblioteca.
      \param pipeline El pipeline cuyas bibliotecas cerramos
      \return -1 en caso de error, 0 si todo va bien
  */
    int pipeline_cerrar_todas_bibliotecas(pipeline_t * pipeline);
  /*! \brief Envía un error al módulo de salida de errores del pipe
      \param pipeline El pipeline en el que trabajamos
      \param elemento El elemento que envía el error
      \param error Una cadena de error, se supone que el módulo sabrá tratarla
      \return -1 en caso de error, 0 si todo va bien
  */
    int pipeline_error(const pipeline_t * pipeline, const elemento_t * elemento, const char *error);
  /*! \brief Un ciclo del pipe
      \param pipeline El pipe
      \return -1 en caso de error, 0 si todo va bien
  */
    int pipeline_ciclo(const pipeline_t * pipeline);
  /*! \brief Inicia un módulo del pipe. Después de llamar a la función de inicio, se llamará a <code>error()</code>
      \param pipeline El pipe en el que trabajamos
      \param elemento El elemento que iniciamos
      \return -1 en caso de error, 0 si todo va bien
  */
    int pipeline_iniciar(const pipeline_t * pipeline, elemento_t * elemento);
  /*! \brief Detiene un módulo del pipe. Después de llamar a la función de parada, se llamará a <code>error()</code>
      \param pipeline El pipe en el que trabajamos
      \param elemento El elemento que paramos
      \return -1 en caso de error, 0 si todo va bien
  */    
  int pipeline_parar(const pipeline_t * pipeline, elemento_t * elemento);
  /*! \brief Abre la ventana de propiedades. Después de llamar a la función de propiedades, el pipeline captará el mensaje de información y/o error.
      \param pipeline El pipe en el que trabajamos
      \param elemento El elemento cuya ventana queremos abrir
      \return 0 si todo va bien, -1 si algo falla
  */
  int pipeline_abrir_propiedades(const pipeline_t * pipeline, const elemento_t * elemento);

#ifdef __cplusplus
}
#endif
#endif
