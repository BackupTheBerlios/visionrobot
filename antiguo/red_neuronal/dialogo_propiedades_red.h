/*! \file dialogo_propiedades_red.h
    \brief Define el cuadro de diálogo de opciones de la red neuronal
    \author Carlos
    \version 0.1
*/
#ifndef _DIALOGO_PROPIEDADES_RED_H_INCLUIDO_
#define _DIALOGO_PROPIEDADES_RED_H_INCLUIDO_

#include <gtk/gtk.h>

/*! \brief Estructura de datos del diálgo */

typedef struct {
  GtkWidget *dlg_propiedades_red;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *hbox1;
  GtkWidget *entry_ruta;
  GtkWidget *button1;
  GtkWidget *alignment1;
  GtkWidget *hbox2;
  GtkWidget *image1;
  GtkWidget *label4;
  GtkWidget *hseparator1;
  GtkWidget *frame1;
  GtkWidget *vbox2;
  GtkWidget *rbt_ordenes;
  GSList *rbt_ordenes_group;
  GtkWidget *rbt_parametros;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;
} dialogo_propiedades_red_t;

/*! \brief Cierra un diálogo de este tipo
    \param dialogo El diálogo que hay que cerrar
*/
void dialogo_propiedades_red_cerrar(dialogo_propiedades_red_t * dialogo);

/*! \brief Crea un diálogo de preferencias
    \return Un puntero al nuevo diálogo
*/
dialogo_propiedades_red_t * dialogo_propiedades_red_crear();

#endif
