/*! \file callbacks.h
    \brief Cabeceras de las funciones de eventos de Gtk
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

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_nuevo2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_guardar2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_salir2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_pipeline1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_conectar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_ayuda1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data);

gboolean
on_window1_destroy_event(GtkWidget * widget,
			 GdkEvent * event, gpointer user_data);

gboolean
on_window1_delete_event(GtkWidget * widget,
			GdkEvent * event, gpointer user_data);

void on_window1_destroy(GtkObject * object, gpointer user_data);

void on_window1_show(GtkWidget * widget, gpointer user_data);

void
on_fixed1_drag_begin(GtkWidget * widget,
		     GdkDragContext * drag_context, gpointer user_data);

gboolean
on_fixed1_drag_motion(GtkWidget * widget,
		      GdkDragContext * drag_context,
		      gint x, gint y, guint time, gpointer user_data);

void
on_fixed1_drag_leave(GtkWidget * widget,
		     GdkDragContext * drag_context,
		     guint time, gpointer user_data);

void
on_fixed1_drag_end(GtkWidget * widget,
		   GdkDragContext * drag_context, gpointer user_data);

void
on_button6_drag_begin(GtkWidget * widget,
		      GdkDragContext * drag_context, gpointer user_data);

gboolean
on_button6_drag_motion(GtkWidget * widget,
		       GdkDragContext * drag_context,
		       gint x, gint y, guint time, gpointer user_data);

void
on_button6_drag_end(GtkWidget * widget,
		    GdkDragContext * drag_context, gpointer user_data);

void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data);

void
on___ndice_de_la_ayuda1_activate(GtkMenuItem * menuitem,
				 gpointer user_data);

void pinchado(GtkMenuItem * menuitem, gpointer user_data);

void on_button6_activate(GtkButton * button, gpointer user_data);
