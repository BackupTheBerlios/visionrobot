#include "propiedades_red.h"
#include "dialogo_propiedades_red.h"

#include <gtk/gtk.h>

gboolean abrir_propiedades_red(red_t * red) {

  dialogo_propiedades_red_t * dialogo = dialogo_propiedades_red_crear();
  
  if(!dialogo) {
     return FALSE;
  }
  
  if(red) {
          gtk_entry_set_text(GTK_ENTRY(dialogo->entry_ruta), red->ruta);
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialogo->rbt_ordenes), red->tipo == ORDEN);
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialogo->rbt_parametros), red->tipo == PARAM);
  }

  gboolean devolver;

  if (gtk_dialog_run(GTK_DIALOG(dialogo->dlg_propiedades_red)) == GTK_RESPONSE_OK) {
     free(red->ruta);
     red->ruta = (char*)gtk_entry_get_text(GTK_ENTRY(dialogo->entry_ruta));
     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dialogo->rbt_ordenes))) {
         red->tipo = ORDEN;
     }
     else {
          red->tipo = PARAM;
     }
    devolver = TRUE;
  }
  else {
    devolver = FALSE;
  }

  dialogo_propiedades_red_cerrar(dialogo);

  return devolver;
}

