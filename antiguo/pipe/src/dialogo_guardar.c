#include "dialogo_guardar.h"
//#include "funciones.h"
#include <stdlib.h>

void dialogo_guardar_cerrar(dialogo_guardar_t * dialogo_guardar)
{
    gtk_widget_destroy(dialogo_guardar->filechooserdialog2);
    g_free(dialogo_guardar);
}


dialogo_guardar_t *dialogo_guardar_crear()
{

    dialogo_guardar_t *dialogo_guardar =
	(dialogo_guardar_t *) malloc(sizeof(dialogo_guardar_t));
    dialogo_guardar->filechooserdialog2 =
	gtk_file_chooser_dialog_new("Guardar como...", NULL,
				    GTK_FILE_CHOOSER_ACTION_SAVE, NULL);
    gtk_widget_set_name(dialogo_guardar->filechooserdialog2,
			"filechooserdialog2");

//#ifndef WIN32
    dialogo_guardar->filechooserdialog2_icon_pixbuf =
	gdk_pixbuf_new_from_file("img/pipeline.xpm", 0);
    if (dialogo_guardar->filechooserdialog2_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW
			    (dialogo_guardar->filechooserdialog2),
			    dialogo_guardar->
			    filechooserdialog2_icon_pixbuf);
	gdk_pixbuf_unref(dialogo_guardar->filechooserdialog2_icon_pixbuf);
    }
//#endif

    dialogo_guardar->dialog_vbox2 =
	GTK_DIALOG(dialogo_guardar->filechooserdialog2)->vbox;
    gtk_widget_set_name(dialogo_guardar->dialog_vbox2, "dialog_vbox2");
    gtk_widget_show(dialogo_guardar->dialog_vbox2);

    dialogo_guardar->dialog_action_area2 =
	GTK_DIALOG(dialogo_guardar->filechooserdialog2)->action_area;
    gtk_widget_set_name(dialogo_guardar->dialog_action_area2,
			"dialog_action_area2");
    gtk_widget_show(dialogo_guardar->dialog_action_area2);
    gtk_button_box_set_layout(GTK_BUTTON_BOX
			      (dialogo_guardar->dialog_action_area2),
			      GTK_BUTTONBOX_END);

    dialogo_guardar->button4 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(dialogo_guardar->button4, "button4");
    gtk_widget_show(dialogo_guardar->button4);
    gtk_dialog_add_action_widget(GTK_DIALOG
				 (dialogo_guardar->filechooserdialog2),
				 dialogo_guardar->button4,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(dialogo_guardar->button4, GTK_CAN_DEFAULT);

    dialogo_guardar->button5 = gtk_button_new_from_stock("gtk-open");
    gtk_widget_set_name(dialogo_guardar->button5, "button5");
    gtk_widget_show(dialogo_guardar->button5);
    gtk_dialog_add_action_widget(GTK_DIALOG
				 (dialogo_guardar->filechooserdialog2),
				 dialogo_guardar->button5,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(dialogo_guardar->button5, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    /*GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, filechooserdialog2,
       "filechooserdialog2");
       GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, dialog_vbox2,
       "dialog_vbox2");
       GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, dialog_action_area2,
       "dialog_action_area2");
       GLADE_HOOKUP_OBJECT(filechooserdialog2, button4, "button4");
       GLADE_HOOKUP_OBJECT(filechooserdialog2, button5, "button5"); */

    gtk_widget_grab_default(dialogo_guardar->button5);
    return dialogo_guardar;
}
