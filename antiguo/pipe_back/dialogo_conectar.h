#ifndef _DIALOGO_CONECTAR_H_INCLUIDO_
#define _DIALOGO_CONECTAR_H_INCLUIDO_

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *dialog1;
    GdkPixbuf *dialog1_icon_pixbuf;
    GtkWidget *dialog_vbox1;
    GtkWidget *vbox1;
    GtkWidget *label1;
    GtkWidget *comboboxentry1;
    GtkWidget *dialog_action_area1;
    GtkWidget *cancelbutton1;
    GtkWidget *okbutton1;
} dialogo_conectar_t;

void dialogo_conectar_cerrar(dialogo_conectar_t * dialogo_conectar);
dialogo_conectar_t *dialogo_conectar_crear();

#endif
