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

#define OPCIONES_LINEA ((GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION >= 6) || (GLIB_MAJOR_VERSION > 2))

typedef struct {
  guint m_timer;
  gboolean m_seguir;
} dato_timer_t;


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
  gboolean seguir = FALSE;
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(data), "pipeline");
  if(p) {
    dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(data), "timer");
    pipeline_ciclo(p);
    seguir = t->m_seguir;
    if(!t->m_seguir) {
      g_timeout_add(t->m_timer, tick, data);
      t->m_seguir = TRUE;
    }      
  }
  return seguir;
}


void abrir_pipe(const char *ruta, GladeXML *xml) {
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(ventana), "pipeline");
  dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(ventana), "timer");
  pipeline_borrar(p);  
  p = pipeline_cargar(ruta, g_get_current_dir(), funcion_error, xml);
  g_object_set_data(G_OBJECT(ventana), "pipeline", p);
  pipeline_iniciar(p);
  g_timeout_add(t->m_timer, tick, ventana);
}

void on_bot_abrir_clicked(GtkButton *button, gpointer user_data) {
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET(button));
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  GtkWidget*  dialogo = gtk_file_chooser_dialog_new("Abrir pipeline...",
						    GTK_WINDOW(ventana),
						    GTK_FILE_CHOOSER_ACTION_OPEN,
						    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
						    NULL);
  if (gtk_dialog_run (GTK_DIALOG (dialogo)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogo));
    abrir_pipe (filename, xml);
    g_free (filename);
  }
  
  gtk_widget_destroy (dialogo);
}


void on_hsc_temporizador_value_changed(GtkRange *range, gpointer user_data){
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET(range));
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  guint timer = (guint)floor(gtk_range_get_value(range));
  dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(ventana), "timer");
  t->m_timer = timer;
  t->m_seguir = FALSE;
}


int main(int argc, char **argv)
{
  dato_timer_t dato_timer = {1000, TRUE};
#if OPCIONES_LINEA
  const GOptionEntry entries[] = {
    { "timer", 't', 0, G_OPTION_ARG_INT, &dato_timer->m_timer, "Establece el intervalo del temporizador generador de ciclos, en milisegundos", "T" },
    { NULL }
  };  
  GOptionContext* contexto = g_option_context_new (" <pipeline> - ejecuta un pipeline definido en un XML válido");
  g_option_context_add_main_entries (contexto, entries, 0);
  g_option_context_add_group (contexto, gtk_get_option_group (TRUE));
  g_option_context_parse (contexto, &argc, &argv, 0);
  g_option_context_set_help_enabled (contexto, TRUE);
#endif

  gtk_init(&argc, &argv);
  glade_init();
  GladeXML *xml = glade_xml_new("ventana_pipeline.glade", NULL, NULL);
  glade_xml_signal_autoconnect(xml);
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  g_object_set_data(G_OBJECT(ventana), "timer", &dato_timer);
  g_object_set_data(G_OBJECT(ventana), "pipeline", 0);
  GtkWidget *hsc_timer = glade_xml_get_widget(xml, "hsc_temporizador");
  gtk_range_set_value(GTK_RANGE(hsc_timer), (gdouble)dato_timer.m_timer);
  if(argc >= 2) {
    abrir_pipe(argv[1], xml);
  }
  gtk_main();
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(ventana), "pipeline");
  pipeline_borrar(p);
  
#if OPCIONES_LINEA
  g_option_context_free(contexto);
#endif 

  return 0;
}


