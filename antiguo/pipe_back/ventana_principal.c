/*! \file ventana_principal.h
    \brief Ventana principal de la aplicación
    \author Carlos León
    \version 0.1
 */

#include "ventana_principal.h"

#include <gdk/gdkkeysyms.h>
#include "dialogo_abrir.h"
#include "dialogo_guardar.h"
#include "dialogo_conectar.h"
#include "propiedades_modulo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <glib.h>


#define get_widget(_x, _y) GTK_TOGGLE_BUTTON(((GtkFixedChild * )g_list_nth_data(( (GtkFixed *)(_x)->fixed1)->children, (_y)))->widget)

#define get_x(_x, _y) (gint)(((GtkFixedChild * )g_list_nth_data(( (GtkFixed *)(_x)->fixed1)->children, (_y)))->x)

#define get_y(_x, _y) (gint)(((GtkFixedChild * )g_list_nth_data(( (GtkFixed *)(_x)->fixed1)->children, (_y)))->y)


gboolean ventana_principal_tick(gpointer data)
{
    ventana_principal_t *ventana = (ventana_principal_t *) data;
    pipeline_ciclo(ventana->pipeline);
    return ventana->m_seguir_timer;
}



int ventana_principal_crear_timer(ventana_principal_t * ventana,
				  guint retardo)
{
    ventana->m_timer =
	g_timeout_add(retardo, ventana_principal_tick, ventana);
    ventana->m_seguir_timer = TRUE;
    return 0;

}

int ventana_principal_parar_timer(ventana_principal_t * ventana)
{
    ventana->m_seguir_timer = FALSE;
    return 0;
}




gint ventana_principal_expose_event(GtkWidget * widget, GdkEvent * event,
				    gpointer data)
{
    ventana_principal_t *ventana_principal = (ventana_principal_t *) data;
    int i, j;

    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	int x_origen = (int) get_x(ventana_principal, i);
	int y_origen = get_y(ventana_principal, i);
	for (j = 0;
	     j <
	     ventana_principal->pipeline->m_elemento[i].
	     m_numero_conexiones; ++j) {
	    int x_destino =
		get_x(ventana_principal, ventana_principal->pipeline->
		      m_elemento[i].m_destino[j]->m_id);
	    int y_destino =
		get_y(ventana_principal, ventana_principal->pipeline->
		      m_elemento[i].m_destino[j]->m_id);
	    gdk_draw_line(ventana_principal->ww, ventana_principal->gc,
			  x_origen, y_origen, x_destino, y_destino);
	}
    }

    return FALSE;
}



int ventana_principal_elegir_modulo(ventana_principal_t * ventana)
{

    dialogo_conectar_t *dialogo_conectar = dialogo_conectar_crear();
    GtkWidget *dialog = dialogo_conectar->dialog1;
    GtkWidget *combo = dialogo_conectar->comboboxentry1;

    int i;
    for (i = 0; i < ventana->pipeline->m_numero; ++i) {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),
				  ventana->pipeline->m_elemento[i].
				  m_nombre);
    }

    int respuesta =
	gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK ?
	respuesta = gtk_combo_box_get_active(GTK_COMBO_BOX(combo)) : -1;

    dialogo_conectar_cerrar(dialogo_conectar);

    return respuesta;
}


