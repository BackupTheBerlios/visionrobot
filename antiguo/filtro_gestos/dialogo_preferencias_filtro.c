#include "dialogo_preferencias_filtro.h"

#include <stdlib.h>

void dialogo_preferencias_filtro_cerrar(dialogo_preferencias_filtro_t * dialogo) {
  gtk_widget_destroy(dialogo->dlg_preferencias_filtro);
  g_free(dialogo);
}

dialogo_preferencias_filtro_t * dialogo_preferencias_filtro_crear() {
  dialogo_preferencias_filtro_t * dialogo_preferencias_filtro =
    (dialogo_preferencias_filtro_t*)malloc(sizeof(dialogo_preferencias_filtro_t));

   dialogo_preferencias_filtro->dlg_preferencias_filtro = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialogo_preferencias_filtro->dlg_preferencias_filtro), "Preferencias del filtro");

  dialogo_preferencias_filtro->dialog_vbox1 = GTK_DIALOG (dialogo_preferencias_filtro->dlg_preferencias_filtro)->vbox;
  gtk_widget_show (dialogo_preferencias_filtro->dialog_vbox1);

  dialogo_preferencias_filtro->vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (dialogo_preferencias_filtro->vbox1);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->dialog_vbox1), dialogo_preferencias_filtro->vbox1, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->lab_info_dialogo = gtk_label_new ("Elija las preferencias del filtro ");
  gtk_widget_show (dialogo_preferencias_filtro->lab_info_dialogo);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox1), dialogo_preferencias_filtro->lab_info_dialogo, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (dialogo_preferencias_filtro->hbox1);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox1), dialogo_preferencias_filtro->hbox1, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->frame1 = gtk_frame_new (NULL);
  gtk_widget_show (dialogo_preferencias_filtro->frame1);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->hbox1), dialogo_preferencias_filtro->frame1, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (dialogo_preferencias_filtro->vbox2);
  gtk_container_add (GTK_CONTAINER (dialogo_preferencias_filtro->frame1), dialogo_preferencias_filtro->vbox2);

  dialogo_preferencias_filtro->label4 = gtk_label_new_with_mnemonic ("Colo_r:");
  gtk_widget_show (dialogo_preferencias_filtro->label4);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox2), dialogo_preferencias_filtro->label4, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->bot_color_ordenes = gtk_color_button_new ();
  gtk_widget_show (dialogo_preferencias_filtro->bot_color_ordenes);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox2), dialogo_preferencias_filtro->bot_color_ordenes, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->label6 = gtk_label_new_with_mnemonic ("_Tolerancia:");
  gtk_widget_show (dialogo_preferencias_filtro->label6);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox2), dialogo_preferencias_filtro->label6, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->sld_tolerancia_ordenes = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 100, 0, 0, 0)));
  gtk_widget_show (dialogo_preferencias_filtro->sld_tolerancia_ordenes);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox2), dialogo_preferencias_filtro->sld_tolerancia_ordenes, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->label2 = gtk_label_new ("\303\223rdenes");
  gtk_widget_show (dialogo_preferencias_filtro->label2);
  gtk_frame_set_label_widget (GTK_FRAME (dialogo_preferencias_filtro->frame1), dialogo_preferencias_filtro->label2);

  dialogo_preferencias_filtro->frame2 = gtk_frame_new (NULL);
  gtk_widget_show (dialogo_preferencias_filtro->frame2);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->hbox1), dialogo_preferencias_filtro->frame2, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (dialogo_preferencias_filtro->vbox3);
  gtk_container_add (GTK_CONTAINER (dialogo_preferencias_filtro->frame2), dialogo_preferencias_filtro->vbox3);

  dialogo_preferencias_filtro->label5 = gtk_label_new_with_mnemonic ("C_olor");
  gtk_widget_show (dialogo_preferencias_filtro->label5);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox3), dialogo_preferencias_filtro->label5, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->bot_color_argumentos = gtk_color_button_new ();
  gtk_widget_show (dialogo_preferencias_filtro->bot_color_argumentos);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox3), dialogo_preferencias_filtro->bot_color_argumentos, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->label7 = gtk_label_new_with_mnemonic ("To_lerancia:");
  gtk_widget_show (dialogo_preferencias_filtro->label7);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox3), dialogo_preferencias_filtro->label7, FALSE, FALSE, 0);

  dialogo_preferencias_filtro->sld_tolerancia_argumentos = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 100, 0, 0, 0)));
  gtk_widget_show (dialogo_preferencias_filtro->sld_tolerancia_argumentos);
  gtk_box_pack_start (GTK_BOX (dialogo_preferencias_filtro->vbox3), dialogo_preferencias_filtro->sld_tolerancia_argumentos, TRUE, TRUE, 0);

  dialogo_preferencias_filtro->label3 = gtk_label_new ("Argumentos");
  gtk_widget_show (dialogo_preferencias_filtro->label3);
  gtk_frame_set_label_widget (GTK_FRAME (dialogo_preferencias_filtro->frame2), dialogo_preferencias_filtro->label3);

  dialogo_preferencias_filtro->dialog_action_area1 = GTK_DIALOG (dialogo_preferencias_filtro->dlg_preferencias_filtro)->action_area;
  gtk_widget_show (dialogo_preferencias_filtro->dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialogo_preferencias_filtro->dialog_action_area1), GTK_BUTTONBOX_END);

  dialogo_preferencias_filtro->cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (dialogo_preferencias_filtro->cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_preferencias_filtro->dlg_preferencias_filtro), dialogo_preferencias_filtro->cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (dialogo_preferencias_filtro->cancelbutton1, GTK_CAN_DEFAULT);

  dialogo_preferencias_filtro->okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (dialogo_preferencias_filtro->okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_preferencias_filtro->dlg_preferencias_filtro), dialogo_preferencias_filtro->okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (dialogo_preferencias_filtro->okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  /*GLADE_HOOKUP_OBJECT_NO_REF (dlg_preferencias_filtro, dlg_preferencias_filtro, "dlg_preferencias_filtro");
  GLADE_HOOKUP_OBJECT_NO_REF (dlg_preferencias_filtro, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, lab_info_dialogo, "lab_info_dialogo");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label4, "label4");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, bot_color_ordenes, "bot_color_ordenes");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label6, "label6");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, sld_tolerancia_ordenes, "sld_tolerancia_ordenes");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label2, "label2");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label5, "label5");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, bot_color_argumentos, "bot_color_argumentos");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label7, "label7");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, sld_tolerancia_argumentos, "sld_tolerancia_argumentos");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, label3, "label3");
  GLADE_HOOKUP_OBJECT_NO_REF (dlg_preferencias_filtro, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dlg_preferencias_filtro, okbutton1, "okbutton1");*/

  return dialogo_preferencias_filtro;//->dlg_preferencias_filtro;
 
}
