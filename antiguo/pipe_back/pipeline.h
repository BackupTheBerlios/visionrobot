/*! \file pipeline.h
    \brief Cabecera del módulo de pipelines
    \author zosco
    \version 0.1
 */
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _PIPELINE_H_INCLUIDO_
#define _PIPELINE_H_INCLUIDO_

#ifdef __cplusplus
extern "C" {
#endif

//#include <gtk/gtk.h>

/*#define MAX_ELEMENTOS 32
#define MAX_CONEXIONES 8*/
#define MAX_RUTA 128
#define MAX_NOMBRE 64

#define F_CICLO "ciclo"
#define F_INICIAR "iniciar"
#define F_CERRAR "cerrar"
#define F_PROPIEDADES "propiedades"
#define F_SET_DATOS "set_datos"
#define F_GET_DATOS "get_datos"
#define F_ERROR "error"

/*#define F_CICLO_ "_ciclo"
#define F_INICIAR_ "_iniciar"
#define F_CERRAR_ "_cerrar"
#define F_PROPIEDADES_ "_propiedades"
#define F_SET_DATOS_ "_set_datos"
#define F_GET_DATOS_ "_get_datos"
#define F_ERROR_ "_error"*/


#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/*! \brief Provisionalmente, la llamada a cada ciclo del pipeline */
    typedef int (*funcion_1) ();
    typedef void *(*funcion_2) ();
    typedef int (*funcion_3) (const void *);
    typedef char *(*funcion_4) ();

/*! \brief La estructura de datos de cada elemento del pipe */
    struct elemento_s {
      /*guint*/int m_id;
	//GtkWidget *m_widget;
	/*gint m_x;
	gint m_y;*/
#ifdef WIN32
	 HMODULE
#else
	void *
#endif
	 m_handler;

	funcion_1 m_funcion_ciclo;
	funcion_1 m_funcion_iniciar;
	funcion_1 m_funcion_propiedades;
	funcion_1 m_funcion_cerrar;	
	funcion_2 m_funcion_get_datos;
	funcion_3 m_funcion_set_datos;
	funcion_4 m_funcion_error;	
	char m_iniciado;
	/*gchar*/char m_ruta[MAX_RUTA];
	/*gchar*/char m_nombre[MAX_NOMBRE];
	struct elemento_s **m_destino/*[MAX_CONEXIONES]*/;
	/*gint*/int m_numero_conexiones;
    };

/*! \brief Declaración del tipo de los elementos para su uso */
    typedef struct elemento_s elemento_t;

/*! \brief La estructura de datos del pipeline */
    struct pipeline_s {
	/*gboolean*/char m_corriendo;
	int m_numero;
	int m_error;
	elemento_t *m_elemento/*[MAX_ELEMENTOS]*/;
    };

/*! \brief Declaración del tipo del pipeline para su uso */
    typedef struct pipeline_s pipeline_t;

/*! \brief Crea un pipe
    \return El pipe creado
*/
    pipeline_t *pipeline_crear();
/*! \brief Crea un módulo en el pipe
    \param pipeline El pipe en el se crea
    \param nombre El nombre que aparecerá reflejado
    \param x La coordenada x
    \param y La coordenada y
    \param ruta La ruta a la biblioteca dinámica
    \return El nuevo elemento
*/
    elemento_t *pipeline_nuevo(pipeline_t * pipeline, const char *nombre/*, gint x,
		      gint y*/, /*gchar*/const char * ruta);
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
    int pipeline_borrar(pipeline_t * pipeline, int/*gint*/ id);
/*! \brief Guarda un pipeline en XML
    \param pipeline El pipe
    \param archivo La ruta en la que se guarda
    \return Devuelve 0
*/
    int pipeline_guardar(pipeline_t * pipeline, const char *archivo);
    /*! \brief Carga un pipeline
       \param ruta La ruta del archivo XML    
       \return El nuevo pipeline
     */
    pipeline_t *pipeline_cargar(const char *ruta);

    /*! \brief Carga los símbolos de la nueva biblioteca
       \param pipeline El pipe sobre el que cargamos la biblioteca
       \param id El id del elemento sobre el que hacemos los cambios
     */
  void pipeline_cambiar_biblioteca(elemento_t * elemento);//pipeline_t * pipeline, gint id);

    /*! \brief Conecta dos módulos
       \param pipeline El pipeline que contiene los módulos
       \param origen El índice del elemento origen
       \param destino El índice del elemento destino
       \return 0 si todo va bien, -1 en casa
     */
    int pipeline_conectar(pipeline_t * pipeline, /*gint*/int origen, /*gint*/int destino);

  int pipeline_cerrar_biblioteca(elemento_t * elemento);
  int pipeline_cerrar_todas_bibliotecas(pipeline_t * pipeline);
  
  int pipeline_error(pipeline_t * pipeline, const char * error);
  int pipeline_ciclo(pipeline_t * pipeline);
  int pipeline_iniciar(pipeline_t * pipeline, elemento_t * elemento);
  int pipeline_parar(pipeline_t * pipeline, elemento_t * elemento);

#ifdef __cplusplus
}
#endif
#endif