void ventana_principal_info(ventana_principal_t * ventana,
			    const gchar * texto)
{

    GtkWidget *dialog =
	gtk_message_dialog_new(GTK_WINDOW(ventana->window1),
			       GTK_DIALOG_MODAL |
			       GTK_DIALOG_DESTROY_WITH_PARENT,
			       GTK_MESSAGE_INFO,
			       GTK_BUTTONS_CLOSE,
			       texto);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void ventana_principal_acerca_de(ventana_principal_t * ventana)
{
    char *buffer =
	"Visi\303\263n por computador - Pipeline\nVersi\303\263n 0.1\nCarlos Le\303\263n, Jorge Mendoza, Diego S\303\241nchez\nSistemas Inform\303\241ticos 2004/2005\nFacultad de Inform\303\241tica (UCM)";
    ventana_principal_info(ventana, buffer);
}

void ventana_principal_mostrar(ventana_principal_t * ventana,
			       const char *info)
{
    char buffer[128];
    sprintf(buffer, "Visi\303\263n por computador");
    if (info) {
	sprintf(buffer, "%s - %s", buffer, info);
    }
    gtk_statusbar_pop(GTK_STATUSBAR(ventana->statusbar1), ventana->id);
    ventana->id =
	gtk_statusbar_push(GTK_STATUSBAR(ventana->statusbar1), 0, buffer);
}

gboolean ventana_principal_confirmacion(ventana_principal_t * ventana,
					const gchar * texto)
{
    GtkWidget *dialog =
	gtk_message_dialog_new(GTK_WINDOW(ventana->window1),
			       GTK_DIALOG_MODAL |
			       GTK_DIALOG_DESTROY_WITH_PARENT,
			       GTK_MESSAGE_QUESTION,
			       GTK_BUTTONS_YES_NO,
			       texto);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result == GTK_RESPONSE_YES;
}

gboolean ventana_principal_salir(ventana_principal_t * ventana)
{
    return ventana->modificado ? ventana_principal_confirmacion(ventana,
								"El pipeline ha sido modificado y no se ha guardado.\n\302\277 Realmente desea salir ?")
	: TRUE;
}


void ventana_principal_guardar_como_aux(ventana_principal_t *
					ventana_principal,
					const char *file)
{
    if (file != 0) {
	pipeline_guardar(ventana_principal->pipeline, file);
	strcpy(ventana_principal->archivo, file);
	ventana_principal->modificado = FALSE;
	ventana_principal_mostrar(ventana_principal,
				  ventana_principal->archivo);
	gtk_widget_set_sensitive(ventana_principal->guardar2, FALSE);
    }
}

char *ventana_principal_guardar_ventana(ventana_principal_t * ventana)
{
    dialogo_guardar_t *dialogo_guardar = dialogo_guardar_crear();
    GtkWidget *dialog = dialogo_guardar->filechooserdialog2;
    char *filename = 0;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    dialogo_guardar_cerrar(dialogo_guardar);
    return filename;
}

void ventana_principal_establecer(ventana_principal_t * ventana_principal)
{
    int i, si = 0, cual;
    gboolean todas_iniciadas = TRUE;
    gboolean todas_paradas = TRUE;
    guint numero =
	g_list_length(gtk_container_get_children
		      (GTK_CONTAINER(ventana_principal->fixed1)));
    for (i = 0; i < numero; ++i) {

	if (gtk_toggle_button_get_active(get_widget(ventana_principal, i))) {

	    si++;
	    cual = i;
	}
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado == 1) {
	    todas_paradas = FALSE;
	}
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado == 0) {
	    todas_iniciadas = FALSE;
	}
    }
    gboolean b = numero > 0;

    gtk_widget_set_sensitive(ventana_principal->establecer_error, b);

    gtk_widget_set_sensitive(ventana_principal->iniciar_todas_biblioteca, b
			     && !todas_iniciadas);
    gtk_widget_set_sensitive(ventana_principal->cerrar_todas_biblioteca, b
			     && !todas_paradas);
    gtk_widget_set_sensitive(ventana_principal->ciclos_biblioteca, b);
    gtk_widget_set_sensitive(ventana_principal->ciclo_biblioteca, b);
    gtk_widget_set_sensitive(ventana_principal->parar_biblioteca, b);

    gtk_widget_set_sensitive(ventana_principal->conectar1, si == 1);
    gtk_widget_set_sensitive(ventana_principal->propiedades1, si == 1);
    gtk_widget_set_sensitive(ventana_principal->propiedades_biblioteca,
			     si == 1);
    gtk_widget_set_sensitive(ventana_principal->iniciar_biblioteca, si == 1
			     && ventana_principal->pipeline->
			     m_elemento[cual].m_iniciado == 0);
    gtk_widget_set_sensitive(ventana_principal->cerrar_biblioteca, si == 1
			     && ventana_principal->pipeline->
			     m_elemento[cual].m_iniciado == 1);

    gtk_widget_set_sensitive(ventana_principal->borrar1, si > 0);
}

void ventana_principal_pinchado(GtkWidget * boton, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
	    guint numero =
	g_list_length(gtk_container_get_children
		      (GTK_CONTAINER(ventana_principal->fixed1)));
  int i;
	for(i = 0; i < numero; ++i) {
    GtkToggleButton *widget = get_widget(ventana_principal, i);
    if(boton != GTK_WIDGET(widget)) {
      gtk_toggle_button_set_active(widget, FALSE);
  }
  }
	ventana_principal_establecer(ventana_principal);
}

void ventana_principal_crear_boton(ventana_principal_t * ventana,
				   const char *nombre, int x, int y)
{
    GtkWidget *boton = gtk_toggle_button_new_with_label(strdup(nombre));
    gtk_widget_show(GTK_WIDGET(boton));
    gtk_fixed_put(GTK_FIXED(ventana->fixed1), GTK_WIDGET(boton), x, y);
    g_signal_connect((gpointer) boton, "released",
		     G_CALLBACK(ventana_principal_pinchado), ventana);
}

void ventana_principal_borrar(ventana_principal_t * ventana_principal)
{
    //gtk_widget_destroy(ventana_principal->window1);
    pipeline_vaciar(ventana_principal->pipeline);
    g_free(ventana_principal);
}


void ventana_principal_borrar_boton(ventana_principal_t * ventana, int id)
{
    gtk_widget_destroy(GTK_WIDGET(get_widget(ventana, id)));
}

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    if (ventana_principal_confirmacion
	(ventana_principal,
	 "\302\277 Realmente desea borrar los m\303\263dulos elegidos ?"))
    {
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    if (gtk_toggle_button_get_active
		(get_widget(ventana_principal, i))) {
		pipeline_borrar(ventana_principal->pipeline, i);
		ventana_principal_borrar_boton(ventana_principal, i);
		i--;
		ventana_principal->modificado = TRUE;
	    }
	}
	ventana_principal_establecer(ventana_principal);
    }
}


