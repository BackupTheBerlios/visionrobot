/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>

static GladeXML* xml = 0;


static void salida_imprimir(gpointer key, gpointer value, gpointer user_data) {
  char *valor = (char *)value;
  GtkWidget* texto =  glade_xml_get_widget(xml, "txt_salida");
  GtkTextIter iter;
  GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
  gtk_text_buffer_insert(buffer, &iter, valor, -1);
}

static char *salida_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  salida_imprimir(0, (gpointer)in, 0);
  return 0;
}


static char *salida_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  xml = glade_xml_new("ventana_salida.glade", NULL, NULL);
  glade_xml_signal_autoconnect(xml);
  g_hash_table_foreach(argumentos, salida_imprimir, 0);
  return "iniciado";
}
static char *salida_cerrar(modulo_t *modulo)
{
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Salida";
  modulo->m_iniciar = salida_iniciar;
  modulo->m_cerrar = salida_cerrar;
  modulo->m_ciclo = salida_ciclo;
  modulo->m_dato = 0;
  return modulo;

}

