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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define LIBDIR g_get_current_dir()
#define DATADIR g_get_current_dir()
#define PACKAGE
#endif

#include "pipeline.xpm"

//! Macro que verifica la versión de GLib que estamos usando. Para que tengamos acceso a las funciones de tratamiento de argumentos desde la línea de comandos, la versión Glib debe ser 2.6 o posterior.
#define OPCIONES_LINEA ((GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION >= 6) || (GLIB_MAJOR_VERSION > 2))

//! La estructura de datos que define un temporizador.
typedef struct {
  guint m_timer;		/*!< El número de milisegundos entre llamadas. */
  guint m_timer_id;		/*!< El "id" del temporizador, para poder gestionarlo con facilidad. */
  gboolean m_iniciado;		/*!< Si se ha iniciado ya el temporizador o no. */
} dato_timer_t;

//! Enumeración para los campos de la lista de elementos del pipeline.
enum {
  ACTIVO,			/*!< Columna de checboxes. */
  NOMBRE,			/*!< Columna del nombre. */
  NUMERO			/*!< Número de columnas. */
};

//! Función de retrollamada para el pipeline.
/*! Función implementada según la interfaz del pipeline.
  
\param nombre El nombre del módulo.
\param modulo El tipo del módulo.
\param textos La información que se le pasa, lo importante.
\param dato Un puntero a la información que nos es útil. En este caso, el GladeXML de la ventana principal.
*/
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

//! Función de retrollamada para el temporizador.
/*! Es llamada con la frecuencia dada en la aplicación.
  
\param data Un puntero al dato que nos interesa. En este caso, la ventana principal.

\return Devuelve siempre TRUE, para que siga ejecutándose. Si se quiere detener, se detiene desde fuera.
*/
gboolean tick(gpointer data)
{
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(data), "pipeline");
  if(p) {
    pipeline_ciclo(p);
  }
  return TRUE;
}

//! Retrollamada de evento de cambiar el botón de activo.
/*! Se llama a esta función cuando se activa o desactiva un elemento.
  
\param cell El renderer del checkbox.
\param path_string La ruta del árbol, que convertimos a GtkTreePath.
\param user_data Un puntero de información adicional. En este caso, apunta al GladeXML de la aplicación.
*/
void cell_toggled_callback (GtkCellRendererToggle *cell,
			    gchar                 *path_string,
			    gpointer               user_data) {
  GladeXML * xml = (GladeXML*)user_data;
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  GtkTreeView *arbol = (GtkTreeView *)glade_xml_get_widget(xml, "tree_elementos");
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(ventana), "pipeline");
  GtkTreeModel *model = gtk_tree_view_get_model(arbol);
  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);

  if(gtk_tree_model_get_iter (model, &iter, path)) {
    char *nombre;
    gboolean activo;
    gtk_tree_model_get(model, &iter, ACTIVO, &activo, -1);
    gtk_tree_model_get(model, &iter, NOMBRE, &nombre, -1);
    activo = !activo;
    gtk_list_store_set (GTK_LIST_STORE(model), &iter,
			ACTIVO, (gboolean)activo,
			-1);
    pipeline_set_activo(p, nombre, activo);
    g_free(nombre);
    gtk_tree_path_free (path);
  }
}

//! Abre un pipeline.
/*! Dados una ruta y un GladeXML ya creado, carga el pipeline y establece los atributos pertinentes (datos de la ventana).
  
\param ruta La ruta que se carga.
\param xml El GladeXML, ya creado y listo.
*/
void abrir_pipe(const char *ruta, GladeXML *xml) {
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  pipeline_t *p = (pipeline_t *)g_object_get_data(G_OBJECT(ventana), "pipeline");
  dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(ventana), "timer");
  pipeline_borrar(p);  
  GString *dir = g_string_new(LIBDIR);
  g_string_append_printf(dir, "/%s", PACKAGE);
  p = pipeline_cargar(ruta, dir->str, funcion_error, xml);
  g_string_free(dir, TRUE);
  g_object_set_data(G_OBJECT(ventana), "pipeline", p);
  pipeline_iniciar(p);
  GtkWidget *pausa = glade_xml_get_widget(xml, "tbn_pausa");
  if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pausa)) &&
     !t->m_iniciado) {
    t->m_timer_id = g_timeout_add(t->m_timer, tick, ventana);
    t->m_iniciado = TRUE;
  }
  char **nombres = pipeline_nombres(p);  
  GtkTreeView *arbol = (GtkTreeView *)glade_xml_get_widget(xml, "tree_elementos");
  GtkListStore *store = (GtkListStore*)gtk_tree_view_get_model(arbol);
  GtkTreeIter iter;
  while(*nombres) {
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
			ACTIVO, (gboolean)pipeline_get_activo(p, *nombres),
			NOMBRE, *nombres,			
			-1);
    nombres++;
  }
}

