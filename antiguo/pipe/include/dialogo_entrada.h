#ifndef _DIALOGO_ENTRADA_H_INCLUIDO_
#define _DIALOGO_ENTRADA_H_INCLUIDO_

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *dialogo_entrada_ventana;
  GdkPixbuf *dialogo_entrada_ventana_icon_pixbuf;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *label_entrada;
  GtkWidget *entry_entrada;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;
} dialogo_entrada_t;

void dialogo_entrada_cerrar(dialogo_entrada_t * dialogo_entrada);
dialogo_entrada_t *dialogo_entrada_crear();
char *dialogo_entrada_leer_cadena(GtkWidget * w);

#endif //_DIALOGO_ENTRADA_H_