void ventana_principal_propiedades(ventana_principal_t * ventana,
				   gint id_elemento)
{
    propiedades_modulo_t *propiedades = propiedades_modulo_crear();
    if (id_elemento != -1) {
	gtk_entry_set_text(GTK_ENTRY(propiedades->entry2),
			   ventana->pipeline->
			   m_elemento[id_elemento].m_nombre);
	gtk_entry_set_text(GTK_ENTRY(propiedades->entry5),
			   ventana->pipeline->
			   m_elemento[id_elemento].m_ruta);
	char buffer[64];
	sprintf(buffer, "%i", get_x(ventana, id_elemento));
	gtk_entry_set_text(GTK_ENTRY(propiedades->entry3), strdup(buffer));
	sprintf(buffer, "%i", get_y(ventana, id_elemento));
	gtk_entry_set_text(GTK_ENTRY(propiedades->entry4), strdup(buffer));
    }
    if (gtk_dialog_run(GTK_DIALOG(propiedades->propiedades_modulo)) ==
	GTK_RESPONSE_OK) {
	ventana->modificado = TRUE;
	if (id_elemento == -1) {
	    pipeline_nuevo(ventana->pipeline,
			   gtk_entry_get_text(GTK_ENTRY
					      (propiedades->entry2)),
			   gtk_entry_get_text(GTK_ENTRY
					      (propiedades->entry5)));
	    ventana_principal_crear_boton(ventana,
					  gtk_entry_get_text(GTK_ENTRY
							     (propiedades->
							      entry2)),
					  atoi(gtk_entry_get_text
					       (GTK_ENTRY
						(propiedades->entry3))),
					  atoi(gtk_entry_get_text
					       (GTK_ENTRY
						(propiedades->entry4))));
	} else {
	    if (strcmp(ventana->pipeline->m_elemento[id_elemento].m_ruta,
		       gtk_entry_get_text(GTK_ENTRY(propiedades->entry5))))
	    {
		strcpy(ventana->pipeline->m_elemento[id_elemento].m_ruta,
		       gtk_entry_get_text(GTK_ENTRY(propiedades->entry5)));
		pipeline_cambiar_biblioteca(&ventana->pipeline->
					    m_elemento[id_elemento]);
	    }
	    strcpy(ventana->pipeline->m_elemento[id_elemento].m_nombre,
		   gtk_entry_get_text(GTK_ENTRY(propiedades->entry2)));
	    gtk_button_set_label(GTK_BUTTON
				 (get_widget(ventana, id_elemento)),
				 strdup(gtk_entry_get_text
					(GTK_ENTRY(propiedades->entry2))));

	    gtk_fixed_move(GTK_FIXED(ventana->fixed1),
			   GTK_WIDGET(get_widget(ventana, id_elemento)),
			   atoi(gtk_entry_get_text
				(GTK_ENTRY(propiedades->entry3))),
			   atoi(gtk_entry_get_text
				(GTK_ENTRY(propiedades->entry4))));
	}
    }
    propiedades_modulo_cerrar(propiedades);
}

void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    ventana_principal_acerca_de(ventana_principal);
}


void on_nuevo2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    if (ventana_principal_confirmacion
	(ventana_principal,
	 "\302\277 Realmente desea eliminar todos los m\303\263dulos del pipeline ?"))
    {
	int i;
	guint numero =
	g_list_length(gtk_container_get_children
		      (GTK_CONTAINER(ventana_principal->fixed1)));
	for (i = 0; i < numero; ++i) {
	    ventana_principal_borrar_boton(ventana_principal, 0);
	}
	pipeline_vaciar(ventana_principal->pipeline);
	ventana_principal->pipeline = pipeline_crear();
	ventana_principal->archivo[0] = '\0';
	ventana_principal_mostrar(ventana_principal, 0);
	ventana_principal->modificado = FALSE;
	gtk_widget_set_sensitive(ventana_principal->nuevo2, FALSE);
	gtk_widget_set_sensitive(ventana_principal->guardar2, FALSE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2, FALSE);
	ventana_principal_establecer(ventana_principal);
    }

}

void ventana_principal_establecer_boton(ventana_principal_t *ventana_principal
                    , gint id) {
    GtkToggleButton *widget = get_widget(ventana_principal, 
                id);
                
    //Pongo tres b�feres porque el duende no me deja con uno solo
    char buffer[64];
    char buffer2[64];
    char buffer3[64];
    sprintf(buffer, "%s", ventana_principal->pipeline->
                      m_elemento[id].m_nombre);
    if(ventana_principal->pipeline->m_error == id) {
          sprintf(buffer2, "! %s", buffer);
   }
   else {
      strcpy(buffer2, buffer);
    }
    if(ventana_principal->pipeline->
                      m_elemento[id].m_iniciado) {
        sprintf(buffer3, "* %s", buffer2);
    }
    else{
      strcpy(buffer3, buffer2);
    }
    
  gtk_button_set_label(GTK_BUTTON(widget), buffer3);
}