//! Retrollamada para el botón de abrir pipeline.
/*! 
  
\param button El botón que lo lanza.
\param user_data El dato que recibe (0 actualmente).
*/
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


//! Función de retrollamada del botón de pausa.
/*! 
  
\param togglebutton El botón que llama.
\param user_data El argumento de usuario, le pasamos cero en esta implementación.
*/
void on_tbn_pausa_activate(GtkToggleButton *togglebutton,
			   gpointer user_data){
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET(togglebutton));
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(ventana), "timer");
  if(gtk_toggle_button_get_active(togglebutton)) {
    g_source_remove(t->m_timer_id);
    t->m_iniciado = FALSE;
  }
  else {
    t->m_timer_id = g_timeout_add(t->m_timer, tick, ventana);
    t->m_iniciado = TRUE;
  }
}

//! Retrollamada para el cambio de temporizador.
/*! Destruye el temporizador anterior y crea uno nuevo con la frecuencia correspondiente al valor de la barra.
  
\param range En GtkRange que llama.
\param user_data Puntero de información, ahora 0.
*/
void on_hsc_temporizador_value_changed(GtkRange *range, gpointer user_data){
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET(range));
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  GtkWidget *pausa = glade_xml_get_widget(xml, "tbn_pausa");
  dato_timer_t *t = (dato_timer_t *)g_object_get_data(G_OBJECT(ventana), "timer");
  guint timer = (guint)floor(gtk_range_get_value(range));
  if(t->m_iniciado) {
    g_source_remove(t->m_timer_id);
  }
  t->m_timer = timer;
  if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pausa))) {
    t->m_timer_id = g_timeout_add(t->m_timer, tick, ventana);
    t->m_iniciado = TRUE;
  }
}

//! Función principal
/*! 
  
\param argc Número de argumentos.
\param argv Cadena de argumentos.

\return 0 si todo va bien, código de error si no es así.
*/
int main(int argc, char **argv)
{
  dato_timer_t dato_timer = {1000, 0, FALSE};
#if OPCIONES_LINEA
  const GOptionEntry entries[] = {
    { "timer", 't', 0, G_OPTION_ARG_INT, &dato_timer.m_timer, "Establece el intervalo del temporizador generador de ciclos, en milisegundos", "T" },
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
  GString *buffer = g_string_new(DATADIR);
  g_string_append_printf(buffer, "/%s/%s", PACKAGE, "ventana_pipeline.glade");
  GladeXML *xml = glade_xml_new(buffer->str, NULL, NULL);
  glade_xml_signal_autoconnect(xml);
  GtkWidget *ventana = glade_xml_get_widget(xml, "win_pipeline");
  g_string_free(buffer, TRUE);

  GdkPixbuf * pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)pipeline_xpm);
  gtk_window_set_icon(GTK_WINDOW(ventana), pixbuf);

  GtkTreeView *arbol = (GtkTreeView *)glade_xml_get_widget(xml, "tree_elementos");

  GtkListStore *store = gtk_list_store_new (NUMERO, G_TYPE_BOOLEAN, G_TYPE_STRING);
  gtk_tree_view_set_model (arbol, GTK_TREE_MODEL(store));
  g_object_unref (GTK_TREE_MODEL(store));

  GtkCellRenderer *renderer_toggle = gtk_cell_renderer_toggle_new ();
  gtk_tree_view_insert_column_with_attributes (arbol,
					       -1,  
					       "Activo",  					       
					       renderer_toggle,
					       "active", ACTIVO,
					       NULL);
  g_object_set(renderer_toggle, "activatable", TRUE, NULL);

  g_signal_connect(renderer_toggle, "toggled", (GCallback) cell_toggled_callback, xml);

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (arbol,
					       -1,      
					       "Nombre",  
					       renderer,
					       "text", NOMBRE,
					       NULL);

  g_object_set_data(G_OBJECT(ventana), "timer", &dato_timer);
  g_object_set_data(G_OBJECT(ventana), "pipeline", 0);
  g_object_set_data(G_OBJECT(ventana), "renderer_toggle", renderer_toggle);
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


