/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos Le�n
    \versi�n 0.1
*/

#include "dll_pipeline.h"
#include <gtk/gtk.h>

GtkWidget * ventana;
GtkWidget * texto;

int ciclo () {
  return 0;
}

int set_datos(const void * datos) {
  const gchar * valor = (const gchar *)datos;
  GtkTextIter * iter;
  GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
  gtk_text_buffer_get_iter_at_offset
                                            (buffer,
                                             iter,
                                             -1);
  gtk_text_buffer_insert(buffer,
				   iter,
				   valor,
				   -1);
}

void * get_datos() {
  return 0;
}

int iniciar() {
  ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(ventana),
		       "Salida del pipeline");
  gtk_window_set_default_size(GTK_WINDOW(ventana), 300, 200);
  texto = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(ventana), texto);
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
