/*!
  \file   main.c
  \author Carlos León
  \date   Sun Mar 13 22:13:36 2005
  
  \brief  Programa principal de "Visión por computador"
  
  
*/
#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>

#define OPCIONES_LINEA ((GLIB_MAJOR_VERSION==2 && GLIB_MINOR_VERSION >= 6) || (GLIB_MAJOR_VERSION>2))

void funcion_error(const char *nombre, const char *modulo, const char *textos, void *dato) {
  GladeXML  *xml = (GladeXML *)dato;
  if(xml && nombre && textos && modulo) {
    GString *valor = g_string_new("");
    g_string_sprintf(valor, "%s [%s]: %s\n", strdup(nombre), strdup(modulo), strdup(textos));
    // Sacar la salida por pantalla hace fácil redirigir el resultado de depuración
    // a un archivo de texto: pipeline --timer=500 vision.xml > vision.log
    printf("%s", valor->str);
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
    pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(data), "pipeline");
    pipeline_ciclo(p);
    return TRUE;
}

void on_hsc_tolerancia_rojo_value_changed(GtkRange *range, gpointer user_data){
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET(range));
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  g_timeout_add((int)floor(gtk_range_get_value(range)), tick, ventana);
}


int main(int argc, char **argv)
{
  int valor;
  guint timer = 1000;

#if OPCIONES_LINEA
  const GOptionEntry entries[] = {
    { "timer", 't', 0, G_OPTION_ARG_INT, &timer, "Establece el intervalo del temporizador generador de ciclos, en milisegundos", "T" },
    { NULL }
  };  
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
    GladeXML *xml = glade_xml_new("ventana_pipeline.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);
    GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
    pipeline_t * p = pipeline_cargar(argv[1], g_get_current_dir(), funcion_error, xml);
    g_object_set_data(G_OBJECT(ventana), "pipeline", p);
    GtkWidget *hsc_timer = glade_xml_get_widget(xml, "hsc_temporizador");
    gtk_range_set_value(GTK_RANGE(hsc_timer), (gdouble)timer);
    if(p) {
      g_timeout_add(timer, tick, ventana);
      pipeline_iniciar(p);	
      gtk_main();
      pipeline_borrar(p);
      valor = 0;
    }
    else {
      valor = -1;
    }
  }

#if OPCIONES_LINEA
  g_option_context_free(contexto);
#endif 

  return valor;
}


