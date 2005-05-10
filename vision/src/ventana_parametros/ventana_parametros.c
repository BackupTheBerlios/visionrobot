/*! \file ventana_parametros.c
    \brief Una ventana en la que se pueden ajustar los parámetros de un filtro.

          \section modulo Módulo
	  Este módulo tiene como función el establecimiento de los parámetros del filtro, el color fundamental, y la tolerancia a errores de este. Está pensado para admitir un color. Funciona con GTK, por lo que el programa que lo instancie deberá iniciar previamente estas librerías.
	  Funciona con el archivo "ventana_parametros.glade", que deberá estar en el mismo directorio.
	  Para usar el módulo habrá que conectarlo a un módulo de filtros. Después elegir, pinchando en el botón de color, el color fundamental (puede ayudarse de la herramienta de cuentagotas para adquirir el color de la pantalla), y con la barra de desplazamiento establecer asimismo la tolerancia de cada color (rojo, verde y azul). El módulo envía al filtro una estructura con los valores elegidos.

	  \section puertos Puertos
	  El módulo no dispone de puertos de entrada, ya que es un módulo de inicio (debe especificarse en el XML con inicio="1"), y como salida tiene:
	  <ul>
	    <li><em>salida_filtro</em>: Un puntero a una estructura <code>filtro_gestos_in_parametros_t</code>, en cuyos campos se hallan los valores pertinentes.
	  </ul>

	  \section argumentos Argumentos
	  Los argumentos que puede, en el archivo de especificación, admitir el módulo son:
	  <ul>
	    <li><em>rojo</em>: El color rojo que se establece por defecto.
	    <li><em>verde</em>: El color verde que se establece por defecto.
	    <li><em>azul</em>: El color azul que se establece por defecto.
	    <li><em>tolerancia</em>: La tolerancia que se establece por defecto.
	  </ul>
    \author Carlos León
    \version 1.0
*/  
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

#define MAX_GUINT 65535
#define MAX_CHAR 255

#define PUERTO_FILTRO "salida_filtro"


#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define DATADIR g_get_current_dir()
#define PACKAGE ""
#endif

//! Estructura de datos del módulo
typedef struct {
  char m_cambio; /*!< Si hay que cambiar. */
  GladeXML* m_xml; /*!< El GladeXML de la ventana. */
  filtro_gestos_in_parametros_t m_filtro; /*!< La estructura que contiene los colores actuales. */
} ventana_parametros_dato_t;

static GtkWidget *ventana_parametros_get_widget(GtkWidget *w, const char *nombre) {
  GladeXML *xml = glade_get_widget_tree(w);
  return glade_xml_get_widget(xml, nombre);
}

static ventana_parametros_dato_t *ventana_parametros_get_dato(GtkWidget *w) {
  GtkWidget *ventana = ventana_parametros_get_widget(w, "win_parametros_filtro");
  modulo_t * modulo = g_object_get_data(G_OBJECT(ventana), "modulo");
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  return dato;
}

static gdouble ventana_parametros_get_valor(GtkWidget *w, const char *nombre) {
  GtkWidget *scroll_ordenes = ventana_parametros_get_widget(w, nombre);
  return gtk_range_get_value(GTK_RANGE(scroll_ordenes));
}

static guint16 ventana_parametros_char_to_int(color_t color) {
  return (guint16)floor(((float)color) * ((float)MAX_GUINT) / ((float)MAX_CHAR));
}

static color_t ventana_parametros_int_to_char(guint16 color) {
  return (color_t)floor(((float)color) * ((float)MAX_CHAR) / ((float)MAX_GUINT));
}

static guint16 ventana_parametros_incremento(double numero, double valor, double porcentaje) {
  double total = numero * porcentaje / 100.0;
  total = total + valor;
  if(total <= 0.0) total = 0.0;
  if(total >= numero) total = numero;
  return (guint16)floor(total);
}

static color_t ventana_parametros_suma(guint16 color, gdouble porcentaje) {
  return ventana_parametros_int_to_char(ventana_parametros_incremento((double)MAX_GUINT, (double)color, porcentaje));
}
static void ventana_parametros_calcular_orden(filtro_gestos_in_parametros_t * parametros, GdkColor* color,
					      gdouble porcentaje_rojo, 
					      gdouble porcentaje_verde,
					      gdouble porcentaje_azul) {
  parametros->m_rojo_sup = ventana_parametros_suma(color->red, porcentaje_rojo);
  parametros->m_verde_sup = ventana_parametros_suma(color->green, porcentaje_verde);
  parametros->m_azul_sup = ventana_parametros_suma(color->blue, porcentaje_azul);
  parametros->m_rojo_inf = ventana_parametros_suma(color->red, -porcentaje_rojo);
  parametros->m_verde_inf = ventana_parametros_suma(color->green, -porcentaje_verde);
  parametros->m_azul_inf = ventana_parametros_suma(color->blue, -porcentaje_azul);
}
static void ventana_parametros_color_ordenes(GtkWidget *w) {
  ventana_parametros_dato_t *dato = ventana_parametros_get_dato(w);
  GdkColor color;
  gdouble valor_rojo = ventana_parametros_get_valor(w, "hsc_tolerancia_rojo");
  gdouble valor_verde = ventana_parametros_get_valor(w, "hsc_tolerancia_verde");
  gdouble valor_azul = ventana_parametros_get_valor(w, "hsc_tolerancia_azul");
  gtk_color_button_get_color(GTK_COLOR_BUTTON(ventana_parametros_get_widget(w, "bot_color_ordenes")), &color);
  dato->m_cambio = 1;
  ventana_parametros_calcular_orden(&dato->m_filtro, &color, valor_rojo, valor_verde, valor_azul);
}
G_MODULE_EXPORT void on_bot_color_ordenes_color_set(GtkColorButton *widget, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(widget));
}

