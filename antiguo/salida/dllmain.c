/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos León
    \versión 0.1
*/

#ifdef WIN32
#include "dll_pipeline.h"
#endif
#include <gtk/gtk.h>

GtkWidget * ventana;
GtkWidget * texto;
GtkWidget * scroll;

gboolean
cerrar_ventana(GtkWidget * widget,
			GdkEvent * event, gpointer user_data) {
      gtk_widget_hide(ventana);
			  return TRUE;
}  

int ciclo () {
  return 0;
}

int set_datos(const void * datos) {
  const gchar * valor = (const gchar *)datos;
  if(valor) {
    GtkTextIter iter;
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
     gtk_text_buffer_get_iter_at_offset
                                            (buffer,
                                             &iter,
                                             -1);
    gtk_text_buffer_insert(buffer,
				   &iter,
				   valor,
				   -1);
 }
}

void * get_datos() {
  return 0;
}

int iniciar() {
  ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_show(scroll);
  gtk_window_set_title(GTK_WINDOW(ventana),
		       "Salida del pipeline");
  gtk_window_set_default_size(GTK_WINDOW(ventana), 300, 200);
  texto = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(scroll), texto);
  gtk_container_add(GTK_CONTAINER(ventana), scroll);
  gtk_widget_show(texto);
  g_signal_connect((gpointer) ventana, "delete_event",
		     G_CALLBACK(cerrar_ventana), 0);
		     gtk_widget_show(ventana);
  return 0;
}

int propiedades() {
  gtk_widget_show(ventana);
}

int cerrar() {
  gtk_widget_destroy(ventana);
}


BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

char * error() {
  return 0;
}
