/*! \file callbacks.c
    \brief Funciones de eventos de Gtk
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
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "auxiliar.h"
#include "pipeline.h"

#define TAM_ARCHIVO 64

extern GtkWidget *window1;
pipeline_t *pipeline;
guint id;
char archivo[TAM_ARCHIVO];
gboolean modificado = FALSE;
GtkWidget *status_bar;

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    if (confirmacion
	(window1, "¿ Realmente desea borrar los módulos elegidos ?")) {
	int i;
	for (i = 0; i < pipeline->m_numero; ++i) {
	    if (gtk_toggle_button_get_active
		(GTK_TOGGLE_BUTTON(pipeline->m_elemento[i].m_widget))) {
		borrar(pipeline, i);
		i--;
		modificado = TRUE;
	    }
	}
	establecer(pipeline, window1);
    }
}


void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    acerca_de(window1);
}


void on_nuevo2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    if (confirmacion
	(window1,
	 "¿ Realmente desea eliminar todos los módulos del pipeline ?")) {
	vaciar_pipeline(pipeline);
	pipeline = crear_pipeline();
	archivo[0] = '\0';
	mostrar(GTK_STATUSBAR(status_bar), 0, &id);
	modificado = FALSE;
	gtk_widget_set_sensitive(lookup_widget(window1, "nuevo2"), FALSE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar2"),
				 FALSE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar_como2"),
				 FALSE);
	establecer(pipeline, window1);
    }

}


void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    char *file = abrir_ventana(window1);
    if (file != 0) {
	vaciar_pipeline(pipeline);
	pipeline = cargar(file);
	strcpy(archivo, file);
	modificado = FALSE;
	mostrar(GTK_STATUSBAR(status_bar), archivo, &id);
	g_free(file);
	GtkWidget *fixed = lookup_widget(window1, "fixed1");
	insertar(GTK_FIXED(fixed), pipeline);
	gtk_widget_set_sensitive(lookup_widget(window1, "nuevo2"), TRUE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar_como2"),
				 TRUE);
	int i;
	for (i = 0; i < pipeline->m_numero; ++i) {
	    g_signal_connect((gpointer) pipeline->m_elemento[i].m_widget,
			     "toggled", G_CALLBACK(pinchado), NULL);
	}
	establecer(pipeline, window1);
    }
}


void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    gchar *file = guardar_ventana(window1);
    guardar_como_aux(pipeline,
		     file, window1, status_bar, archivo, &id, &modificado);
    g_free(file);
}


void on_guardar2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    guardar_como_aux(pipeline,
		     archivo,
		     window1, status_bar, archivo, &id, &modificado);
}


void on_salir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    if (salir(window1, modificado)) {
	gtk_main_quit();
    }
}


void on_pipeline1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

void pinchado(GtkMenuItem * menuitem, gpointer user_data)
{
    establecer(pipeline, window1);
}

void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    GtkWidget *fixed = lookup_widget(window1, "fixed1");
    propiedades(-1, GTK_FIXED(fixed), pipeline, &modificado);
    if (modificado) {
	gtk_widget_set_sensitive(lookup_widget(window1, "nuevo2"), TRUE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar2"), TRUE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar_como2"),
				 TRUE);
	g_signal_connect((gpointer) pipeline->
			 m_elemento[pipeline->m_numero - 1].m_widget,
			 "toggled", G_CALLBACK(pinchado), NULL);
    }
}




void on_conectar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}


void on_ayuda1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

gboolean
on_window1_delete_event(GtkWidget * widget,
			GdkEvent * event, gpointer user_data)
{
    return !salir(window1, modificado);
}

void on_window1_destroy(GtkObject * object, gpointer user_data)
{
    vaciar_pipeline(pipeline);
    gtk_main_quit();
}

void on_window1_show(GtkWidget * widget, gpointer user_data)
{
    status_bar = lookup_widget(window1, "statusbar1");
    id = gtk_statusbar_push(GTK_STATUSBAR(status_bar), 0,
			    "Visión por computador");
    pipeline = crear_pipeline();

}

void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    int i;
    GtkWidget *fixed = lookup_widget(window1, "fixed1");
    for (i = 0; i < pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(pipeline->m_elemento[i].m_widget))) {
	    propiedades(i, GTK_FIXED(fixed), pipeline, &modificado);
	}
    }
    if (modificado) {
	gtk_widget_set_sensitive(lookup_widget(window1, "nuevo2"), TRUE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar2"), TRUE);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar_como2"),
				 TRUE);
    }
}

void
on___ndice_de_la_ayuda1_activate(GtkMenuItem * menuitem,
				 gpointer user_data)
{
  int i;
  for (i = 0; i < pipeline->m_numero; ++i) {
    if (gtk_toggle_button_get_active
	(GTK_TOGGLE_BUTTON(pipeline->m_elemento[i].m_widget))) {
      if (pipeline->m_elemento[i].m_funcion_ciclo) {
	pipeline->m_elemento[i].m_funcion_ciclo();
      }
    }
  }
}

void on_button6_activate(GtkButton * button, gpointer user_data)
{  
  gchar* file = abrir_ventana(window1);
  /*GtkWidget *modulo = lookup_widget(dialog, "entry5");
    gtk_entry_set_text(GTK_ENTRY(modulo), file); */
}
