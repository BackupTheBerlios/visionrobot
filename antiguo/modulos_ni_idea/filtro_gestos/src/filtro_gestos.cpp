/*! \file filtro_gestos_vc.h
    \brief CPP de la DLL del filtro de gestos
    \author Diego y Carlos
    \version 0.1
*/

#ifdef WIN32
#include "dll_pipeline.h"
#endif
#include "Codigo_Filtro.h"
#include "preferencias.h"
#include <stdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

data_in* buffer_in;
data_out*  buffer_out;
Filtro* filtro;
respuesta_t opciones;
GtkWidget * ventana;
char cadena_error[64];
char hay_error = 0;
GdkGC* gc;
GdkPixbuf * pixbuf;

#define PON_ERROR(x) sprintf(cadena_error, (x));hay_error = 1;



#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif   

void pintar() {  
  GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
    FALSE, 8,buffer_out->ancho, buffer_out->alto);
  guchar * p = gdk_pixbuf_get_pixels (pixbuf);
  guchar * b = buffer_out->tipo_orden;
  
  
  int ancho = buffer_out->ancho * buffer_out->bytes;
  int i, j;
  for(j = 0; j < buffer_out->alto; ++j) {
    BYTE * aux = &p[(j + 1) * ancho]; 
    for(i = 0; i < ancho; ++i) {
      *--aux = *b++;      
    }
  }
    if(pixbuf)  {
          gdk_pixbuf_render_to_drawable(pixbuf,
    ventana->window, gc, 0, 0,
    0, 0, buffer_out->ancho, buffer_out->alto,
    GDK_RGB_DITHER_NONE, 0, 0);
    
      gdk_pixbuf_unref(pixbuf);
    }
}

int ciclo (){
  if(buffer_in && buffer_in->m_imagen != 0) {
	buffer_out= filtro->Filtrar(buffer_in->m_imagen, buffer_in->m_alto ,buffer_in->m_ancho, buffer_in->m_bytes);
	buffer_out->ancho = buffer_in->m_ancho;
  buffer_out->alto = buffer_in->m_alto;	
  buffer_out->bytes = buffer_in->m_bytes;  
  pintar(); 
  }
  PON_ERROR("Ciclo en el modulo de gestos.\n");
  return 0;
}
int set_datos(const void * datos){
  buffer_in= (data_in*)datos;  
  if(buffer_in) {
    sprintf(cadena_error, "Filtro de gestos recibe imagen de %i x %i.\n", buffer_in->m_ancho, buffer_in->m_alto);
    hay_error = 1;    
  }
  return 0;
}
void * get_datos(){
  data_out * aux = buffer_out;
  buffer_out= NULL;
  return aux;
}


gboolean
cerrar_ventana(GtkWidget * widget,
			GdkEvent * event, gpointer user_data) {
      gtk_widget_hide(ventana);
			  return TRUE;
}  

int iniciar(const char **argumentos) {
    PON_ERROR("Iniciando filtro de gestos...\n");
      filtro= new Filtro();

      // Iniciar la ventana de visualización
      ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(ventana),
		       "Imagen filtrada");   
    gtk_window_set_default_size (GTK_WINDOW(ventana), 320, 240);		
    
    
    g_signal_connect((gpointer) ventana, "delete_event",
		     G_CALLBACK(cerrar_ventana), 0);
    gtk_widget_show(ventana);
    gc =gdk_gc_new (ventana->window);

	return 0;
}

int color_tolerancia_superior(int color, float tolerancia) {
  return color + ((int)(tolerancia * color));
}

int color_tolerancia_inferior(int color, float tolerancia) {
  return color - ((int)(tolerancia * color));
}


int propiedades(){
    PON_ERROR("Abriendo propiedades...\n");
  // Carlos -->
  if(abrir_preferencias(&opciones)) {    
     //filtro->SetColor(240,160,120,70,120,70,100,50,100,50,100,50);
     filtro->SetColor(
        color_tolerancia_superior(opciones.orden.color.red, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.red, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.orden.color.green, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.green, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.orden.color.blue, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.blue, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.red, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.red, opciones.argumento.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.green, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.green, opciones.argumento.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.blue, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.blue, opciones.argumento.tolerancia));
    // Temporalmente
  }
  // <--
  return 0;
}
int cerrar(){
    PON_ERROR("Cerrando filtro de gestos...\n");
      delete filtro;
      filtro = 0;
      g_free(gc);
      gtk_widget_destroy(ventana);
	return 0;
}
char * error() {	
	char * dev = hay_error ? cadena_error : 0;
	hay_error = 0;
	return dev;
 }