G_MODULE_EXPORT void on_hsc_tolerancia_rojo_value_changed(GtkRange *range, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(range));
}
G_MODULE_EXPORT void on_hsc_tolerancia_verde_value_changed(GtkRange *range, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(range));
}
G_MODULE_EXPORT void on_hsc_tolerancia_azul_value_changed(GtkRange *range, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(range));
}

//! Realiza un ciclo en el módulo.
/*! En el ciclo recibe la señal del módulo de gestión de resultados del pipeline.
  
\param modulo El módulo actual.
\param puerto El puerto por el que llega la información.
\param dato La información que llega.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *ventana_parametros_ciclo(modulo_t *modulo, const char *puerto, const void *value){
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t *)modulo->m_dato;
  if(dato->m_cambio == 1) {
    dato->m_cambio = 2;
    g_hash_table_insert(modulo->m_tabla, PUERTO_FILTRO, &dato->m_filtro);
  }
  else if (dato->m_cambio == 2) {
    dato->m_cambio = 0;
    g_hash_table_insert(modulo->m_tabla, PUERTO_FILTRO, 0);
  }
  return 0;
}

//! Inicia un módulo.
/*! Crea la memoria, lee los argumentos del XML, y abre el puerto paralelo.
  
\param modulo El módulo actual.
\param argumentos Una tabla con los argumentos.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *ventana_parametros_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  char *titulo = (char *)g_hash_table_lookup(argumentos, "titulo");
  double tolerancia_rojo;
  double tolerancia_verde;
  GdkColor color;
  double tolerancia_azul;
  GString *buffer;
  GtkWidget *color_ordenes;
  GtkWidget *w_tolerancia_rojo;
  GtkWidget *w_tolerancia_verde;
  GtkWidget *w_tolerancia_azul;
  GtkWidget *ventana;
  if (g_hash_table_size(argumentos) < 6) {
    return "faltan parametros";
  } 
  buffer = g_string_new(DATADIR);
  g_string_append_printf(buffer, "/%s/ventana_parametros.glade", PACKAGE);
  dato->m_xml = glade_xml_new(buffer->str, NULL, NULL);
  g_string_free(buffer, TRUE);
  ventana = glade_xml_get_widget(dato->m_xml, "win_parametros_filtro");

  if(titulo) {
    gtk_window_set_title(GTK_WINDOW(ventana), titulo);
  }
  color_ordenes = glade_xml_get_widget(dato->m_xml, "bot_color_ordenes");
  w_tolerancia_rojo = glade_xml_get_widget(dato->m_xml, "hsc_tolerancia_rojo");
  w_tolerancia_verde = glade_xml_get_widget(dato->m_xml, "hsc_tolerancia_verde");
  w_tolerancia_azul = glade_xml_get_widget(dato->m_xml, "hsc_tolerancia_azul");
  g_object_set_data(G_OBJECT(ventana), "modulo", (gpointer)modulo);
  glade_xml_signal_autoconnect(dato->m_xml);
  dato->m_cambio = 1;


  color.red = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "rojo")));
  color.green = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "verde")));
  color.blue = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "azul")));
  tolerancia_rojo = atof(g_hash_table_lookup(argumentos, "tolerancia_rojo"));
  tolerancia_verde = atof(g_hash_table_lookup(argumentos, "tolerancia_verde"));
  tolerancia_azul = atof(g_hash_table_lookup(argumentos, "tolerancia_azul"));
  gtk_color_button_set_color(GTK_COLOR_BUTTON(color_ordenes), &color);
  gtk_range_set_value(GTK_RANGE(w_tolerancia_rojo), (gdouble)tolerancia_rojo);
  gtk_range_set_value(GTK_RANGE(w_tolerancia_verde), (gdouble)tolerancia_verde);
  gtk_range_set_value(GTK_RANGE(w_tolerancia_azul), (gdouble)tolerancia_azul);
  ventana_parametros_calcular_orden(&dato->m_filtro, &color, tolerancia_rojo, tolerancia_verde, tolerancia_azul);

  return "iniciado";
}

//! Cierra un módulo.
/*! Libera toda la memoria creada.
  
\param modulo El módulo actual.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *ventana_parametros_cerrar(modulo_t *modulo)
{
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Parametros";
  modulo->m_iniciar = ventana_parametros_iniciar;
  modulo->m_cerrar = ventana_parametros_cerrar;
  modulo->m_ciclo = ventana_parametros_ciclo;
  modulo->m_dato = (ventana_parametros_dato_t*)malloc(sizeof(ventana_parametros_dato_t));
  return modulo;

}
