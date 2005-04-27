/*! \file salida.c
    \brief Muestra una salida en forma de texto

          \section modulo Módulo
	  El módulo de salida se encarga de crear una ventana (con GTK, el ejecutable que llame a esta librería debe inciar con <code>gtk_main()</code>) que muestra líneas de texto. La ventana, como es usual, sólo se cierra cuando se cierra el módulo.
	  El módulo necesita, además, que esté en el mismo directorio el archivo de Glade "ventana_salida.glade", pues es éste el que define la ventana.
	  \section puertos Puertos
	  El módulo no dispone de puertos de salida, y como entrada tiene:
	  <ul>
	    <li><em>entrada_texto</em>: Recibe un char * que presenta por la ventana. No hace falta añadir el salto de línea, el módulo lo genera solo.
	  </ul>
	  \section argumentos Argumentos
	  Es posible generar una salida estándar en la ventana al inciar, añadiendo a la descripción en XML un parámetro:
	  <ul>
	    <li><em>texto</em>: Imprime el argumento en la ventana al inciarla.
	  </ul>
    \author Carlos León
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define DATADIR g_get_current_dir()
#define PACKAGE ""
#endif

/*! \brief El puerto de entrada, recibe un <code>char *</code> */
#define PUERTO "entrada_texto"

static void salida_imprimir(const char *value, GladeXML *xml) {
  if(value) {
    GtkWidget* texto;
    GString *valor = g_string_new("");
    GtkTextIter iter;
    GtkTextBuffer * buffer;
    g_string_sprintf(valor, "%s\n", (char *)value);
    texto =  glade_xml_get_widget(xml, "txt_salida");    
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer, &iter, valor->str, -1);
    g_string_free(valor, TRUE);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(texto), &iter, 0.0, FALSE, 1.0, 1.0);
  }
}


static void salida_iniciar_aux(gpointer key, gpointer value, gpointer user_data) {
  if(!strcmp(key, "texto")) {
    salida_imprimir((char *)value, (GladeXML *)user_data);
  }
}
static char *salida_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  GladeXML *xml = (GladeXML *)modulo->m_dato;
  if(!strcmp(PUERTO, puerto)) {
    salida_imprimir((char*)dato, xml);
  }

  return 0;
}

static char *salida_iniciar(modulo_t *modulo, GHashTable *argumentos) {

  GString *buffer = g_string_new(DATADIR);
  GladeXML *xml = glade_xml_new(buffer->str, NULL, NULL);
  g_string_append_printf(buffer, "/%s/ventana_salida.glade", PACKAGE);
  g_string_free(buffer, TRUE);
  modulo->m_dato = xml;
  glade_xml_signal_autoconnect(xml);
  g_hash_table_foreach(argumentos, salida_iniciar_aux, xml);
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