void on_establecer_error(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    gint destino = ventana_principal_elegir_modulo(ventana_principal);
    
  int anterior =  ventana_principal->pipeline->m_error; 

    ventana_principal->pipeline->m_error = destino;    
    
   ventana_principal_establecer_boton(ventana_principal, destino);
   
   if(anterior != -1) {
      ventana_principal_establecer_boton(ventana_principal, anterior);
    }
}

void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    gchar *file = dialogo_abrir_ventana(ventana_principal->window1);
    if (file) {
	ventana_principal_abrir(file, ventana_principal);
	g_free(file);
    }
}


void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    pipeline_cerrar_todas_bibliotecas(ventana_principal->pipeline);
    char *file = ventana_principal_guardar_ventana(ventana_principal);
    if (file) {
	ventana_principal_guardar_como_aux(ventana_principal, file);
	g_free(file);
    }
}


void on_guardar2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    char *buffer;
    if (ventana_principal->archivo[0] != '\0') {
	buffer = ventana_principal->archivo;
    } else {
	buffer = ventana_principal_guardar_ventana(ventana_principal);
    }
    ventana_principal_guardar_como_aux(ventana_principal, buffer);

    if (buffer != ventana_principal->archivo) {
	g_free(buffer);
    }
}


void on_salir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    if (ventana_principal_salir(ventana_principal)) {
	gtk_main_quit();
    }
}


void on_pipeline1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}



void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    ventana_principal_propiedades(ventana_principal, -1);
    if (ventana_principal->modificado) {
	gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2, TRUE);

	ventana_principal_establecer(ventana_principal);
    }
}


void on_conectar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    gint destino = ventana_principal_elegir_modulo(ventana_principal);

    if (destino != -1) {
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    if (gtk_toggle_button_get_active
		(get_widget(ventana_principal, i))) {
		if (pipeline_conectar
		    (ventana_principal->pipeline, i, destino) == -1) {
		    ventana_principal_info(ventana_principal,
					   "No se pudo realizar la conexi\303\263n.");
		} else {
		    ventana_principal_expose_event(0, 0,
						   ventana_principal);
		}
	    }
	}
    }

}


void on_ayuda1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

gboolean
on_window1_delete_event(GtkWidget * widget,
			GdkEvent * event, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    return !ventana_principal_salir(ventana_principal);
}

void on_window1_destroy(GtkObject * object, gpointer user_data)
{
    gtk_main_quit();
}



void on_window1_show(GtkWidget * widget, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;

    ventana_principal->id =
	gtk_statusbar_push(GTK_STATUSBAR(ventana_principal->statusbar1), 0,
			   "Visi\303\263n por computador");
    ventana_principal->pipeline = pipeline_crear();

  /** warning */
    ventana_principal->archivo[0] = '\0';

    GtkWidget *fixed = ventana_principal->fixed1;
    ventana_principal->ww = fixed->window;
    ventana_principal->gc = gdk_gc_new(ventana_principal->ww);
    gtk_signal_connect(GTK_OBJECT(fixed), "expose_event",
		       GTK_SIGNAL_FUNC(ventana_principal_expose_event),
		       ventana_principal);
}

void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active(get_widget(ventana_principal, i))) {
	    ventana_principal_propiedades(ventana_principal, i);
	}
    }
    if (ventana_principal->modificado) {
	gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2, TRUE);
    }
}

void
on___ndice_de_la_ayuda1_activate(GtkMenuItem * menuitem,
				 gpointer user_data)
{
}


void on_propiedades_biblioteca_activate(GtkButton * button,
					gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado) {
	    if (gtk_toggle_button_get_active
		(get_widget(ventana_principal, i))) {
		if (ventana_principal->pipeline->m_elemento[i].
		    m_funcion_propiedades) {
		    ventana_principal->pipeline->m_elemento[i].
			m_funcion_propiedades();
		}
	    }
	}
    }

}
void on_ciclos_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    ventana_principal_crear_timer(ventana_principal,
				  ventana_principal->m_retardo);
}

void ventana_principal_inciar_biblioteca(ventana_principal_t * ventana,
                    gint id) {

  pipeline_iniciar(ventana->pipeline,
			     &ventana->pipeline->m_elemento[id]);
   ventana_principal_establecer_boton(ventana, id);
}

void ventana_principal_parar_biblioteca(ventana_principal_t * ventana,
                    gint id) {

  pipeline_parar(ventana->pipeline,
			     &ventana->pipeline->m_elemento[id]);
  ventana_principal_establecer_boton(ventana, id);			     
}

void on_iniciar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active(get_widget(ventana_principal, i))) {
	    ventana_principal_inciar_biblioteca(ventana_principal, i);

	}
    }
    ventana_principal_establecer(ventana_principal);
}

void on_iniciar_todas_biblioteca_activate(GtkButton * button,
					  gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    ventana_principal_inciar_biblioteca(ventana_principal, 
          ventana_principal->pipeline->m_error);
/*    pipeline_iniciar(ventana_principal->pipeline,
		     &ventana_principal->pipeline->
		     m_elemento[ventana_principal->pipeline->m_error]);*/
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (i != ventana_principal->pipeline->m_error) {
	    ventana_principal_inciar_biblioteca(ventana_principal, i);
	}
    }
    ventana_principal_establecer(ventana_principal);
}


