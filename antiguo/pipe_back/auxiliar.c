/*! \file auxiliar.c
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
  *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
  */
#include "auxiliar.h"
#include "interface.h"
#include "support.h"
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

gboolean confirmacion(GtkWidget * w, const gchar * texto)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(w),
					       GTK_DIALOG_MODAL |
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_QUESTION,
					       GTK_BUTTONS_YES_NO,
					       texto);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result == GTK_RESPONSE_YES;
}

gboolean salir(GtkWidget * w, gboolean modificado)
{
    return modificado ? confirmacion(w,
				     "El pipeline ha sido modificado y no se ha guardado.\n\302\277 Realmente desea salir ?")
	: TRUE;
}
void info(GtkWidget * w, const gchar * texto)
{

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(w),
					       GTK_DIALOG_MODAL |
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_INFO,
					       GTK_BUTTONS_CLOSE,
					       texto);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void acerca_de(GtkWidget * w)
{
    char *buffer =
	"Visi\303\263n por computador - Pipeline\nVersi\303\263n 0.1\nCarlos Le\303\263n, Jorge Mendoza, Diego S\303\241nchez\nSistemas Inform\303\241ticos 2004/2005\nFacultad de Inform\303\241tica (UCM)";
    info(w, buffer);
}

char *abrir_ventana(GtkWidget * w)
{
    GtkWidget *dialog = create_filechooserdialog1();
    char *filename = 0;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_CANCEL) {
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    gtk_widget_destroy(dialog);
    return filename;
}

char *guardar_ventana(GtkWidget * w)
{
    GtkWidget *dialog = create_filechooserdialog2();
    char *filename = 0;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_CANCEL) {
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    gtk_widget_destroy(dialog);
    return filename;
}

gchar *entrada(GtkWidget * w, const gchar * pregunta)
{
    GtkWidget *d = create_dialog1();
    gchar *respuesta = 0;
    GtkWidget *label = lookup_widget(d, "label3");
    gtk_label_set_text(GTK_LABEL(label), pregunta);
    gchar *res = 0;
    if (gtk_dialog_run(GTK_DIALOG(d)) == GTK_RESPONSE_OK) {
	GtkWidget *entry = lookup_widget(d, "entry1");
	respuesta = (gchar *) gtk_entry_get_text(GTK_ENTRY(entry));
	res = strdup(respuesta);
    }

    gtk_widget_destroy(d);
    return res;
}

void insertar(GtkFixed * fixed, pipeline_t * pipeline)
{
    int id;
    for (id = 0; id < pipeline->m_numero; ++id) {
	gtk_fixed_put(GTK_FIXED(fixed),
		      GTK_WIDGET(pipeline->m_elemento[id].m_widget),
		      pipeline->m_elemento[id].m_x,
		      pipeline->m_elemento[id].m_y);
    }
}

void propiedades(gint id_elemento, GtkFixed * fixed, pipeline_t * pipeline,
		 gboolean * mod)
{
    GtkWidget *dialog = create_propiedades_modulo();
    GtkWidget *nombre = lookup_widget(dialog, "entry2");
    GtkWidget *x = lookup_widget(dialog, "entry3");
    GtkWidget *y = lookup_widget(dialog, "entry4");
    GtkWidget *modulo = lookup_widget(dialog, "entry5");
    gchar buffer[8];
    if (id_elemento != -1) {
	gtk_entry_set_text(GTK_ENTRY(nombre),
			   pipeline->m_elemento[id_elemento].m_nombre);
	gtk_entry_set_text(GTK_ENTRY(modulo),
			   pipeline->m_elemento[id_elemento].m_ruta);
	sprintf(buffer, "%i", pipeline->m_elemento[id_elemento].m_x);
	gtk_entry_set_text(GTK_ENTRY(x), buffer);
	sprintf(buffer, "%i", pipeline->m_elemento[id_elemento].m_y);
	gtk_entry_set_text(GTK_ENTRY(y), buffer);
    }
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	*mod = TRUE;
	gchar *nombre_v = (gchar *) gtk_entry_get_text(GTK_ENTRY(nombre));
	gchar *ruta_v = (gchar *) gtk_entry_get_text(GTK_ENTRY(modulo));
	gint x_v = atoi((gchar *) gtk_entry_get_text(GTK_ENTRY(x)));
	gint y_v = atoi((gchar *) gtk_entry_get_text(GTK_ENTRY(y)));

	if (fixed != NULL) {
	    if (id_elemento == -1) {
		elemento_t *elemento =
		    nuevo(pipeline, nombre_v, x_v, y_v, ruta_v);
		gtk_fixed_put(GTK_FIXED(fixed),
			      GTK_WIDGET(elemento->m_widget),
			      elemento->m_x, elemento->m_y);
	    } else {
		pipeline->m_elemento[id_elemento].m_x = x_v;
		pipeline->m_elemento[id_elemento].m_y = y_v;
		strcpy(pipeline->m_elemento[id_elemento].m_ruta, ruta_v);
		strcpy(pipeline->m_elemento[id_elemento].m_nombre,
		       nombre_v);
		gtk_button_set_label(GTK_BUTTON
				     (pipeline->m_elemento[id_elemento].
				      m_widget),
				     pipeline->m_elemento[id_elemento].
				     m_nombre);
		gtk_fixed_move(GTK_FIXED(fixed),
			       GTK_WIDGET(pipeline->
					  m_elemento[id_elemento].
					  m_widget),
			       pipeline->m_elemento[id_elemento].m_x,
			       pipeline->m_elemento[id_elemento].m_y);
		cambiar_biblioteca(pipeline, id_elemento);
	    }
	}
    } else {
	*mod = FALSE;
    }
    gtk_widget_destroy(dialog);
}
void mostrar(GtkStatusbar * b, const char *info, guint * id)
{
    char buffer[128];
    sprintf(buffer, "Visi\303\263n por computador");
    if (info) {
	sprintf(buffer, "%s - %s", buffer, info);
    }
    gtk_statusbar_pop(GTK_STATUSBAR(b), *id);
    *id = gtk_statusbar_push(GTK_STATUSBAR(b), 0, buffer);
}


