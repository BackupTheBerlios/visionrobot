/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include "salida_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>

static GladeXML* xml = 0;

static void salida_imprimir(const char *value) {
  GString *valor = g_string_new("");
  g_string_sprintf(valor, "%s\n", (char *)value);
  GtkWidget* texto =  glade_xml_get_widget(xml, "txt_salida");
  GtkTextIter iter;
  GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
  gtk_text_buffer_insert(buffer, &iter, valor->str, -1);
  g_string_free(valor, TRUE);
  gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(texto), &iter, 0.0, FALSE, 1.0, 1.0);
}


static void salida_iniciar_aux(gpointer key, gpointer value, gpointer user_data) {
  salida_imprimir((char *)value);
}

static void salida_ciclo_aux(gpointer key, gpointer value, gpointer user_data) {
  if(key == PIPELINE_SALIDA) {
    salida_imprimir((const char*) value);
  }
}

static char *salida_ciclo(modulo_t *modulo, char tipo, GHashTable *lista)//, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  g_hash_table_foreach(lista, salida_ciclo_aux, 0);
  return 0;
}

static char *salida_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  xml = glade_xml_new("ventana_salida.glade", NULL, NULL);
  glade_xml_signal_autoconnect(xml);
  g_hash_table_foreach(argumentos, salida_iniciar_aux, 0);
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
  modulo->m_tipo = PIPELINE_SALIDA;
  modulo->m_nombre = "Salida";
  modulo->m_iniciar = salida_iniciar;
  modulo->m_cerrar = salida_cerrar;
  modulo->m_ciclo = salida_ciclo;
  modulo->m_dato = 0;
  return modulo;

}