void on_parar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    ventana_principal_parar_timer(ventana_principal);
}

void on_cerrar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active(get_widget(ventana_principal, i))) {
    ventana_principal_parar_biblioteca(ventana_principal, i);
/*	    pipeline_parar(ventana_principal->pipeline,
			   &ventana_principal->pipeline->m_elemento[i]);*/
	}
    }
    ventana_principal_establecer(ventana_principal);
}

void on_cerrar_todas_biblioteca_activate(GtkButton * button,
					 gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
          ventana_principal_parar_biblioteca(ventana_principal, i);
	/*pipeline_parar(ventana_principal->pipeline,
		       &ventana_principal->pipeline->m_elemento[i]);*/
    }
    ventana_principal_establecer(ventana_principal);
}

void on_ciclo_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) user_data;
    pipeline_ciclo(ventana_principal->pipeline);
}


void ventana_principal_abrir(char *file,
			     ventana_principal_t * ventana_principal)
{
    if (file != 0) {
	pipeline_vaciar(ventana_principal->pipeline);
	ventana_principal->pipeline = pipeline_cargar(file);

	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    ventana_principal_crear_boton(ventana_principal,
					  ventana_principal->pipeline->
					  m_elemento[i].m_nombre, 10,
					  (30 * i) + 10);
	}
	if (!ventana_principal->pipeline)
	    ventana_principal->pipeline = pipeline_crear();
	strcpy(ventana_principal->archivo, file);
	ventana_principal->modificado = FALSE;
	ventana_principal_mostrar(ventana_principal,
				  ventana_principal->archivo);
	gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2, TRUE);
	ventana_principal_establecer(ventana_principal);
	ventana_principal_expose_event(0, 0,
						   ventana_principal);
    }
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
      ventana_principal_establecer_boton(ventana_principal, i);
    }

}

ventana_principal_t *ventana_principal_crear()
{
    ventana_principal_t *ventana_principal =
	(ventana_principal_t *) malloc(sizeof(ventana_principal_t));
    ventana_principal->accel_group = gtk_accel_group_new();
    ventana_principal->m_retardo = 0;

    ventana_principal->window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(ventana_principal->window1, "window1");
    gtk_window_set_title(GTK_WINDOW(ventana_principal->window1),
			 "Visi\303\263n por computador - Pipeline");
    gtk_window_set_default_size(GTK_WINDOW(ventana_principal->window1),
				500, 400);

#ifndef WIN32
    ventana_principal->window1_icon_pixbuf =
	gdk_pixbuf_new_from_file("pipeline.xpm", 0);
    if (ventana_principal->window1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(ventana_principal->window1),
			    ventana_principal->window1_icon_pixbuf);
	gdk_pixbuf_unref(ventana_principal->window1_icon_pixbuf);
    }
