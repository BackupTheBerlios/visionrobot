#include "dialogo_abrir.h"
#include "funciones.h"
#include <stdlib.h>

void dialogo_abrir_cerrar(dialogo_abrir_t * dialogo_abrir) {
  gtk_widget_destroy(dialogo_abrir->filechooserdialog1);
  free(dialogo_abrir);
}

dialogo_abrir_t *dialogo_abrir_crear()
{
  dialogo_abrir_t * dialogo_abrir = (dialogo_abrir_t *)malloc(sizeof(dialogo_abrir_t));
    dialogo_abrir->filechooserdialog1 =
	gtk_file_chooser_dialog_new("Abrir...", NULL,
				    GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
    gtk_widget_set_name(dialogo_abrir->filechooserdialog1, "filechooserdialog1");
    dialogo_abrir->filechooserdialog1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (dialogo_abrir->filechooserdialog1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(dialogo_abrir->filechooserdialog1),
			    dialogo_abrir->filechooserdialog1_icon_pixbuf);
	gdk_pixbuf_unref(dialogo_abrir->filechooserdialog1_icon_pixbuf);
    }

    dialogo_abrir->dialog_vbox1 = GTK_DIALOG(dialogo_abrir->filechooserdialog1)->vbox;
    gtk_widget_set_name(dialogo_abrir->dialog_vbox1, "dialog_vbox1");
    gtk_widget_show(dialogo_abrir->dialog_vbox1);

    dialogo_abrir->dialog_action_area1 = GTK_DIALOG(dialogo_abrir->filechooserdialog1)->action_area;
    gtk_widget_set_name(dialogo_abrir->dialog_action_area1, "dialog_action_area1");
    gtk_widget_show(dialogo_abrir->dialog_action_area1);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialogo_abrir->dialog_action_area1),
			      GTK_BUTTONBOX_END);

    dialogo_abrir->button2 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(dialogo_abrir->button2, "button2");
    gtk_widget_show(dialogo_abrir->button2);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialogo_abrir->filechooserdialog1), dialogo_abrir->button2,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(dialogo_abrir->button2, GTK_CAN_DEFAULT);

    dialogo_abrir->button3 = gtk_button_new_from_stock("gtk-open");
    gtk_widget_set_name(dialogo_abrir->button3, "button3");
    gtk_widget_show(dialogo_abrir->button3);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialogo_abrir->filechooserdialog1), dialogo_abrir->button3,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(dialogo_abrir->button3, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    /*GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, filechooserdialog1,
			       "filechooserdialog1");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_vbox1,
			       "dialog_vbox1");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_action_area1,
			       "dialog_action_area1");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button2, "button2");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button3, "button3");*/

    gtk_widget_grab_default(dialogo_abrir->button3);
    return dialogo_abrir;
}
