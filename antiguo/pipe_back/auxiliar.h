/*! \file auxiliar.h
    \brief Funciones auxiliares para la aplicación de pipeline
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
  */
#ifndef _AUX_H
#define _AUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include "pipeline.h"

/*! \brief Presenta un mensaje de entrada, y devuelve la respuesta
    \param w La ventana padre
    \param pregunta El texto que se presenta
    \return La cadena que el usuario escribe
*/
    gchar *entrada(GtkWidget * w, const gchar * pregunta);

/*! \brief Presenta un mensaje de confirmación, y devuelve la respuesta
    \param w La ventana padre
    \param texto El texto que se presenta
    \return TRUE si se pulsa sí, FALSE en caso contrario
*/
    gboolean confirmacion(GtkWidget * w, const gchar * texto);

/*! \brief Pide confirmación para salir
    \param w La ventana padre
    \param modificado Variable que dice si el pipeline hay que grabarlo
    \return TRUE si se pulsa sí, FALSE en caso contrario
*/
    gboolean salir(GtkWidget * w, gboolean modificado);

/*! \brief Presenta un mensaje de información
    \param w La ventana padre
    \param texto El texto que se presenta
*/
    void info(GtkWidget * w, const gchar * texto);

/*! \brief Enseña ventana de información de la aplicación
    \param w La ventana padre
*/
    void acerca_de(GtkWidget * w);

/*! \brief Abre un archivo, mostrando un cuadro de diálogo
    \param w La ventana padre
    \return El puntero a FILE del archivo abierto
*/
    char *abrir_ventana(GtkWidget * w);

/*! \brief Guardar un archivo, mostrando un cuadro de diálogo
    \param w La ventana padre
    \return El puntero a FILE del archivo abierto
*/

    char *guardar_ventana(GtkWidget * w);
/*! \brief Abre la ventana de propiedades de un elemento
    \param id_elemento El id del elemento a modificar, -1 si es un elemento nuevo    
    \param fixed El GtkFixed en el que está el botón    
    \param pipeline El pipe en el que está el elemento
    \param mod Variable de modificación que vamos a cambiar
*/
    void propiedades(gint id_elemento, GtkFixed * fixed,
		     pipeline_t * pipeline, gboolean * mod);

/*! \brief Incluye en un "fixed" los elementos de un pipeline     
    \param fixed El GtkFixed en el que se incluyen    
    \param pipeline El pipeline que vamos a incluir
*/
    void insertar(GtkFixed * fixed, pipeline_t * pipeline);

/*! \brief Muestra un mensaje en el StatusBar     
    \param b El puntero al StatusBar    
    \param info El mensaje
    \param id Un id que tiene que ser siempre el mismo
    \warning Esta función es bastante precaria.
*/
    void mostrar(GtkStatusbar * b, const char *info, guint * id);

/*! \brief Establece unos cuantos menús     
    \param pipeline Hay que pasarle un pipeline para que tenga control de los botones    
    \param window Y una ventana donde están los botones
*/
    void establecer(pipeline_t * pipeline, GtkWidget * window);

/*! \brief Guarda un pipeline - Se usa para hacer iguales el guardar y el guardar como     
    \param pipeline El pipe que se guarda  
    \param file La ruta que se va a salvar
    \param window La ventana de la aplicación
    \param status_bar Un GtkStatusbar para que se refleje todo
    \param archivo El nombre por defecto del archivo actual
    \param id Un id, igual que en mostrar
    \param modificado La variable que establecemos para que la aplicación sepa si hay cambios
*/
    void guardar_como_aux(pipeline_t * pipeline, char *file,
			  GtkWidget * window, GtkWidget * status_bar,
			  char *archivo, guint * id,
			  gboolean * modificado);

    /*! \brief Desactiva la ejecución de un elemento
       \param elemento El elemento que paramos
       \return 0 si todo va bien, -1 en caso de error
     */
    int parar(elemento_t * elemento);
    /*!\brief Activa la ejecución de un elemento
       \param elemento El elemento que activamos
       \return 0 si todo va bien, -1 en caso de error
     */
    int iniciar(elemento_t * elemento);
    /*!\brief Activa el temporizador de ciclos del pipeline. Esta función crea una alarma que realiza ciclos seguidos hasta que se para la alarma.
       \param retardo El retardo en microsegundos
       \return 0 si todo va bien, -1 en caso de error
     */

    int crear_timer(long int retardo);
    /*!\brief Desactiva el temporizador de ciclos
       \return 0 si todo va bien, -1 en caso de error
     */

    int parar_timer();
    /*!\brief Inicia el sistema de señales
       \return 0 si todo va bien, -1 en caso de error
     */

    int senyal();
    /*!\brief Efectúa un único ciclo del pipeline
       \param pipeline El pipe sobre el que se ejecuta el ciclo
       \return 0 si todo va bien, -1 en caso de error
     */

    int haz_un_ciclo(pipeline_t * pipeline);

    int elegir_modulo(GtkWidget * window1, pipeline_t * pipeline);

#ifdef __cplusplus
}
#endif
#endif				/* _AUX_H */