void establecer(pipeline_t * pipeline, GtkWidget * window1)
{
    int i, si = 0, cual;
    gboolean todas_iniciadas = TRUE;
    gboolean todas_paradas = TRUE;
    for (i = 0; i < pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(pipeline->m_elemento[i].m_widget))) {
	    si++;
	    cual = i;
	}
	if (pipeline->m_elemento[i].m_iniciado == 1) {
	    todas_paradas = FALSE;
	}
	if (pipeline->m_elemento[i].m_iniciado == 0) {
	    todas_iniciadas = FALSE;
	}
    }
    gboolean b = pipeline->m_numero > 0;
    gtk_widget_set_sensitive(lookup_widget
			     (window1, "iniciar_todas_biblioteca"), b
			     && !todas_iniciadas);
    gtk_widget_set_sensitive(lookup_widget
			     (window1, "cerrar_todas_biblioteca"), b
			     && !todas_paradas);
    gtk_widget_set_sensitive(lookup_widget(window1, "ciclos_biblioteca"),
			     b);
    gtk_widget_set_sensitive(lookup_widget(window1, "ciclo_biblioteca"),
			     b);
    gtk_widget_set_sensitive(lookup_widget(window1, "parar_biblioteca"),
			     b);

    gtk_widget_set_sensitive(lookup_widget(window1, "conectar1"), si == 1);
    gtk_widget_set_sensitive(lookup_widget(window1, "propiedades1"),
			     si == 1);
    gtk_widget_set_sensitive(lookup_widget
			     (window1, "propiedades_biblioteca"), si == 1);
    gtk_widget_set_sensitive(lookup_widget(window1, "iniciar_biblioteca"),
			     si == 1
			     && pipeline->m_elemento[cual].m_iniciado ==
			     0);
    gtk_widget_set_sensitive(lookup_widget(window1, "cerrar_biblioteca"),
			     si == 1
			     && pipeline->m_elemento[cual].m_iniciado ==
			     1);

    gtk_widget_set_sensitive(lookup_widget(window1, "borrar1"), si > 0);
}

void guardar_como_aux(pipeline_t * pipeline, char *file,
		      GtkWidget * window1, GtkWidget * status_bar,
		      char *archivo, guint * id, gboolean * modificado)
{
    if (file != 0) {
	guardar(pipeline, file);
	strcpy(archivo, file);
	modificado = FALSE;
	mostrar(GTK_STATUSBAR(status_bar), archivo, id);
	gtk_widget_set_sensitive(lookup_widget(window1, "guardar2"),
				 FALSE);
    }
}

int parar(elemento_t * elemento) {
  char buffer[MAX_NOMBRE];
  elemento->m_iniciado = 0;
  sprintf(buffer, "%s", elemento->m_nombre);
  gtk_button_set_label(GTK_BUTTON
		       (elemento->m_widget),
		       buffer);
  if(elemento->m_funcion_cerrar) {
    elemento->m_funcion_cerrar();
    return 0;
  }
  return -1;
}

int iniciar(elemento_t * elemento) {
  char buffer[MAX_NOMBRE];
  elemento->m_iniciado = 1;
  sprintf(buffer, "* %s", elemento->m_nombre);
  gtk_button_set_label(GTK_BUTTON
		       (elemento->m_widget),
		       buffer);
  if(elemento->m_funcion_iniciar) {
    elemento->m_funcion_iniciar();
    return 0;
  }
  return -1;
}

int crear_timer() {
  struct itimerval t;
  t.it_interval.tv_usec = 1;
  t.it_interval.tv_sec = 99999;
  t.it_value.tv_usec = 99999;
  t.it_value.tv_sec = 1;
  setitimer(ITIMER_REAL, &t, 0);
  return 0;
}

int parar_timer() {
  struct itimerval t;
  t.it_interval.tv_usec = 0;
  t.it_interval.tv_sec = 0;
  t.it_value.tv_usec = 0;
  t.it_value.tv_sec = 0;
  setitimer(ITIMER_REAL, &t, 0);
  return 0;
}

void
catch_alarm (int sig) {
  g_print("jajajaj, funciona!!!!");
  signal (sig, catch_alarm);
}

int senyal() {
  signal (SIGALRM, catch_alarm);
  return 0;
}
