#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <string.h>

static GladeXML* xml = 0;
static guint timer = 1000;

#ifdef GTK
#define OPCIONES_LINEA
#endif

#ifdef OPCIONES_LINEA
static GOptionEntry entries[] = {
  { "timer", 't', 0, G_OPTION_ARG_INT, &timer, "Establece el intervalo del temporizador generador de ciclos, en milisegundos", "T" },
  { NULL }
};
#endif


void funcion_error(const char *nombre, const char *textos) {
  if(xml && nombre && textos) {
    GString *valor = g_string_new("");
    g_string_sprintf(valor, "%s: %s\n", strdup(nombre), strdup(textos));
    printf("%s: %s\n", nombre, textos);
    GtkWidget* texto =  glade_xml_get_widget(xml, "txt_error");
    GtkTextIter iter;
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer, &iter, valor->str, -1);
    g_string_free(valor, TRUE);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(texto), &iter, 0.0, FALSE, 1.0, 1.0);
  }
}

gboolean tick(gpointer data)
{
    pipeline_t *p = (pipeline_t *)data;
    pipeline_ciclo(p);
    return TRUE;
}

int main(int argc, char **argv)
{
  int valor;

#ifdef OPCIONES_LINEA
  GOptionContext* contexto = g_option_context_new (" <pipeline> - ejecuta un pipeline definido en un XML válido");
  g_option_context_add_main_entries (contexto, entries, 0);
  g_option_context_add_group (contexto, gtk_get_option_group (TRUE));
  g_option_context_parse (contexto, &argc, &argv, 0);
  g_option_context_set_help_enabled (contexto, TRUE);
#endif

  if (argc < 2) {
    printf("Faltan argumentos. Uso: %s <pipeline>.\n", argv[0]);
    return -1;
  }  
  else {
    gtk_init(&argc, &argv);
    glade_init();
    xml = glade_xml_new("ventana_pipeline.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);
    pipeline_t * p = pipeline_cargar(argv[1], g_get_current_dir(), funcion_error);
    if(p) {
      g_timeout_add(timer, tick, p);
      pipeline_iniciar(p);	
      gtk_main();
      pipeline_borrar(p);
      valor = 0;
    }
    else {
      valor = -1;
    }
  }

#ifdef OPCIONES_LINEA
  g_option_context_free(contexto);
#endif 

  return valor;
}


