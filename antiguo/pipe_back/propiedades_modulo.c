#include "propiedades_modulo.h"
#include "dialogo_abrir.h"
//#include "funciones.h"
#include <stdlib.h>

void propiedades_modulo_cerrar(propiedades_modulo_t * propiedades_modulo_)
{
     gtk_widget_destroy(propiedades_modulo_->propiedades_modulo);
    g_free(propiedades_modulo_);
}

void on_button6_activate(GtkButton * button, gpointer user_data)
{
    propiedades_modulo_t *propiedades_modulo_ =
	(propiedades_modulo_t *) user_data;
    GtkWidget *dialog = propiedades_modulo_->propiedades_modulo;
    gchar *file = dialogo_abrir_ventana(dialog);
    if (file) {
	GtkWidget *modulo = propiedades_modulo_->entry5;
	gtk_entry_set_text(GTK_ENTRY(modulo), file);
	g_free(file);
    }
}


propiedades_modulo_t *propiedades_modulo_crear()
{
    propiedades_modulo_t *propiedades_modulo_o =
	(propiedades_modulo_t *) malloc(sizeof(propiedades_modulo_t));
    propiedades_modulo_o->propiedades_modulo = gtk_dialog_new();
    gtk_widget_set_name(propiedades_modulo_o->propiedades_modulo,
			"propiedades_modulo");
    gtk_container_set_border_width(GTK_CONTAINER
				   (propiedades_modulo_o->
				    propiedades_modulo), 2);
    gtk_window_set_title(GTK_WINDOW
			 (propiedades_modulo_o->propiedades_modulo),
			 "Propiedades del m\303\263dulo");

#ifndef WIN32
    propiedades_modulo_o->propiedades_modulo_icon_pixbuf =
	gdk_pixbuf_new_from_file("pipeline.xpm", 0);
    if (propiedades_modulo_o->propiedades_modulo_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW
			    (propiedades_modulo_o->propiedades_modulo),
			    propiedades_modulo_o->
			    propiedades_modulo_icon_pixbuf);
	gdk_pixbuf_unref(propiedades_modulo_o->
			 propiedades_modulo_icon_pixbuf);
    }
