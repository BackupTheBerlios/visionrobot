#ifndef _PROPIEDADES_MODULO_H_INCLUIDO_
#define _PROPIEDADES_MODULO_H_INCLUIDO_

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *propiedades_modulo;
    GdkPixbuf *propiedades_modulo_icon_pixbuf;
    GtkWidget *dialog_vbox4;
    GtkWidget *vbox4;
    GtkWidget *label4;
    GtkWidget *entry2;
    GtkWidget *hseparator1;
    GtkWidget *label5;
    GtkWidget *hbox1;
    GtkWidget *label6;
    GtkWidget *entry3;
    GtkWidget *label7;
    GtkWidget *entry4;
    GtkWidget *hseparator2;
    GtkWidget *label8;
    GtkWidget *hbox2;
    GtkWidget *entry5;
    GtkWidget *button6;
    GtkWidget *dialog_action_area4;
    GtkWidget *cancelbutton2;
    GtkWidget *okbutton2;
} propiedades_modulo_t;

void propiedades_modulo_cerrar(propiedades_modulo_t * propiedades_modulo);

propiedades_modulo_t * propiedades_modulo_crear();

#endif
