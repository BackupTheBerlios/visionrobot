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

typedef struct {
  GtkWidget * ventana;
  GdkGC* gc;
} datos_ventana_t;

static void ventana_pintar(modulo_t * modulo, char* imagen, int alto, int ancho, int bytes) {
  GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
				      FALSE, 8,ancho, alto);
  guchar * p = gdk_pixbuf_get_pixels (pixbuf);
  guchar * b = imagen;
  
  
  int ancho_for = ancho * bytes;
  int i, j;
  for(j = 0; j < alto; ++j) {
    char * aux = &p[(j + 1) * ancho_for]; 
    for(i = 0; i < ancho_for; ++i) {
      *--aux = *b++;      
    }
  }
  if(pixbuf)  {
    gdk_pixbuf_render_to_drawable(pixbuf,
				  ((datos_ventana_t*)modulo->m_dato)->ventana->window, 
				  ((datos_ventana_t*)modulo->m_dato)->gc, 0, 0,
				  0, 0, ancho, alto,
				  GDK_RGB_DITHER_NONE, 0, 0);
    
    gdk_pixbuf_unref(pixbuf);
  }
}

static char *ventana_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  char* imagen;
  int alto, ancho, bytes;
  switch(in->m_tipo) {
  case PIPELINE_IMAGENES:
    imagen = ((filtro_gestos_in_t*)in->m_dato)->m_imagen;
    alto = ((filtro_gestos_in_t*)in->m_dato)->m_alto;
    ancho = ((filtro_gestos_in_t*)in->m_dato)->m_ancho;
    bytes = ((filtro_gestos_in_t*)in->m_dato)->m_bytes;
    break;
  case PIPELINE_FILTRO_GESTOS:    
    imagen = ((red_neuronal_in_t*)in->m_dato)->m_tipo_orden;
    alto = ((red_neuronal_in_t*)in->m_dato)->m_alto;
    ancho = ((red_neuronal_in_t*)in->m_dato)->m_ancho;
    bytes = ((red_neuronal_in_t*)in->m_dato)->m_bytes;
    break;
  }

  ventana_pintar(modulo, imagen, alto, ancho, bytes);
  return 0;
}

static void ventana_crear_ventana(modulo_t *modulo, int alto, int ancho) {
  ((datos_ventana_t*)modulo->m_dato)->ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);  
  gtk_window_set_title(GTK_WINDOW(((datos_ventana_t*)modulo->m_dato)->ventana), "Ventana de imágenes");   
  gtk_window_set_default_size (GTK_WINDOW(((datos_ventana_t*)modulo->m_dato)->ventana), ancho, alto);
  g_signal_connect((gpointer) ((datos_ventana_t*)modulo->m_dato)->ventana, "delete_event",
		   G_CALLBACK(gtk_main_quit), 0);	
  gtk_widget_show(((datos_ventana_t*)modulo->m_dato)->ventana);
  ((datos_ventana_t*)modulo->m_dato)->gc =gdk_gc_new (((datos_ventana_t*)modulo->m_dato)->ventana->window);
}

static int ventana_valor(const char *tipo, int argc, const char **argv)
{
  int i;
  for (i = 0; i < argc; i += 2) {
    if (!strcmp(tipo, argv[i])) {
      return atoi(argv[i + 1]);
    }
  }
  return 0;
}

static char *ventana_iniciar(modulo_t *modulo, int argc, const char **argv)
{
  gtk_init(&argc, (char***)&argv);
  ventana_crear_ventana(modulo, ventana_valor("alto", argc, argv), ventana_valor("ancho", argc, argv));
  return "iniciado";
}
static char *ventana_cerrar(modulo_t *modulo)
{
  g_free(((datos_ventana_t*)modulo->m_dato)->gc);
  gtk_widget_destroy(((datos_ventana_t*)modulo->m_dato)->ventana);
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