#endif

    propiedades_modulo_o->dialog_vbox4 =
	GTK_DIALOG(propiedades_modulo_o->propiedades_modulo)->vbox;
    gtk_widget_set_name(propiedades_modulo_o->dialog_vbox4,
			"dialog_vbox4");
    gtk_widget_show(propiedades_modulo_o->dialog_vbox4);

    propiedades_modulo_o->vbox4 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(propiedades_modulo_o->vbox4, "vbox4");
    gtk_widget_show(propiedades_modulo_o->vbox4);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->dialog_vbox4),
		       propiedades_modulo_o->vbox4, TRUE, TRUE, 0);

    propiedades_modulo_o->label4 =
	gtk_label_new_with_mnemonic("_Etiqueta");
    gtk_widget_set_name(propiedades_modulo_o->label4, "label4");
    gtk_widget_show(propiedades_modulo_o->label4);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->label4, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(propiedades_modulo_o->label4), 0, 2);

    propiedades_modulo_o->entry2 = gtk_entry_new();
    gtk_widget_set_name(propiedades_modulo_o->entry2, "entry2");
    gtk_widget_show(propiedades_modulo_o->entry2);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->entry2, FALSE, FALSE, 0);

    propiedades_modulo_o->hseparator1 = gtk_hseparator_new();
    gtk_widget_set_name(propiedades_modulo_o->hseparator1, "hseparator1");
    gtk_widget_show(propiedades_modulo_o->hseparator1);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->hseparator1, TRUE, TRUE, 2);

    propiedades_modulo_o->label5 = gtk_label_new("Coordenadas");
    gtk_widget_set_name(propiedades_modulo_o->label5, "label5");
    gtk_widget_show(propiedades_modulo_o->label5);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->label5, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(propiedades_modulo_o->label5), 0, 2);

    propiedades_modulo_o->hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_name(propiedades_modulo_o->hbox1, "hbox1");
    gtk_widget_show(propiedades_modulo_o->hbox1);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->hbox1, TRUE, TRUE, 0);

    propiedades_modulo_o->label6 = gtk_label_new_with_mnemonic("_X");
    gtk_widget_set_name(propiedades_modulo_o->label6, "label6");
    gtk_widget_show(propiedades_modulo_o->label6);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox1),
		       propiedades_modulo_o->label6, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(propiedades_modulo_o->label6), 5, 0);

    propiedades_modulo_o->entry3 = gtk_entry_new();
    gtk_widget_set_name(propiedades_modulo_o->entry3, "entry3");
    gtk_widget_show(propiedades_modulo_o->entry3);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox1),
		       propiedades_modulo_o->entry3, TRUE, TRUE, 0);

    propiedades_modulo_o->label7 = gtk_label_new_with_mnemonic("_Y");
    gtk_widget_set_name(propiedades_modulo_o->label7, "label7");
    gtk_widget_show(propiedades_modulo_o->label7);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox1),
		       propiedades_modulo_o->label7, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(propiedades_modulo_o->label7), 5, 0);

    propiedades_modulo_o->entry4 = gtk_entry_new();
    gtk_widget_set_name(propiedades_modulo_o->entry4, "entry4");
    gtk_widget_show(propiedades_modulo_o->entry4);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox1),
		       propiedades_modulo_o->entry4, TRUE, TRUE, 0);

    propiedades_modulo_o->hseparator2 = gtk_hseparator_new();
    gtk_widget_set_name(propiedades_modulo_o->hseparator2, "hseparator2");
    gtk_widget_show(propiedades_modulo_o->hseparator2);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->hseparator2, TRUE, TRUE, 2);

    propiedades_modulo_o->label8 =
	gtk_label_new_with_mnemonic("_Ruta del m\303\263dulo");
    gtk_widget_set_name(propiedades_modulo_o->label8, "label8");
    gtk_widget_show(propiedades_modulo_o->label8);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->label8, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(propiedades_modulo_o->label8), 0, 2);

    propiedades_modulo_o->hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_name(propiedades_modulo_o->hbox2, "hbox2");
    gtk_widget_show(propiedades_modulo_o->hbox2);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->vbox4),
		       propiedades_modulo_o->hbox2, TRUE, TRUE, 0);

    propiedades_modulo_o->entry5 = gtk_entry_new();
    gtk_widget_set_name(propiedades_modulo_o->entry5, "entry5");
    gtk_widget_show(propiedades_modulo_o->entry5);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox2),
		       propiedades_modulo_o->entry5, TRUE, TRUE, 0);

    propiedades_modulo_o->button6 = gtk_button_new_with_mnemonic("_...");
    gtk_widget_set_name(propiedades_modulo_o->button6, "button6");
    gtk_widget_show(propiedades_modulo_o->button6);
    gtk_box_pack_start(GTK_BOX(propiedades_modulo_o->hbox2),
		       propiedades_modulo_o->button6, FALSE, FALSE, 0);
    g_signal_connect((gpointer) propiedades_modulo_o->button6, "clicked",
		     G_CALLBACK(on_button6_activate),
		     propiedades_modulo_o);

    propiedades_modulo_o->dialog_action_area4 =
	GTK_DIALOG(propiedades_modulo_o->propiedades_modulo)->action_area;
    gtk_widget_set_name(propiedades_modulo_o->dialog_action_area4,
			"dialog_action_area4");
    gtk_widget_show(propiedades_modulo_o->dialog_action_area4);
    gtk_button_box_set_layout(GTK_BUTTON_BOX
			      (propiedades_modulo_o->dialog_action_area4),
			      GTK_BUTTONBOX_END);

    propiedades_modulo_o->cancelbutton2 =
	gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(propiedades_modulo_o->cancelbutton2,
			"cancelbutton2");
    gtk_widget_show(propiedades_modulo_o->cancelbutton2);
    gtk_dialog_add_action_widget(GTK_DIALOG
				 (propiedades_modulo_o->
				  propiedades_modulo),
				 propiedades_modulo_o->cancelbutton2,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(propiedades_modulo_o->cancelbutton2,
			 GTK_CAN_DEFAULT);

    propiedades_modulo_o->okbutton2 = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_set_name(propiedades_modulo_o->okbutton2, "okbutton2");
    gtk_widget_show(propiedades_modulo_o->okbutton2);
    gtk_dialog_add_action_widget(GTK_DIALOG
				 (propiedades_modulo_o->
				  propiedades_modulo),
				 propiedades_modulo_o->okbutton2,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(propiedades_modulo_o->okbutton2, GTK_CAN_DEFAULT);

    gtk_label_set_mnemonic_widget(GTK_LABEL(propiedades_modulo_o->label4),
				  propiedades_modulo_o->entry2);
    gtk_label_set_mnemonic_widget(GTK_LABEL(propiedades_modulo_o->label6),
				  propiedades_modulo_o->entry3);
    gtk_label_set_mnemonic_widget(GTK_LABEL(propiedades_modulo_o->label7),
				  propiedades_modulo_o->entry4);
    gtk_label_set_mnemonic_widget(GTK_LABEL(propiedades_modulo_o->label8),
				  propiedades_modulo_o->entry5);

    gtk_widget_grab_default(propiedades_modulo_o->okbutton2);
    return propiedades_modulo_o;
}
