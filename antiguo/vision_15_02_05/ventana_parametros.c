#include "ventana_parametros_sdk.h"
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  GladeXML* m_xml;
  filtro_gestos_in_t m_filtro;
} ventana_parametros_dato_t;

static GtkWidget *ventana_parametros_get_widget(GtkWidget *w, const char *nombre) {
  GladeXML *xml = glade_get_widget_tree(w);
  return glade_xml_get_widget(xml, nombre);
}

static ventana_parametros_dato_t *get_dato(GtkWidget *w) {
  GtkWidget *ventana = ventana_parametros_get_widget(w, "win_parametros_filtro");
  modulo_t * modulo = g_object_get_data(G_OBJECT(ventana), "modulo");
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  return dato;
}

static gdouble ventana_parametros_get_valor(GtkWidget *w, const char *nombre) {
  GtkWidget *scroll_ordenes = ventana_parametros_get_widget(w, nombre);
  return gtk_range_get_value(GTK_RANGE(scroll_ordenes));
}

static int ventana_parametros_incremento(int valor, double porcentaje) {
 return floor(((float)valor) * porcentaje / 100.0f);
}

static void ventana_parametros_color_ordenes(GtkWidget *w) {
  GdkColor color;
  gtk_color_button_get_color(GTK_COLOR_BUTTON(ventana_parametros_get_widget(w, "bot_color_ordenes")), &color);
  ventana_parametros_dato_t *dato = get_dato(w);
  gdouble valor = ventana_parametros_get_valor(w, "hsc_tolerancia_ordenes");
  int inc_rojo = ventana_parametros_incremento(color.red, valor);
  int inc_verde = ventana_parametros_incremento(color.green, valor);
  int inc_azul = ventana_parametros_incremento(color.blue, valor);
  dato->m_filtro.m_dato.parametros.m_rojo_sup_orden = color.red + inc_rojo;
  dato->m_filtro.m_dato.parametros.m_verde_sup_orden = color.green + inc_verde;
  dato->m_filtro.m_dato.parametros.m_azul_sup_orden = color.blue + inc_azul;
  dato->m_filtro.m_dato.parametros.m_rojo_inf_orden = color.red - inc_rojo;
  dato->m_filtro.m_dato.parametros.m_verde_inf_orden = color.green - inc_verde;
  dato->m_filtro.m_dato.parametros.m_azul_inf_orden = color.blue - inc_azul;
}

static void ventana_parametros_color_parametros(GtkWidget *w) {
  GdkColor color;
  gtk_color_button_get_color(GTK_COLOR_BUTTON(ventana_parametros_get_widget(w, "bot_color_ordenes")), &color);
  ventana_parametros_dato_t *dato = get_dato(w);
  gdouble valor = ventana_parametros_get_valor(w, "hsc_tolerancia_ordenes");
  int inc_rojo = ventana_parametros_incremento(color.red, valor);
  int inc_verde = ventana_parametros_incremento(color.green, valor);
  int inc_azul = ventana_parametros_incremento(color.blue, valor);
  dato->m_filtro.m_dato.parametros.m_rojo_sup_param = color.red + inc_rojo;
  dato->m_filtro.m_dato.parametros.m_verde_sup_param = color.green + inc_verde;
  dato->m_filtro.m_dato.parametros.m_azul_sup_param = color.blue + inc_azul;
  dato->m_filtro.m_dato.parametros.m_rojo_inf_param = color.red - inc_rojo;
  dato->m_filtro.m_dato.parametros.m_verde_inf_param = color.green - inc_verde;
  dato->m_filtro.m_dato.parametros.m_azul_inf_param = color.blue - inc_azul;

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

static char *ventana_parametros_ciclo(modulo_t *modulo, char tipo, GHashTable *lista)
{
  return 0;
}

static char *ventana_parametros_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  if (g_hash_table_size(argumentos) < 12) {
    return "faltan parametros";
  }
  ventana_parametros_dato_t *dato = (ventana_parametros_dato_t*)modulo->m_dato;
  dato->m_xml = glade_xml_new("ventana_parametros.glade", NULL, NULL);
  GtkWidget *ventana = glade_xml_get_widget(dato->m_xml, "win_parametros_filtro");
  g_object_set_data(G_OBJECT(ventana), "modulo", (gpointer)modulo);
  glade_xml_signal_autoconnect(dato->m_xml);
  dato->m_filtro.m_tipo = PIPELINE_FILTRO_GESTOS_PARAMETROS;
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_FILTRO_GESTOS), &dato->m_filtro);
  dato->m_filtro.m_dato.parametros.m_rojo_sup_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_superior_rojo"));
  dato->m_filtro.m_dato.parametros.m_rojo_inf_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_inferior_rojo"));
  dato->m_filtro.m_dato.parametros.m_verde_sup_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_superior_verde"));
  dato->m_filtro.m_dato.parametros.m_verde_inf_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_inferior_verde"));
  dato->m_filtro.m_dato.parametros.m_azul_sup_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_superior_azul"));
  dato->m_filtro.m_dato.parametros.m_azul_inf_orden = (unsigned char)atoi(g_hash_table_lookup(argumentos, "orden_inferior_azul"));
  dato->m_filtro.m_dato.parametros.m_rojo_sup_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_superior_rojo"));
  dato->m_filtro.m_dato.parametros.m_rojo_inf_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_inferior_rojo"));
  dato->m_filtro.m_dato.parametros.m_verde_sup_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_superior_verde"));
  dato->m_filtro.m_dato.parametros.m_verde_inf_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_inferior_verde"));
  dato->m_filtro.m_dato.parametros.m_azul_sup_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_superior_azul"));
  dato->m_filtro.m_dato.parametros.m_azul_inf_param = (unsigned char)atoi(g_hash_table_lookup(argumentos, "param_inferior_azul"));
  
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
