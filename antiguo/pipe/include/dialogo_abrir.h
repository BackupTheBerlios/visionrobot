#ifndef _DIALOGO_ABRIR_H_INCLUIDO_
#define _DIALOGO_ABRIR_H_INCLUIDO_

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *filechooserdialog1;
    GdkPixbuf *filechooserdialog1_icon_pixbuf;
    GtkWidget *dialog_vbox1;
    GtkWidget *dialog_action_area1;
    GtkWidget *button2;
    GtkWidget *button3;
} dialogo_abrir_t;


dialogo_abrir_t *dialogo_abrir_crear();

void dialogo_abrir_cerrar(dialogo_abrir_t * dialogo_abrir);

/*! \brief Abre un archivo, mostrando un cuadro de di�logo
    \param w La ventana padre
    \return El puntero a FILE del archivo abierto
*/
char *dialogo_abrir_ventana(GtkWidget * w);

#endif
