/*! \file ventana_imagen.c
    \brief Muestra una ventana con una imagen
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include "imagenes_sdk.h"
#include "filtro_gestos_sdk.h"
#include "red_neuronal_sdk.h"
#include "ventana_imagen_sdk.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glade/glade.h>

typedef struct {
  GtkWidget * ventana;
  char *nombre_foto;
  GdkGC* gc;
  char* imagen;
  int alto;
  int ancho;
  int bytes;
} datos_ventana_t;


static GdkPixbuf *ventana_get_pixbuf(modulo_t * modulo) {
  datos_ventana_t * datos = (datos_ventana_t *)modulo->m_dato;

  GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
				      FALSE, 8,datos->ancho, datos->alto);
  guchar * p = gdk_pixbuf_get_pixels (pixbuf);
  guchar * b = datos->imagen;
  
  
  int ancho_for = datos->ancho * datos->bytes;
  int i, j;
  for(j = 0; j < datos->alto; ++j) {
    char * aux = &p[(j + 1) * ancho_for]; 
    for(i = 0; i < ancho_for; ++i) {
      *--aux = *b++;      
    }
  }
  return pixbuf;
}

gboolean ventana_foto(GtkWidget *w, GdkEventKey *event, gpointer data) {
  modulo_t *modulo = (modulo_t *)data;
  datos_ventana_t *datos = (datos_ventana_t *)modulo->m_dato;
  if(event->keyval == GDK_F5) {
    char *buf = (char*)malloc(sizeof(char) * strlen(datos->nombre_foto) + 4);
    sprintf(buf, "%s.png", datos->nombre_foto);
    gdk_pixbuf_save(ventana_get_pixbuf(modulo),
		    buf,
		    "png", 0, 0);
  }
  return FALSE;
}


static void ventana_pintar(modulo_t * modulo) {

  GdkPixbuf *pixbuf = ventana_get_pixbuf(modulo);
  if(pixbuf)  {
    datos_ventana_t * datos = (datos_ventana_t *)modulo->m_dato;
    gdk_pixbuf_render_to_drawable(pixbuf,
				  ((datos_ventana_t*)modulo->m_dato)->ventana->window, 
				  ((datos_ventana_t*)modulo->m_dato)->gc, 0, 0,
				  0, 0, datos->ancho, datos->alto,
				  GDK_RGB_DITHER_NONE, 0, 0);
    
    gdk_pixbuf_unref(pixbuf);
  }
}

static char *ventana_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  datos_ventana_t * datos = (datos_ventana_t *)modulo->m_dato;
  switch(in->m_tipo) {
  case PIPELINE_IMAGENES:
    datos->imagen = ((filtro_gestos_in_t*)in->m_dato)->m_imagen;
    datos->alto = ((filtro_gestos_in_t*)in->m_dato)->m_alto;
    datos->ancho = ((filtro_gestos_in_t*)in->m_dato)->m_ancho;
    datos->bytes = ((filtro_gestos_in_t*)in->m_dato)->m_bytes;
    break;
  case PIPELINE_FILTRO_GESTOS:    
    datos->imagen = ((red_neuronal_in_t*)in->m_dato)->m_tipo_orden;
    datos->alto = ((red_neuronal_in_t*)in->m_dato)->m_alto;
    datos->ancho = ((red_neuronal_in_t*)in->m_dato)->m_ancho;
    datos->bytes = ((red_neuronal_in_t*)in->m_dato)->m_bytes;
    break;
  }

  ventana_pintar(modulo);
  return 0;
}

static void ventana_crear_ventana(modulo_t *modulo) {
  static GladeXML* xml = 0;
  xml = glade_xml_new("ventana_imagen.glade", NULL, NULL);
  glade_xml_signal_autoconnect(xml);  
  ((datos_ventana_t*)modulo->m_dato)->ventana = glade_xml_get_widget(xml, "win_imagen");
  gtk_window_set_default_size (GTK_WINDOW(((datos_ventana_t*)modulo->m_dato)->ventana), 100, 100);
  g_signal_connect ((gpointer) ((datos_ventana_t*)modulo->m_dato)->ventana, "key_press_event",
                    G_CALLBACK (ventana_foto),
                    modulo);
  ((datos_ventana_t*)modulo->m_dato)->gc =gdk_gc_new (((datos_ventana_t*)modulo->m_dato)->ventana->window);
}
static char *ventana_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  if(g_hash_table_size(argumentos) < 1) {return "faltan argumentos";}
  ((datos_ventana_t*)modulo->m_dato)->nombre_foto = strdup(g_hash_table_lookup(argumentos, "nombre_foto"));
  ventana_crear_ventana(modulo);
  return "iniciado";
}
static char *ventana_cerrar(modulo_t *modulo)
{
  gdk_gc_unref(((datos_ventana_t*)modulo->m_dato)->gc);
  gtk_widget_destroy(((datos_ventana_t*)modulo->m_dato)->ventana);
  free(((datos_ventana_t*)modulo->m_dato)->nombre_foto);
  free(modulo->m_dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Ventana de imagen";
  modulo->m_iniciar = ventana_iniciar;
  modulo->m_cerrar = ventana_cerrar;
  modulo->m_ciclo = ventana_ciclo;
  modulo->m_dato = (datos_ventana_t*)malloc(sizeof(datos_ventana_t));  
  return modulo;
}