#endif

    ventana_principal->vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(ventana_principal->vbox1, "vbox1");
    gtk_widget_show(ventana_principal->vbox1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->window1),
		      ventana_principal->vbox1);

    ventana_principal->menubar1 = gtk_menu_bar_new();
    gtk_widget_set_name(ventana_principal->menubar1, "menubar1");
    gtk_widget_show(ventana_principal->menubar1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1),
		       ventana_principal->menubar1, FALSE, FALSE, 0);

    ventana_principal->archivo1 =
	gtk_menu_item_new_with_mnemonic("_Archivo");
    gtk_widget_set_name(ventana_principal->archivo1, "archivo1");
    gtk_widget_show(ventana_principal->archivo1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1),
		      ventana_principal->archivo1);

    ventana_principal->archivo1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->archivo1_menu, "archivo1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->archivo1),
			      ventana_principal->archivo1_menu);

    ventana_principal->nuevo2 =
	gtk_image_menu_item_new_with_mnemonic("_Nuevo");
    gtk_widget_set_name(ventana_principal->nuevo2, "nuevo2");
    gtk_widget_show(ventana_principal->nuevo2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->nuevo2);
    gtk_widget_set_sensitive(ventana_principal->nuevo2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->nuevo2, "activate",
			       ventana_principal->accel_group, GDK_n,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image103 =
	gtk_image_new_from_stock("gtk-new", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image103, "image103");
    gtk_widget_show(ventana_principal->image103);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->nuevo2),
				  ventana_principal->image103);

    ventana_principal->abrir2 =
	gtk_image_menu_item_new_with_mnemonic("_Abrir...");
    gtk_widget_set_name(ventana_principal->abrir2, "abrir2");
    gtk_widget_show(ventana_principal->abrir2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->abrir2);
    gtk_widget_add_accelerator(ventana_principal->abrir2, "activate",
			       ventana_principal->accel_group, GDK_a,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image104 =
	gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image104, "image104");
    gtk_widget_show(ventana_principal->image104);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->abrir2),
				  ventana_principal->image104);

    ventana_principal->separador1 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador1, "separador1");
    gtk_widget_show(ventana_principal->separador1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->separador1);
    gtk_widget_set_sensitive(ventana_principal->separador1, FALSE);

    ventana_principal->guardar_como2 =
	gtk_image_menu_item_new_with_mnemonic("Guardar _como...");
    gtk_widget_set_name(ventana_principal->guardar_como2, "guardar_como2");
    gtk_widget_show(ventana_principal->guardar_como2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->guardar_como2);
    gtk_widget_set_sensitive(ventana_principal->guardar_como2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->guardar_como2,
			       "activate", ventana_principal->accel_group,
			       GDK_g, GDK_CONTROL_MASK | GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    ventana_principal->image105 =
	gtk_image_new_from_stock("gtk-save-as", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image105, "image105");
    gtk_widget_show(ventana_principal->image105);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->guardar_como2),
				  ventana_principal->image105);

    ventana_principal->guardar2 =
	gtk_image_menu_item_new_with_mnemonic("_Guardar");
    gtk_widget_set_name(ventana_principal->guardar2, "guardar2");
    gtk_widget_show(ventana_principal->guardar2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->guardar2);
    gtk_widget_set_sensitive(ventana_principal->guardar2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->guardar2, "activate",
			       ventana_principal->accel_group, GDK_g,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image106 =
	gtk_image_new_from_stock("gtk-save", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image106, "image106");
    gtk_widget_show(ventana_principal->image106);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->guardar2),
				  ventana_principal->image106);

    ventana_principal->separador2 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador2, "separador2");
    gtk_widget_show(ventana_principal->separador2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->separador2);
    gtk_widget_set_sensitive(ventana_principal->separador2, FALSE);

    ventana_principal->salir2 =
	gtk_image_menu_item_new_with_mnemonic("_Salir");
    gtk_widget_set_name(ventana_principal->salir2, "salir2");
    gtk_widget_show(ventana_principal->salir2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu),
		      ventana_principal->salir2);
    gtk_widget_add_accelerator(ventana_principal->salir2, "activate",
			       ventana_principal->accel_group, GDK_F4,
			       GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image107 =
	gtk_image_new_from_stock("gtk-quit", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image107, "image107");
    gtk_widget_show(ventana_principal->image107);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->salir2),
				  ventana_principal->image107);

    ventana_principal->pipeline1 =
	gtk_menu_item_new_with_mnemonic("_Pipeline");
    gtk_widget_set_name(ventana_principal->pipeline1, "pipeline1");
    gtk_widget_show(ventana_principal->pipeline1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1),
		      ventana_principal->pipeline1);

    ventana_principal->pipeline1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->pipeline1_menu,
			"pipeline1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->pipeline1),
			      ventana_principal->pipeline1_menu);

    ventana_principal->crear1 =
	gtk_image_menu_item_new_with_mnemonic("_Crear");
    gtk_widget_set_name(ventana_principal->crear1, "crear1");
    gtk_widget_show(ventana_principal->crear1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->crear1);
    gtk_widget_add_accelerator(ventana_principal->crear1, "activate",
			       ventana_principal->accel_group, GDK_Insert,
			       0, GTK_ACCEL_VISIBLE);

    ventana_principal->image108 =
	gtk_image_new_from_stock("gtk-add", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image108, "image108");
    gtk_widget_show(ventana_principal->image108);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->crear1),
				  ventana_principal->image108);

    ventana_principal->propiedades1 =
	gtk_image_menu_item_new_with_mnemonic("_Propiedades...");
    gtk_widget_set_name(ventana_principal->propiedades1, "propiedades1");
    gtk_widget_show(ventana_principal->propiedades1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->propiedades1);
    gtk_widget_set_sensitive(ventana_principal->propiedades1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->propiedades1, "activate",
			       ventana_principal->accel_group, GDK_p,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image109 =
	gtk_image_new_from_stock("gtk-preferences", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image109, "image109");
    gtk_widget_show(ventana_principal->image109);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->propiedades1),
				  ventana_principal->image109);


    ventana_principal->separador5 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador5, "separador3");
    gtk_widget_show(ventana_principal->separador5);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->separador5);
    gtk_widget_set_sensitive(ventana_principal->separador5, FALSE);

    ventana_principal->propiedades_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Info...");
    gtk_widget_set_name(ventana_principal->propiedades_biblioteca,
			"propiedades_biblioteca");
    gtk_widget_show(ventana_principal->propiedades_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->propiedades_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->propiedades_biblioteca,
			     FALSE);
    gtk_widget_add_accelerator(ventana_principal->propiedades_biblioteca,
			       "activate", ventana_principal->accel_group,
			       GDK_f, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->ciclos_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Arrancar pipeline...");
    gtk_widget_set_name(ventana_principal->ciclos_biblioteca,
			"ciclos_biblioteca");
    gtk_widget_show(ventana_principal->ciclos_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->ciclos_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->ciclos_biblioteca, FALSE);
    
    
    gtk_widget_add_accelerator(ventana_principal->ciclos_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_s,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->ciclo_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Un ciclo");
    gtk_widget_set_name(ventana_principal->ciclo_biblioteca,
			"ciclo_biblioteca");
    gtk_widget_show(ventana_principal->ciclo_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->ciclo_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->ciclo_biblioteca, FALSE);
    gtk_widget_add_accelerator(ventana_principal->ciclo_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_c,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->parar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Parar pipeline");
    gtk_widget_set_name(ventana_principal->parar_biblioteca,
			"parar_biblioteca");
    gtk_widget_show(ventana_principal->parar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->parar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->parar_biblioteca, FALSE);
gtk_widget_add_accelerator(ventana_principal->parar_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_t,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->establecer_error =
	gtk_image_menu_item_new_with_mnemonic
	("_Establecer m\303\263dulo de error...");
    gtk_widget_set_name(ventana_principal->ciclos_biblioteca,
			"establecer_error");
    gtk_widget_show(ventana_principal->establecer_error);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->establecer_error);
    gtk_widget_set_sensitive(ventana_principal->establecer_error, FALSE);
gtk_widget_add_accelerator(ventana_principal->establecer_error, "activate",
			       ventana_principal->accel_group, GDK_e,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);





    ventana_principal->iniciar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("Inicia_r m\303\263dulo...");
    gtk_widget_set_name(ventana_principal->iniciar_biblioteca,
			"iniciar_biblioteca");
    gtk_widget_show(ventana_principal->iniciar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->iniciar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->iniciar_biblioteca, FALSE);
    gtk_widget_add_accelerator(ventana_principal->iniciar_biblioteca,
			       "activate", ventana_principal->accel_group,
			       GDK_m, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->iniciar_todas_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("Iniciar _todos...");
    gtk_widget_set_name(ventana_principal->iniciar_todas_biblioteca,
			"iniciar_todas_biblioteca");
    gtk_widget_show(ventana_principal->iniciar_todas_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->iniciar_todas_biblioteca);
		      gtk_widget_add_accelerator(ventana_principal->iniciar_todas_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_d,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_set_sensitive(ventana_principal->iniciar_todas_biblioteca,
			     FALSE);

    ventana_principal->cerrar_todas_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("C_errar todos...");
    gtk_widget_set_name(ventana_principal->cerrar_todas_biblioteca,
			"cerrar_todas_biblioteca");
    gtk_widget_show(ventana_principal->cerrar_todas_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->cerrar_todas_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->cerrar_todas_biblioteca,
			     FALSE);
			     gtk_widget_add_accelerator(ventana_principal->cerrar_todas_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_r,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->cerrar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Cerrar m\303\263dulo...");
    gtk_widget_set_name(ventana_principal->cerrar_biblioteca,
			"cerrar_biblioteca");
    gtk_widget_show(ventana_principal->cerrar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->cerrar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->cerrar_biblioteca, FALSE);
gtk_widget_add_accelerator(ventana_principal->cerrar_biblioteca, "activate",
			       ventana_principal->accel_group, GDK_l,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);












    ventana_principal->conectar1 =
	gtk_image_menu_item_new_with_mnemonic("C_onectar...");
    gtk_widget_set_name(ventana_principal->conectar1, "conectar1");
    gtk_widget_show(ventana_principal->conectar1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->conectar1);
    gtk_widget_set_sensitive(ventana_principal->conectar1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->conectar1, "activate",
			       ventana_principal->accel_group, GDK_space,
			       0, GTK_ACCEL_VISIBLE);

    ventana_principal->image110 =
	gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image110, "image110");
    gtk_widget_show(ventana_principal->image110);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->conectar1),
				  ventana_principal->image110);

    ventana_principal->separador3 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador3, "separador3");
    gtk_widget_show(ventana_principal->separador3);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->separador3);
    gtk_widget_set_sensitive(ventana_principal->separador3, FALSE);

    ventana_principal->borrar1 =
	gtk_image_menu_item_new_with_mnemonic("_Borrar");
    gtk_widget_set_name(ventana_principal->borrar1, "borrar1");
    gtk_widget_show(ventana_principal->borrar1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->borrar1);
    gtk_widget_set_sensitive(ventana_principal->borrar1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->borrar1, "activate",
			       ventana_principal->accel_group, GDK_Delete,
			       0, GTK_ACCEL_VISIBLE);

    ventana_principal->image111 =
	gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image111, "image111");
    gtk_widget_show(ventana_principal->image111);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->borrar1),
				  ventana_principal->image111);

    ventana_principal->ayuda1 = gtk_menu_item_new_with_mnemonic("A_yuda");
    gtk_widget_set_name(ventana_principal->ayuda1, "ayuda1");
    gtk_widget_show(ventana_principal->ayuda1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1),
		      ventana_principal->ayuda1);

    ventana_principal->ayuda1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->ayuda1_menu, "ayuda1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->ayuda1),
			      ventana_principal->ayuda1_menu);

    ventana_principal->__ndice_de_la_ayuda1 =
	gtk_image_menu_item_new_with_mnemonic("\303\215ndice de la ayuda");
    gtk_widget_set_name(ventana_principal->__ndice_de_la_ayuda1,
			"__ndice_de_la_ayuda1");
    gtk_widget_show(ventana_principal->__ndice_de_la_ayuda1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu),
		      ventana_principal->__ndice_de_la_ayuda1);
    gtk_widget_add_accelerator(ventana_principal->__ndice_de_la_ayuda1,
			       "activate", ventana_principal->accel_group,
			       GDK_F1, 0, GTK_ACCEL_VISIBLE);

    ventana_principal->image112 =
	gtk_image_new_from_stock("gtk-index", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image112, "image112");
    gtk_widget_show(ventana_principal->image112);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->
				   __ndice_de_la_ayuda1),
				  ventana_principal->image112);

    ventana_principal->separador4 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador4, "separador4");
    gtk_widget_show(ventana_principal->separador4);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu),
		      ventana_principal->separador4);
    gtk_widget_set_sensitive(ventana_principal->separador4, FALSE);

    ventana_principal->acerca_de1 =
	gtk_image_menu_item_new_with_mnemonic("_Acerca de...");
    gtk_widget_set_name(ventana_principal->acerca_de1, "acerca_de1");
    gtk_widget_show(ventana_principal->acerca_de1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu),
		      ventana_principal->acerca_de1);
    gtk_widget_add_accelerator(ventana_principal->acerca_de1, "activate",
			       ventana_principal->accel_group, GDK_i,
			       GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image113 =
	gtk_image_new_from_stock("gtk-dialog-info", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image113, "image113");
    gtk_widget_show(ventana_principal->image113);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->acerca_de1),
				  ventana_principal->image113);

    ventana_principal->scrolledwindow1 =
	gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(ventana_principal->scrolledwindow1,
			"scrolledwindow1");
    gtk_widget_show(ventana_principal->scrolledwindow1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1),
		       ventana_principal->scrolledwindow1, TRUE, TRUE, 0);

    ventana_principal->viewport1 = gtk_viewport_new(NULL, NULL);
    gtk_widget_set_name(ventana_principal->viewport1, "viewport1");
    gtk_widget_show(ventana_principal->viewport1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->scrolledwindow1),
		      ventana_principal->viewport1);

    ventana_principal->fixed1 = gtk_fixed_new();
    gtk_widget_set_name(ventana_principal->fixed1, "fixed1");
    gtk_widget_show(ventana_principal->fixed1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->viewport1),
		      ventana_principal->fixed1);

    ventana_principal->statusbar1 = gtk_statusbar_new();
    gtk_widget_set_name(ventana_principal->statusbar1, "statusbar1");
    gtk_widget_show(ventana_principal->statusbar1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1),
		       ventana_principal->statusbar1, FALSE, FALSE, 0);

    g_signal_connect((gpointer) ventana_principal->window1, "delete_event",
		     G_CALLBACK(on_window1_delete_event),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->window1, "destroy",
		     G_CALLBACK(on_window1_destroy), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->window1, "show",
		     G_CALLBACK(on_window1_show), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->nuevo2, "activate",
		     G_CALLBACK(on_nuevo2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->abrir2, "activate",
		     G_CALLBACK(on_abrir2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->guardar_como2,
		     "activate", G_CALLBACK(on_guardar_como2_activate),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->guardar2, "activate",
		     G_CALLBACK(on_guardar2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->salir2, "activate",
		     G_CALLBACK(on_salir2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->pipeline1, "activate",
		     G_CALLBACK(on_pipeline1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->crear1, "activate",
		     G_CALLBACK(on_crear1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->propiedades1,
		     "activate", G_CALLBACK(on_propiedades1_activate),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->conectar1, "activate",
		     G_CALLBACK(on_conectar1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->establecer_error,
		     "activate", G_CALLBACK(on_establecer_error),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->borrar1, "activate",
		     G_CALLBACK(on_borrar1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->ayuda1, "activate",
		     G_CALLBACK(on_ayuda1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->__ndice_de_la_ayuda1,
		     "activate",
		     G_CALLBACK(on___ndice_de_la_ayuda1_activate),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->acerca_de1, "activate",
		     G_CALLBACK(on_acerca_de1_activate),
		     ventana_principal);
    g_signal_connect((gpointer) ventana_principal->propiedades_biblioteca,
		     "activate",
		     G_CALLBACK(on_propiedades_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->ciclo_biblioteca,
		     "activate", G_CALLBACK(on_ciclo_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->ciclos_biblioteca,
		     "activate", G_CALLBACK(on_ciclos_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->parar_biblioteca,
		     "activate", G_CALLBACK(on_parar_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->iniciar_biblioteca,
		     "activate",
		     G_CALLBACK(on_iniciar_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->
		     iniciar_todas_biblioteca, "activate",
		     G_CALLBACK(on_iniciar_todas_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->cerrar_biblioteca,
		     "activate", G_CALLBACK(on_cerrar_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->cerrar_todas_biblioteca,
		     "activate",
		     G_CALLBACK(on_cerrar_todas_biblioteca_activate),
		     ventana_principal);

    ventana_principal->modificado = FALSE;

    gtk_window_add_accel_group(GTK_WINDOW(ventana_principal->window1),
			       ventana_principal->accel_group);

    return ventana_principal;
}
