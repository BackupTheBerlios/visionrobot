/*! \file dialogo_preferencias_filtro.h
    \brief Define el cuadro de diálogo de opciones
    \author Carlos
    \version 0.1
*/
#ifndef _DIALOGO_PREFERENCIAS_FILTRO_H_INCLUIDO_
#define _DIALOGO_PREFERENCIAS_FILTRO_H_INCLUIDO_

#include <gtk/gtk.h>

/*! \brief Estructura de datos del diálgo */
typedef struct {
  GtkWidget *dlg_preferencias_filtro;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *lab_info_dialogo;
  GtkWidget *hbox1;
  GtkWidget *frame1;
  GtkWidget *vbox2;
  GtkWidget *label4;
  GtkWidget *bot_color_ordenes;
  GtkWidget *label6;
  GtkWidget *sld_tolerancia_ordenes;
  GtkWidget *label2;
  GtkWidget *frame2;
  GtkWidget *vbox3;
  GtkWidget *label5;
  GtkWidget *bot_color_argumentos;
  GtkWidget *label7;
  GtkWidget *sld_tolerancia_argumentos;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;
} dialogo_preferencias_filtro_t;

/*! \brief Cierra un diálogo de este tipo
    \param dialogo El diálogo que hay que cerrar
*/
void dialogo_preferencias_filtro_cerrar(dialogo_preferencias_filtro_t * dialogo);

/*! \brief Crea un diálogo de preferencias
    \return Un puntero al nuevo diálogo
*/
dialogo_preferencias_filtro_t * dialogo_preferencias_filtro_crear();

#endif
