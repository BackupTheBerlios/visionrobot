#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <string.h>

static GladeXML* xml = 0;

void funcion_error(const char *nombre, const char *textos) {
  if(xml && nombre && textos) {
    GString *valor = g_string_new("");
    g_string_sprintf(valor, "%s: %s\n", strdup(nombre), strdup(textos));
    GtkWidget* texto =  glade_xml_get_widget(xml, "txt_error");
    GtkTextIter iter;
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer, &iter, valor->str, -1);
    g_string_free(valor, TRUE);
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
  if (argc < 2) {
    printf("Faltan argumentos. Uso: %s <nombre_de_archivo>.\n", argv[0]);
    return -1;
  }  
  else {
    gtk_init(&argc, &argv);
    glade_init();
    xml = glade_xml_new("ventana_pipeline.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);
    pipeline_t * p = pipeline_cargar(argv[1], funcion_error, g_get_current_dir());
    g_timeout_add(300, tick, p);
    pipeline_iniciar(p);	
    gtk_main();
    pipeline_cerrar(p);
    pipeline_borrar(p);
    return 0;
  }
}


