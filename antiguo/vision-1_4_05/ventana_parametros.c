#include "ventana_parametros_sdk.h"
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

#define MAX_GUINT 65535
#define MAX_CHAR 255

typedef struct {
  GladeXML* m_xml;
  filtro_gestos_in_t m_filtro;
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

guint16 ventana_parametros_char_to_int(color_t color) {
  return (guint16)floor(((float)color) * ((float)MAX_GUINT) / ((float)MAX_CHAR));
}

color_t ventana_parametros_int_to_char(guint16 color) {
  return (color_t)floor(((float)color) * ((float)MAX_CHAR) / ((float)MAX_GUINT));
}

static guint16 ventana_parametros_incremento(double numero, double valor, double porcentaje) {
  double total = numero * porcentaje / 100.0;
  total = total + valor;
  if(total <= 0.0) total = 0.0;
  if(total >= numero) total = numero;
  return (guint16)floor(total);
}

color_t ventana_parametros_suma(guint16 color, gdouble porcentaje) {
  return ventana_parametros_int_to_char(ventana_parametros_incremento((double)MAX_GUINT, (double)color, porcentaje));
}
static void ventana_parametros_calcular_orden(filtro_gestos_in_parametros_t * parametros, GdkColor* color, gdouble porcentaje) {
  parametros->m_cambio = 1;
  parametros->m_rojo_sup_orden = ventana_parametros_suma(color->red, porcentaje);
  parametros->m_verde_sup_orden = ventana_parametros_suma(color->green, porcentaje);
  parametros->m_azul_sup_orden = ventana_parametros_suma(color->blue, porcentaje);
  parametros->m_rojo_inf_orden = ventana_parametros_suma(color->red, -porcentaje);
  parametros->m_verde_inf_orden = ventana_parametros_suma(color->green, -porcentaje);
  parametros->m_azul_inf_orden = ventana_parametros_suma(color->blue, -porcentaje);
}

static void ventana_parametros_calcular_param(filtro_gestos_in_parametros_t * parametros, GdkColor* color, gdouble porcentaje) {
  parametros->m_cambio = 1;
  parametros->m_rojo_sup_param = ventana_parametros_suma(color->red, porcentaje);
  parametros->m_verde_sup_param = ventana_parametros_suma(color->green, porcentaje);
  parametros->m_azul_sup_param = ventana_parametros_suma(color->blue, porcentaje);
  parametros->m_rojo_inf_param = ventana_parametros_suma(color->red, -porcentaje);
  parametros->m_verde_inf_param = ventana_parametros_suma(color->green, -porcentaje);
  parametros->m_azul_inf_param = ventana_parametros_suma(color->blue, -porcentaje);
}

static void ventana_parametros_color_ordenes(GtkWidget *w) {
  GdkColor color;
  gtk_color_button_get_color(GTK_COLOR_BUTTON(ventana_parametros_get_widget(w, "bot_color_ordenes")), &color);
  ventana_parametros_dato_t *dato = ventana_parametros_get_dato(w);
  gdouble valor = ventana_parametros_get_valor(w, "hsc_tolerancia_ordenes");
  ventana_parametros_calcular_orden(&dato->m_filtro.m_dato.parametros, &color, valor);
}

static void ventana_parametros_color_parametros(GtkWidget *w) {
  GdkColor color;
  gtk_color_button_get_color(GTK_COLOR_BUTTON(ventana_parametros_get_widget(w, "bot_color_parametros")), &color);
  ventana_parametros_dato_t *dato = ventana_parametros_get_dato(w);
  gdouble valor = ventana_parametros_get_valor(w, "hsc_tolerancia_parametros");
  ventana_parametros_calcular_param(&dato->m_filtro.m_dato.parametros, &color, valor);
}

void on_bot_color_ordenes_color_set(GtkColorButton *widget, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(widget));
}

void on_hsc_tolerancia_ordenes_value_changed(GtkRange *range, gpointer user_data){
  ventana_parametros_color_ordenes(GTK_WIDGET(range));
}

void on_bot_color_parametros_color_set(GtkColorButton *widget, gpointer user_data){
  ventana_parametros_color_parametros(GTK_WIDGET(widget));
}

void on_hsc_tolerancia_parametros_value_changed(GtkRange *range, gpointer user_data){
  ventana_parametros_color_parametros(GTK_WIDGET(range));
}

static char *ventana_parametros_ciclo(modulo_t *modulo, char tipo, GHashTable *lista){
  return 0;
}

static char *ventana_parametros_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  if (g_hash_table_size(argumentos) < 8) {
    return "faltan parametros";
  }
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  dato->m_xml = glade_xml_new("ventana_parametros.glade", NULL, NULL);
  GtkWidget *ventana = glade_xml_get_widget(dato->m_xml, "win_parametros_filtro");
  GtkWidget *color_ordenes = glade_xml_get_widget(dato->m_xml, "bot_color_ordenes");
  GtkWidget *color_parametros = glade_xml_get_widget(dato->m_xml, "bot_color_parametros");
  GtkWidget *tolerancia_ordenes = glade_xml_get_widget(dato->m_xml, "hsc_tolerancia_ordenes");
  GtkWidget *tolerancia_parametros = glade_xml_get_widget(dato->m_xml, "hsc_tolerancia_parametros");
  g_object_set_data(G_OBJECT(ventana), "modulo", (gpointer)modulo);
  glade_xml_signal_autoconnect(dato->m_xml);
  dato->m_filtro.m_tipo = PIPELINE_FILTRO_GESTOS_PARAMETROS;
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_FILTRO_GESTOS), &dato->m_filtro);

  GdkColor color;
  double tolerancia;
  color.red = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "orden_rojo")));
  color.green = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "orden_verde")));
  color.blue = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "orden_azul")));
  tolerancia = atof(g_hash_table_lookup(argumentos, "orden_tolerancia"));
  gtk_color_button_set_color(GTK_COLOR_BUTTON(color_ordenes), &color);
  gtk_range_set_value(GTK_RANGE(tolerancia_ordenes), (gdouble)tolerancia);

  ventana_parametros_calcular_orden(&dato->m_filtro.m_dato.parametros, &color, tolerancia);

  color.red = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "param_rojo")));
  color.green = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "param_verde")));
  color.blue = ventana_parametros_char_to_int((color_t)atoi(g_hash_table_lookup(argumentos, "param_azul")));
  tolerancia = atof(g_hash_table_lookup(argumentos, "param_tolerancia"));
  gtk_color_button_set_color(GTK_COLOR_BUTTON(color_parametros), &color);
  gtk_range_set_value(GTK_RANGE(tolerancia_parametros), (gdouble)tolerancia);

  ventana_parametros_calcular_param(&dato->m_filtro.m_dato.parametros, &color, tolerancia);

  return "iniciado";
}

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
  modulo->m_tipo = PIPELINE_VENTANA_PARAMETROS;
  modulo->m_nombre = "Parametros";
  modulo->m_iniciar = ventana_parametros_iniciar;
  modulo->m_cerrar = ventana_parametros_cerrar;
  modulo->m_ciclo = ventana_parametros_ciclo;
  modulo->m_dato = (ventana_parametros_dato_t*)malloc(sizeof(ventana_parametros_dato_t));
  return modulo;

}
