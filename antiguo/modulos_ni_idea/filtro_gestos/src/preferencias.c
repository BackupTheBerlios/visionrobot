#include "preferencias.h"

#include "dialogo_preferencias_filtro.h"

gboolean abrir_preferencias(respuesta_t * respuesta) {

  dialogo_preferencias_filtro_t * dialogo = dialogo_preferencias_filtro_crear();
  
  if(!dialogo) {
     return FALSE;
  }
  
  if(respuesta) {
    gtk_color_button_set_color (GTK_COLOR_BUTTON(dialogo->bot_color_ordenes), &respuesta->orden.color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON(dialogo->bot_color_argumentos), &respuesta->argumento.color);
    gtk_range_set_value(GTK_RANGE(dialogo->sld_tolerancia_ordenes), respuesta->orden.tolerancia);
    gtk_range_set_value(GTK_RANGE(dialogo->sld_tolerancia_argumentos), respuesta->argumento.tolerancia);
  }

  gboolean devolver;

  if (gtk_dialog_run(GTK_DIALOG(dialogo->dlg_preferencias_filtro)) == GTK_RESPONSE_OK) {
    gtk_color_button_get_color(GTK_COLOR_BUTTON(dialogo->bot_color_ordenes), &respuesta->orden.color);
    gtk_color_button_get_color(GTK_COLOR_BUTTON(dialogo->bot_color_argumentos), &respuesta->argumento.color);
    respuesta->orden.tolerancia = gtk_range_get_value(GTK_RANGE(dialogo->sld_tolerancia_ordenes));
    respuesta->argumento.tolerancia = gtk_range_get_value(GTK_RANGE(dialogo->sld_tolerancia_argumentos));
    devolver = TRUE;
  }
  else {
    devolver = FALSE;
  }

  dialogo_preferencias_filtro_cerrar(dialogo);

  return devolver;
}
