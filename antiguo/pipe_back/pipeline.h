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

#ifndef _PIPELINE_H
#define _PIPELINE_H

#include <gtk/gtk.h>

#define MAX_ELEMENTOS 32
#define MAX_CONEXIONES 8
#define MAX_RUTA 128
#define MAX_NOMBRE 64

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/*! \brief Provisionalmente, la llamada a cada ciclo del pipeline */
    typedef int (*funcion_ciclo) ();

/*! \brief La estructura de datos de cada elemento del pipe */
    struct elemento_s {
	GtkWidget *m_widget;
	gint m_x;
	gint m_y;
#ifdef WIN32
	 HMODULE
#else
	void *
#endif
	 m_handler;

	funcion_ciclo m_funcion_ciclo;

	gchar m_ruta[MAX_RUTA];
	gchar m_nombre[MAX_NOMBRE];
	struct elemento_s *m_destino[MAX_CONEXIONES];
    };

/*! \brief Declaración del tipo de los elementos para su uso */
    typedef struct elemento_s elemento_t;

/*! \brief La estructura de datos del pipeline */
    struct pipeline_s {
	int m_numero;
	elemento_t m_elemento[MAX_ELEMENTOS];
    };

/*! \brief Declaración del tipo del pipeline para su uso */
    typedef struct pipeline_s pipeline_t;

/*! \brief Crea un pipe
    \return El pipe creado
*/
    pipeline_t *crear_pipeline();
/*! \brief Crea un módulo en el pipe
    \param pipeline El pipe en el se crea
    \param nombre El nombre que aparecerá reflejado
    \param x La coordenada x
    \param y La coordenada y
    \param ruta La ruta a la biblioteca dinámica
    \return El nuevo elemento
*/
    elemento_t *nuevo(pipeline_t * pipeline, const char *nombre, gint x,
		      gint y, gchar * ruta);
/*! \brief Borra un pipe
    \param pipeline El pipe que se borra
    \return Devuelve 0
*/
    int vaciar_pipeline(pipeline_t * pipeline);
/*! \brief Borra un módulo
    \param pipeline El pipe
    \param id El id del módulo
    \return Devuelve 0
*/
    int borrar(pipeline_t * pipeline, gint id);
/*! \brief Guarda un pipeline en XML
    \param pipeline El pipe
    \param archivo La ruta en la que se guarda
    \return Devuelve 0
*/
    int guardar(pipeline_t * pipeline, const char *archivo);
/*! \brief Carga un pipeline
    \param ruta La ruta del archivo XML    
    \return El nuevo pipeline
*/
    pipeline_t *cargar(const char *ruta);

    void cambiar_biblioteca(pipeline_t * pipeline, gint id);

#ifdef __cplusplus
}
#endif
#endif				/* _PIPELINE_H */
