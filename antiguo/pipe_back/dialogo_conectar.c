#include "dialogo_conectar.h"
#include "funciones.h"

#include <stdlib.h>

void dialogo_conectar_cerrar(dialogo_conectar_t * dialogo_conectar) {
  gtk_widget_destroy(dialogo_conectar->dialog1);
  free(dialogo_conectar);
}
dialogo_conectar_t *dialogo_conectar_crear()
{
  dialogo_conectar_t * dialogo_conectar = (dialogo_conectar_t *)malloc(sizeof(dialogo_conectar_t));
    dialogo_conectar->dialog1 = gtk_dialog_new();
    gtk_widget_set_name(dialogo_conectar->dialog1, "dialog1");
    gtk_window_set_title(GTK_WINDOW(dialogo_conectar->dialog1),
			 "Selecci\303\263n de m\303\263dulo");
    dialogo_conectar->dialog1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (dialogo_conectar->dialog1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(dialogo_conectar->dialog1), dialogo_conectar->dialog1_icon_pixbuf);
	gdk_pixbuf_unref(dialogo_conectar->dialog1_icon_pixbuf);
    }

    dialogo_conectar->dialog_vbox1 = GTK_DIALOG(dialogo_conectar->dialog1)->vbox;
    gtk_widget_set_name(dialogo_conectar->dialog_vbox1, "dialog_vbox1");
    gtk_widget_show(dialogo_conectar->dialog_vbox1);

    dialogo_conectar->vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(dialogo_conectar->vbox1, "vbox1");
    gtk_widget_show(dialogo_conectar->vbox1);
    gtk_box_pack_start(GTK_BOX(dialogo_conectar->dialog_vbox1), dialogo_conectar->vbox1, TRUE, TRUE, 0);

    dialogo_conectar->label1 = gtk_label_new_with_mnemonic("_Elija un m\303\263dulo:");
    gtk_widget_set_name(dialogo_conectar->label1, "label1");
    gtk_widget_show(dialogo_conectar->label1);
    gtk_box_pack_start(GTK_BOX(dialogo_conectar->vbox1), dialogo_conectar->label1, FALSE, FALSE, 0);

    dialogo_conectar->comboboxentry1 = gtk_combo_box_new_text();
    gtk_widget_set_name(dialogo_conectar->comboboxentry1, "comboboxentry1");
    gtk_widget_show(dialogo_conectar->comboboxentry1);
    gtk_box_pack_start(GTK_BOX(dialogo_conectar->vbox1), dialogo_conectar->comboboxentry1, TRUE, TRUE, 0);

    dialogo_conectar->dialog_action_area1 = GTK_DIALOG(dialogo_conectar->dialog1)->action_area;
    gtk_widget_set_name(dialogo_conectar->dialog_action_area1, "dialog_action_area1");
    gtk_widget_show(dialogo_conectar->dialog_action_area1);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialogo_conectar->dialog_action_area1),
			      GTK_BUTTONBOX_END);

    dialogo_conectar->cancelbutton1 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(dialogo_conectar->cancelbutton1, "cancelbutton1");
    gtk_widget_show(dialogo_conectar->cancelbutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialogo_conectar->dialog1), dialogo_conectar->cancelbutton1,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(dialogo_conectar->cancelbutton1, GTK_CAN_DEFAULT);

    dialogo_conectar->okbutton1 = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_set_name(dialogo_conectar->okbutton1, "okbutton1");
    gtk_widget_show(dialogo_conectar->okbutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialogo_conectar->dialog1), dialogo_conectar->okbutton1,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(dialogo_conectar->okbutton1, GTK_CAN_DEFAULT);

    gtk_label_set_mnemonic_widget(GTK_LABEL(dialogo_conectar->label1), dialogo_conectar->comboboxentry1);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    /*GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog1, "dialog1");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog_vbox1, "dialog_vbox1");
    GLADE_HOOKUP_OBJECT(dialog1, vbox1, "vbox1");
    GLADE_HOOKUP_OBJECT(dialog1, label1, "label1");
    GLADE_HOOKUP_OBJECT(dialog1, comboboxentry1, "comboboxentry1");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog_action_area1,
			       "dialog_action_area1");
    GLADE_HOOKUP_OBJECT(dialog1, cancelbutton1, "cancelbutton1");
    GLADE_HOOKUP_OBJECT(dialog1, okbutton1, "okbutton1");*/

    return dialogo_conectar;
}
