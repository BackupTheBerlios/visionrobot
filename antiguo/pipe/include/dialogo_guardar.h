#ifndef _DIALOGO_GUARDAR_H_INCLUIDO_
#define _DIALOGO_GUARDAR_H_INCLUIDO_

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *filechooserdialog2;
    GdkPixbuf *filechooserdialog2_icon_pixbuf;
    GtkWidget *dialog_vbox2;
    GtkWidget *dialog_action_area2;
    GtkWidget *button4;
    GtkWidget *button5;
} dialogo_guardar_t;

dialogo_guardar_t *dialogo_guardar_crear();
void dialogo_guardar_cerrar(dialogo_guardar_t * dialogo_guardar);

#endif
