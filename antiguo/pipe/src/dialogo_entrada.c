#include "dialogo_entrada.h"
#include <stdlib.h>
#include <string.h>

char *dialogo_entrada_leer_cadena(GtkWidget * w) {
    dialogo_entrada_t *dialogo_entrada = dialogo_entrada_crear();
    GtkWidget *dialog = dialogo_entrada->dialogo_entrada_ventana;
    char *texto = 0;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    texto = strdup((char *)gtk_entry_get_text(GTK_ENTRY(dialogo_entrada->entry_entrada)));
    }
    dialogo_entrada_cerrar(dialogo_entrada);
    return texto;
}

void dialogo_entrada_cerrar(dialogo_entrada_t * dialogo_entrada)
{
    gtk_widget_destroy(dialogo_entrada->dialogo_entrada_ventana);
    g_free(dialogo_entrada);
}

dialogo_entrada_t *dialogo_entrada_crear ()
{  
    dialogo_entrada_t *dialogo_entrada =
    (dialogo_entrada_t *) malloc(sizeof(dialogo_entrada_t));
  dialogo_entrada->dialogo_entrada_ventana = gtk_dialog_new ();
  gtk_widget_set_name (dialogo_entrada->dialogo_entrada_ventana, "dialogo_entrada_ventana");
  gtk_window_set_title (GTK_WINDOW (dialogo_entrada->dialogo_entrada_ventana),
            "Entrada de texto");
            
    dialogo_entrada->dialogo_entrada_ventana_icon_pixbuf =
    gdk_pixbuf_new_from_file("img/pipeline.xpm", 0);
//  dialogo_entrada->dialogo_entrada_ventana_icon_pixbuf = create_pixbuf ("img/pipeline.xpm");
  
  if (dialogo_entrada->dialogo_entrada_ventana_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (dialogo_entrada->dialogo_entrada_ventana),
               dialogo_entrada->dialogo_entrada_ventana_icon_pixbuf);
      gdk_pixbuf_unref (dialogo_entrada->dialogo_entrada_ventana_icon_pixbuf);
    }
  gtk_window_set_type_hint (GTK_WINDOW (dialogo_entrada->dialogo_entrada_ventana),
                GDK_WINDOW_TYPE_HINT_DIALOG);

  dialogo_entrada->dialog_vbox1 = GTK_DIALOG (dialogo_entrada->dialogo_entrada_ventana)->vbox;
  gtk_widget_set_name (dialogo_entrada->dialog_vbox1, "dialog_vbox1");
  gtk_widget_show (dialogo_entrada->dialog_vbox1);

  dialogo_entrada->vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (dialogo_entrada->vbox1, "vbox1");
  gtk_widget_show (dialogo_entrada->vbox1);
  gtk_box_pack_start (GTK_BOX (dialogo_entrada->dialog_vbox1), dialogo_entrada->vbox1, TRUE, TRUE, 0);

  dialogo_entrada->label_entrada = gtk_label_new ("Introduzca la cadena:");
  gtk_widget_set_name (dialogo_entrada->label_entrada, "label_entrada");
  gtk_widget_show (dialogo_entrada->label_entrada);
  gtk_box_pack_start (GTK_BOX (dialogo_entrada->vbox1), dialogo_entrada->label_entrada, FALSE, FALSE, 0);

  dialogo_entrada->entry_entrada = gtk_entry_new ();
  gtk_widget_set_name (dialogo_entrada->entry_entrada, "entry_entrada");
  gtk_widget_show (dialogo_entrada->entry_entrada);
  gtk_box_pack_start (GTK_BOX (dialogo_entrada->vbox1), dialogo_entrada->entry_entrada, FALSE, FALSE, 0);

  dialogo_entrada->dialog_action_area1 = GTK_DIALOG (dialogo_entrada->dialogo_entrada_ventana)->action_area;
  gtk_widget_set_name (dialogo_entrada->dialog_action_area1, "dialog_action_area1");
  gtk_widget_show (dialogo_entrada->dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialogo_entrada->dialog_action_area1),
                 GTK_BUTTONBOX_END);

  dialogo_entrada->cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_set_name (dialogo_entrada->cancelbutton1, "cancelbutton1");
  gtk_widget_show (dialogo_entrada->cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_entrada->dialogo_entrada_ventana),
                dialogo_entrada->cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (dialogo_entrada->cancelbutton1, GTK_CAN_DEFAULT);

  dialogo_entrada->okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_set_name (dialogo_entrada->okbutton1, "okbutton1");
  gtk_widget_show (dialogo_entrada->okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogo_entrada->dialogo_entrada_ventana),
                dialogo_entrada->okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (dialogo_entrada->okbutton1, GTK_CAN_DEFAULT);

  
  return dialogo_entrada;
}
