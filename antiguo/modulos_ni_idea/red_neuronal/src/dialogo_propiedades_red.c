#include "dialogo_propiedades_red.h"

#include <stdlib.h>

#include "dialogo_abrir.h"

void boton_abrir(GtkButton * button, gpointer user_data){
// a ver si hay que cargarse algo
  dialogo_propiedades_red_t *dialogo_propiedades_red = (dialogo_propiedades_red_t *)user_data;
  dialogo_abrir_t * dialogo_abrir = dialogo_abrir_crear();
  GtkWidget *dialog = dialogo_abrir->filechooserdialog1;
  char *filename = 0;
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_entry_set_text(GTK_ENTRY(dialogo_propiedades_red->entry_ruta), filename);
    g_free(filename);
  }
  //gtk_widget_destroy(dialog);
  dialogo_abrir_cerrar(dialogo_abrir);
}

void dialogo_propiedades_red_cerrar(dialogo_propiedades_red_t * dialogo) {
  gtk_widget_destroy(dialogo->dlg_propiedades_red);
  g_free(dialogo);
}

dialogo_propiedades_red_t * dialogo_propiedades_red_crear() {
dialogo_propiedades_red_t * dialogo_propiedades_red =
    (dialogo_propiedades_red_t*)malloc(sizeof(dialogo_propiedades_red_t));

dialogo_propiedades_red->rbt_ordenes_group = NULL;
  dialogo_propiedades_red->dlg_propiedades_red = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialogo_propiedades_red->dlg_propiedades_red), "Propiedades de la red");

  dialogo_propiedades_red->dialog_vbox1 = GTK_DIALOG (dialogo_propiedades_red->dlg_propiedades_red)->vbox;
  gtk_widget_show (dialogo_propiedades_red->dialog_vbox1);

  dialogo_propiedades_red->vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (dialogo_propiedades_red->vbox1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->dialog_vbox1), dialogo_propiedades_red->vbox1, TRUE, TRUE, 0);

  dialogo_propiedades_red->label1 = gtk_label_new_with_mnemonic ("_Ruta del archivo:");
  gtk_widget_show (dialogo_propiedades_red->label1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox1), dialogo_propiedades_red->label1, FALSE, FALSE, 0);

  dialogo_propiedades_red->hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (dialogo_propiedades_red->hbox1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox1), dialogo_propiedades_red->hbox1, TRUE, TRUE, 0);

  dialogo_propiedades_red->entry_ruta = gtk_entry_new ();
  gtk_widget_show (dialogo_propiedades_red->entry_ruta);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->hbox1), dialogo_propiedades_red->entry_ruta, TRUE, TRUE, 0);

  dialogo_propiedades_red->button1 = gtk_button_new ();
  gtk_widget_show (dialogo_propiedades_red->button1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->hbox1), dialogo_propiedades_red->button1, FALSE, FALSE, 0);

  dialogo_propiedades_red->alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (dialogo_propiedades_red->alignment1);
  gtk_container_add (GTK_CONTAINER (dialogo_propiedades_red->button1), dialogo_propiedades_red->alignment1);

  dialogo_propiedades_red->hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (dialogo_propiedades_red->hbox2);
  gtk_container_add (GTK_CONTAINER (dialogo_propiedades_red->alignment1), dialogo_propiedades_red->hbox2);

  dialogo_propiedades_red->image1 = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (dialogo_propiedades_red->image1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->hbox2), dialogo_propiedades_red->image1, FALSE, FALSE, 0);

  dialogo_propiedades_red->label4 = gtk_label_new_with_mnemonic ("_...");
  gtk_widget_show (dialogo_propiedades_red->label4);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->hbox2), dialogo_propiedades_red->label4, FALSE, FALSE, 0);

  dialogo_propiedades_red->hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (dialogo_propiedades_red->hseparator1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox1), dialogo_propiedades_red->hseparator1, TRUE, TRUE, 0);

  dialogo_propiedades_red->frame1 = gtk_frame_new (NULL);
  gtk_widget_show (dialogo_propiedades_red->frame1);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox1), dialogo_propiedades_red->frame1, TRUE, TRUE, 0);

  dialogo_propiedades_red->vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (dialogo_propiedades_red->vbox2);
  gtk_container_add (GTK_CONTAINER (dialogo_propiedades_red->frame1), dialogo_propiedades_red->vbox2);

  dialogo_propiedades_red->rbt_ordenes = gtk_radio_button_new_with_mnemonic (NULL, "\303\223_rdenes");
  gtk_widget_show (dialogo_propiedades_red->rbt_ordenes);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox2), dialogo_propiedades_red->rbt_ordenes, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (dialogo_propiedades_red->rbt_ordenes), dialogo_propiedades_red->rbt_ordenes_group);
  dialogo_propiedades_red->rbt_ordenes_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialogo_propiedades_red->rbt_ordenes));

  dialogo_propiedades_red->rbt_parametros = gtk_radio_button_new_with_mnemonic (NULL, "P_ar\303\241metros");
  gtk_widget_show (dialogo_propiedades_red->rbt_parametros);
  gtk_box_pack_start (GTK_BOX (dialogo_propiedades_red->vbox2), dialogo_propiedades_red->rbt_parametros, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (dialogo_propiedades_red->rbt_parametros), dialogo_propiedades_red->rbt_ordenes_group);
  dialogo_propiedades_red->rbt_ordenes_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialogo_propiedades_red->rbt_parametros));

  dialogo_propiedades_red->label3 = gtk_label_new ("M\303\263dulo");
  gtk_widget_show (dialogo_propiedades_red->label3);
  gtk_frame_set_label_widget (GTK_FRAME (dialogo_propiedades_red->frame1), dialogo_propiedades_red->label3);

  dialogo_propiedades_red->dialog_action_area1 = GTK_DIALOG (dialogo_propiedades_red->dlg_propiedades_red)->action_area;
  gtk_widget_show (dialogo_propiedades_red->dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialogo_propiedades_red->dialog_action_area1), GTK_BUTTONBOX_END);

  dialogo_propiedades_red->cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (dialogo_propiedades_red->cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_propiedades_red->dlg_propiedades_red), dialogo_propiedades_red->cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (dialogo_propiedades_red->cancelbutton1, GTK_CAN_DEFAULT);

  dialogo_propiedades_red->okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (dialogo_propiedades_red->okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_propiedades_red->dlg_propiedades_red), dialogo_propiedades_red->okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (dialogo_propiedades_red->okbutton1, GTK_CAN_DEFAULT);
  
      g_signal_connect((gpointer) dialogo_propiedades_red->button1, "clicked",
		     G_CALLBACK(boton_abrir), dialogo_propiedades_red);

  return dialogo_propiedades_red;

}

