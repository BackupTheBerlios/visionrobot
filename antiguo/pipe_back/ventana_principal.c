#include "ventana_principal.h"

#include <stdlib.h>
#include <gdk/gdkkeysyms.h>
#include "auxiliar.h"
#include "funciones.h"
#include <string.h>

gint expose_event(GtkWidget *widget, GdkEvent *event, gpointer data);

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_nuevo2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_guardar2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_salir2_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_pipeline1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_conectar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_ayuda1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data);

gboolean
on_window1_delete_event(GtkWidget * widget,
			GdkEvent * event, gpointer user_data);

void on_window1_destroy(GtkObject * object, gpointer user_data);

void on_window1_show(GtkWidget * widget, gpointer user_data);

void
on_fixed1_drag_begin(GtkWidget * widget,
		     GdkDragContext * drag_context, gpointer user_data);

gboolean
on_fixed1_drag_motion(GtkWidget * widget,
		      GdkDragContext * drag_context,
		      gint x, gint y, guint time, gpointer user_data);

void
on_fixed1_drag_leave(GtkWidget * widget,
		     GdkDragContext * drag_context,
		     guint time, gpointer user_data);

void
on_fixed1_drag_end(GtkWidget * widget,
		   GdkDragContext * drag_context, gpointer user_data);
void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data);

void
on___ndice_de_la_ayuda1_activate(GtkMenuItem * menuitem,
				 gpointer user_data);

void pinchado(GtkMenuItem * menuitem, gpointer user_data);

void on_propiedades_biblioteca_activate(GtkButton * button,
					gpointer user_data);

void on_ciclo_biblioteca_activate(GtkButton * button, gpointer user_data);
void on_ciclos_biblioteca_activate(GtkButton * button, gpointer user_data);
void on_iniciar_biblioteca_activate(GtkButton * button,
				    gpointer user_data);
void on_iniciar_todas_biblioteca_activate(GtkButton * button,
					  gpointer user_data);
void on_parar_biblioteca_activate(GtkButton * button, gpointer user_data);
void on_cerrar_biblioteca_activate(GtkButton * button, gpointer user_data);
void on_cerrar_todas_biblioteca_activate(GtkButton * button,
					 gpointer user_data);


void ventana_principal_borrar(ventana_principal_t * ventana_principal) {
  gtk_widget_destroy(ventana_principal->window1);
  g_free(ventana_principal);
}

ventana_principal_t * ventana_principal_crear()
{
  ventana_principal_t * ventana_principal = (ventana_principal_t *)malloc(sizeof(ventana_principal_t));
    ventana_principal->accel_group = gtk_accel_group_new();

    ventana_principal->window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(ventana_principal->window1, "window1");
    gtk_window_set_title(GTK_WINDOW(ventana_principal->window1),
			 "Visi\303\263n por computador - Pipeline");
    gtk_window_set_default_size(GTK_WINDOW(ventana_principal->window1), 500, 400);
    ventana_principal->window1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (ventana_principal->window1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(ventana_principal->window1), ventana_principal->window1_icon_pixbuf);
	gdk_pixbuf_unref(ventana_principal->window1_icon_pixbuf);
    }

    ventana_principal->vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(ventana_principal->vbox1, "vbox1");
    gtk_widget_show(ventana_principal->vbox1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->window1), ventana_principal->vbox1);

    ventana_principal->menubar1 = gtk_menu_bar_new();
    gtk_widget_set_name(ventana_principal->menubar1, "menubar1");
    gtk_widget_show(ventana_principal->menubar1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1), ventana_principal->menubar1, FALSE, FALSE, 0);

    ventana_principal->archivo1 = gtk_menu_item_new_with_mnemonic("_Archivo");
    gtk_widget_set_name(ventana_principal->archivo1, "archivo1");
    gtk_widget_show(ventana_principal->archivo1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1), ventana_principal->archivo1);

    ventana_principal->archivo1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->archivo1_menu, "archivo1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->archivo1), ventana_principal->archivo1_menu);

    ventana_principal->nuevo2 = gtk_image_menu_item_new_with_mnemonic("_Nuevo");
    gtk_widget_set_name(ventana_principal->nuevo2, "nuevo2");
    gtk_widget_show(ventana_principal->nuevo2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->nuevo2);
    gtk_widget_set_sensitive(ventana_principal->nuevo2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->nuevo2, "activate", ventana_principal->accel_group,
			       GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image103 = gtk_image_new_from_stock("gtk-new", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image103, "image103");
    gtk_widget_show(ventana_principal->image103);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->nuevo2), ventana_principal->image103);

    ventana_principal->abrir2 = gtk_image_menu_item_new_with_mnemonic("_Abrir...");
    gtk_widget_set_name(ventana_principal->abrir2, "abrir2");
    gtk_widget_show(ventana_principal->abrir2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->abrir2);
    gtk_widget_add_accelerator(ventana_principal->abrir2, "activate", ventana_principal->accel_group,
			       GDK_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image104 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image104, "image104");
    gtk_widget_show(ventana_principal->image104);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->abrir2), ventana_principal->image104);

    ventana_principal->separador1 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador1, "separador1");
    gtk_widget_show(ventana_principal->separador1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->separador1);
    gtk_widget_set_sensitive(ventana_principal->separador1, FALSE);

    ventana_principal->guardar_como2 =
	gtk_image_menu_item_new_with_mnemonic("Guardar _como...");
    gtk_widget_set_name(ventana_principal->guardar_como2, "guardar_como2");
    gtk_widget_show(ventana_principal->guardar_como2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->guardar_como2);
    gtk_widget_set_sensitive(ventana_principal->guardar_como2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->guardar_como2, "activate", ventana_principal->accel_group,
			       GDK_g, GDK_CONTROL_MASK | GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    ventana_principal->image105 = gtk_image_new_from_stock("gtk-save-as", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image105, "image105");
    gtk_widget_show(ventana_principal->image105);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->guardar_como2),
				  ventana_principal->image105);

    ventana_principal->guardar2 = gtk_image_menu_item_new_with_mnemonic("_Guardar");
    gtk_widget_set_name(ventana_principal->guardar2, "guardar2");
    gtk_widget_show(ventana_principal->guardar2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->guardar2);
    gtk_widget_set_sensitive(ventana_principal->guardar2, FALSE);
    gtk_widget_add_accelerator(ventana_principal->guardar2, "activate", ventana_principal->accel_group,
			       GDK_g, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image106 = gtk_image_new_from_stock("gtk-save", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image106, "image106");
    gtk_widget_show(ventana_principal->image106);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->guardar2), ventana_principal->image106);

    ventana_principal->separador2 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador2, "separador2");
    gtk_widget_show(ventana_principal->separador2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->separador2);
    gtk_widget_set_sensitive(ventana_principal->separador2, FALSE);

    ventana_principal->salir2 = gtk_image_menu_item_new_with_mnemonic("_Salir");
    gtk_widget_set_name(ventana_principal->salir2, "salir2");
    gtk_widget_show(ventana_principal->salir2);
    gtk_container_add(GTK_CONTAINER(ventana_principal->archivo1_menu), ventana_principal->salir2);
    gtk_widget_add_accelerator(ventana_principal->salir2, "activate", ventana_principal->accel_group,
			       GDK_F4, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image107 = gtk_image_new_from_stock("gtk-quit", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image107, "image107");
    gtk_widget_show(ventana_principal->image107);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->salir2), ventana_principal->image107);

    ventana_principal->pipeline1 = gtk_menu_item_new_with_mnemonic("_Pipeline");
    gtk_widget_set_name(ventana_principal->pipeline1, "pipeline1");
    gtk_widget_show(ventana_principal->pipeline1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1), ventana_principal->pipeline1);

    ventana_principal->pipeline1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->pipeline1_menu, "pipeline1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->pipeline1), ventana_principal->pipeline1_menu);

    ventana_principal->crear1 = gtk_image_menu_item_new_with_mnemonic("_Crear");
    gtk_widget_set_name(ventana_principal->crear1, "crear1");
    gtk_widget_show(ventana_principal->crear1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->crear1);
    gtk_widget_add_accelerator(ventana_principal->crear1, "activate", ventana_principal->accel_group,
			       GDK_Insert, 0, GTK_ACCEL_VISIBLE);

    ventana_principal->image108 = gtk_image_new_from_stock("gtk-add", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image108, "image108");
    gtk_widget_show(ventana_principal->image108);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->crear1), ventana_principal->image108);

    ventana_principal->propiedades1 =
	gtk_image_menu_item_new_with_mnemonic("_Propiedades...");
    gtk_widget_set_name(ventana_principal->propiedades1, "propiedades1");
    gtk_widget_show(ventana_principal->propiedades1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->propiedades1);
    gtk_widget_set_sensitive(ventana_principal->propiedades1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->propiedades1, "activate", ventana_principal->accel_group,
			       GDK_Return, GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    ventana_principal->image109 =
	gtk_image_new_from_stock("gtk-preferences", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image109, "image109");
    gtk_widget_show(ventana_principal->image109);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->propiedades1),
				  ventana_principal->image109);


    ventana_principal->separador5 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador5, "separador3");
    gtk_widget_show(ventana_principal->separador5);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->separador5);
    gtk_widget_set_sensitive(ventana_principal->separador5, FALSE);

    ventana_principal->propiedades_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Info...");
    gtk_widget_set_name(ventana_principal->propiedades_biblioteca, "propiedades_biblioteca");
    gtk_widget_show(ventana_principal->propiedades_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->propiedades_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->propiedades_biblioteca, FALSE);

    ventana_principal->ciclos_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Arrancar pipeline...");
    gtk_widget_set_name(ventana_principal->ciclos_biblioteca, "ciclos_biblioteca");
    gtk_widget_show(ventana_principal->ciclos_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->ciclos_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->ciclos_biblioteca, FALSE);

    ventana_principal->ciclo_biblioteca = gtk_image_menu_item_new_with_mnemonic("_Un ciclo");
    gtk_widget_set_name(ventana_principal->ciclo_biblioteca, "ciclo_biblioteca");
    gtk_widget_show(ventana_principal->ciclo_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->ciclo_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->ciclo_biblioteca, FALSE);

    ventana_principal->parar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Parar pipeline");
    gtk_widget_set_name(ventana_principal->parar_biblioteca, "parar_biblioteca");
    gtk_widget_show(ventana_principal->parar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->parar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->parar_biblioteca, FALSE);

    ventana_principal->iniciar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("Inicia_r m\303\263dulo...");
    gtk_widget_set_name(ventana_principal->iniciar_biblioteca, "iniciar_biblioteca");
    gtk_widget_show(ventana_principal->iniciar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->iniciar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->iniciar_biblioteca, FALSE);

    ventana_principal->iniciar_todas_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("Iniciar _todos...");
    gtk_widget_set_name(ventana_principal->iniciar_todas_biblioteca,
			"iniciar_todas_biblioteca");
    gtk_widget_show(ventana_principal->iniciar_todas_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->iniciar_todas_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->iniciar_todas_biblioteca, FALSE);

    ventana_principal->cerrar_todas_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("C_errar todos...");
    gtk_widget_set_name(ventana_principal->cerrar_todas_biblioteca,
			"cerrar_todas_biblioteca");
    gtk_widget_show(ventana_principal->cerrar_todas_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu),
		      ventana_principal->cerrar_todas_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->cerrar_todas_biblioteca, FALSE);

    ventana_principal->cerrar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("_Cerrar m\303\263dulo...");
    gtk_widget_set_name(ventana_principal->cerrar_biblioteca, "cerrar_biblioteca");
    gtk_widget_show(ventana_principal->cerrar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->cerrar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->cerrar_biblioteca, FALSE);













    ventana_principal->conectar1 = gtk_image_menu_item_new_with_mnemonic("C_onectar...");
    gtk_widget_set_name(ventana_principal->conectar1, "conectar1");
    gtk_widget_show(ventana_principal->conectar1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->conectar1);
    gtk_widget_set_sensitive(ventana_principal->conectar1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->conectar1, "activate", ventana_principal->accel_group,
			       GDK_space, 0, GTK_ACCEL_VISIBLE);

    ventana_principal->image110 = gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image110, "image110");
    gtk_widget_show(ventana_principal->image110);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->conectar1),
				  ventana_principal->image110);

    ventana_principal->separador3 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador3, "separador3");
    gtk_widget_show(ventana_principal->separador3);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->separador3);
    gtk_widget_set_sensitive(ventana_principal->separador3, FALSE);

    ventana_principal->borrar1 = gtk_image_menu_item_new_with_mnemonic("_Borrar");
    gtk_widget_set_name(ventana_principal->borrar1, "borrar1");
    gtk_widget_show(ventana_principal->borrar1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->borrar1);
    gtk_widget_set_sensitive(ventana_principal->borrar1, FALSE);
    gtk_widget_add_accelerator(ventana_principal->borrar1, "activate", ventana_principal->accel_group,
			       GDK_Delete, 0, GTK_ACCEL_VISIBLE);

    ventana_principal->image111 = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image111, "image111");
    gtk_widget_show(ventana_principal->image111);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->borrar1), ventana_principal->image111);

    ventana_principal->ayuda1 = gtk_menu_item_new_with_mnemonic("A_yuda");
    gtk_widget_set_name(ventana_principal->ayuda1, "ayuda1");
    gtk_widget_show(ventana_principal->ayuda1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->menubar1), ventana_principal->ayuda1);

    ventana_principal->ayuda1_menu = gtk_menu_new();
    gtk_widget_set_name(ventana_principal->ayuda1_menu, "ayuda1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ventana_principal->ayuda1), ventana_principal->ayuda1_menu);

    ventana_principal->__ndice_de_la_ayuda1 =
	gtk_image_menu_item_new_with_mnemonic("\303\215ndice de la ayuda");
    gtk_widget_set_name(ventana_principal->__ndice_de_la_ayuda1, "__ndice_de_la_ayuda1");
    gtk_widget_show(ventana_principal->__ndice_de_la_ayuda1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu), ventana_principal->__ndice_de_la_ayuda1);
    gtk_widget_add_accelerator(ventana_principal->__ndice_de_la_ayuda1, "activate",
			       ventana_principal->accel_group, GDK_F1, 0, GTK_ACCEL_VISIBLE);

    ventana_principal->image112 = gtk_image_new_from_stock("gtk-index", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image112, "image112");
    gtk_widget_show(ventana_principal->image112);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (ventana_principal->__ndice_de_la_ayuda1), ventana_principal->image112);

    ventana_principal->separador4 = gtk_separator_menu_item_new();
    gtk_widget_set_name(ventana_principal->separador4, "separador4");
    gtk_widget_show(ventana_principal->separador4);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu), ventana_principal->separador4);
    gtk_widget_set_sensitive(ventana_principal->separador4, FALSE);

    ventana_principal->acerca_de1 = gtk_image_menu_item_new_with_mnemonic("_Acerca de...");
    gtk_widget_set_name(ventana_principal->acerca_de1, "acerca_de1");
    gtk_widget_show(ventana_principal->acerca_de1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->ayuda1_menu), ventana_principal->acerca_de1);
    gtk_widget_add_accelerator(ventana_principal->acerca_de1, "activate", ventana_principal->accel_group,
			       GDK_i, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    ventana_principal->image113 =
	gtk_image_new_from_stock("gtk-dialog-info", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(ventana_principal->image113, "image113");
    gtk_widget_show(ventana_principal->image113);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(ventana_principal->acerca_de1),
				  ventana_principal->image113);

    ventana_principal->scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(ventana_principal->scrolledwindow1, "scrolledwindow1");
    gtk_widget_show(ventana_principal->scrolledwindow1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1), ventana_principal->scrolledwindow1, TRUE, TRUE, 0);

    ventana_principal->viewport1 = gtk_viewport_new(NULL, NULL);
    gtk_widget_set_name(ventana_principal->viewport1, "viewport1");
    gtk_widget_show(ventana_principal->viewport1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->scrolledwindow1), ventana_principal->viewport1);

    ventana_principal->fixed1 = gtk_fixed_new();
    gtk_widget_set_name(ventana_principal->fixed1, "fixed1");
    gtk_widget_show(ventana_principal->fixed1);
    gtk_container_add(GTK_CONTAINER(ventana_principal->viewport1), ventana_principal->fixed1);

    ventana_principal->statusbar1 = gtk_statusbar_new();
    gtk_widget_set_name(ventana_principal->statusbar1, "statusbar1");
    gtk_widget_show(ventana_principal->statusbar1);
    gtk_box_pack_start(GTK_BOX(ventana_principal->vbox1), ventana_principal->statusbar1, FALSE, FALSE, 0);

    g_signal_connect((gpointer) ventana_principal->window1, "delete_event",
		     G_CALLBACK(on_window1_delete_event), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->window1, "destroy",
		     G_CALLBACK(on_window1_destroy), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->window1, "show",
		     G_CALLBACK(on_window1_show), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->nuevo2, "activate",
		     G_CALLBACK(on_nuevo2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->abrir2, "activate",
		     G_CALLBACK(on_abrir2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->guardar_como2, "activate",
		     G_CALLBACK(on_guardar_como2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->guardar2, "activate",
		     G_CALLBACK(on_guardar2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->salir2, "activate",
		     G_CALLBACK(on_salir2_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->pipeline1, "activate",
		     G_CALLBACK(on_pipeline1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->crear1, "activate",
		     G_CALLBACK(on_crear1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->propiedades1, "activate",
		     G_CALLBACK(on_propiedades1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->conectar1, "activate",
		     G_CALLBACK(on_conectar1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->borrar1, "activate",
		     G_CALLBACK(on_borrar1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->ayuda1, "activate",
		     G_CALLBACK(on_ayuda1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->__ndice_de_la_ayuda1, "activate",
		     G_CALLBACK(on___ndice_de_la_ayuda1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->acerca_de1, "activate",
		     G_CALLBACK(on_acerca_de1_activate), ventana_principal);
    g_signal_connect((gpointer) ventana_principal->propiedades_biblioteca, "activate",
		     G_CALLBACK(on_propiedades_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->ciclo_biblioteca, "activate",
		     G_CALLBACK(on_ciclo_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->ciclos_biblioteca, "activate",
		     G_CALLBACK(on_ciclos_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->parar_biblioteca, "activate",
		     G_CALLBACK(on_parar_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->iniciar_biblioteca, "activate",
		     G_CALLBACK(on_iniciar_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->iniciar_todas_biblioteca, "activate",
		     G_CALLBACK(on_iniciar_todas_biblioteca_activate),
		     ventana_principal);

    g_signal_connect((gpointer) ventana_principal->cerrar_biblioteca, "activate",
		     G_CALLBACK(on_cerrar_biblioteca_activate), ventana_principal);

    g_signal_connect((gpointer) ventana_principal->cerrar_todas_biblioteca, "activate",
		     G_CALLBACK(on_cerrar_todas_biblioteca_activate),
		     ventana_principal);

    ventana_principal->modificado = FALSE;
    /* Store pointers to all widgets, for use by lookup_widget(). */
    /*GLADE_HOOKUP_OBJECT_NO_REF(window1, window1, "window1");
    GLADE_HOOKUP_OBJECT(window1, vbox1, "vbox1");
    GLADE_HOOKUP_OBJECT(window1, menubar1, "menubar1");
    GLADE_HOOKUP_OBJECT(window1, archivo1, "archivo1");
    GLADE_HOOKUP_OBJECT(window1, archivo1_menu, "archivo1_menu");
    GLADE_HOOKUP_OBJECT(window1, nuevo2, "nuevo2");
    GLADE_HOOKUP_OBJECT(window1, image103, "image103");
    GLADE_HOOKUP_OBJECT(window1, abrir2, "abrir2");
    GLADE_HOOKUP_OBJECT(window1, image104, "image104");
    GLADE_HOOKUP_OBJECT(window1, separador1, "separador1");
    GLADE_HOOKUP_OBJECT(window1, guardar_como2, "guardar_como2");
    GLADE_HOOKUP_OBJECT(window1, image105, "image105");

    GLADE_HOOKUP_OBJECT(window1, ciclos_biblioteca, "ciclos_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, ciclo_biblioteca, "ciclo_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, parar_biblioteca, "parar_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, iniciar_biblioteca, "iniciar_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, iniciar_todas_biblioteca,
			"iniciar_todas_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, cerrar_todas_biblioteca,
			"cerrar_todas_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, cerrar_biblioteca, "cerrar_biblioteca");

    GLADE_HOOKUP_OBJECT(window1, guardar2, "guardar2");
    GLADE_HOOKUP_OBJECT(window1, image106, "image106");
    GLADE_HOOKUP_OBJECT(window1, separador2, "separador2");
    GLADE_HOOKUP_OBJECT(window1, salir2, "salir2");
    GLADE_HOOKUP_OBJECT(window1, image107, "image107");
    GLADE_HOOKUP_OBJECT(window1, pipeline1, "pipeline1");
    GLADE_HOOKUP_OBJECT(window1, pipeline1_menu, "pipeline1_menu");
    GLADE_HOOKUP_OBJECT(window1, crear1, "crear1");
    GLADE_HOOKUP_OBJECT(window1, image108, "image108");
    GLADE_HOOKUP_OBJECT(window1, propiedades1, "propiedades1");
    GLADE_HOOKUP_OBJECT(window1, propiedades_biblioteca,
			"propiedades_biblioteca");
    GLADE_HOOKUP_OBJECT(window1, image109, "image109");
    GLADE_HOOKUP_OBJECT(window1, conectar1, "conectar1");
    GLADE_HOOKUP_OBJECT(window1, image110, "image110");
    GLADE_HOOKUP_OBJECT(window1, separador3, "separador3");
    GLADE_HOOKUP_OBJECT(window1, borrar1, "borrar1");
    GLADE_HOOKUP_OBJECT(window1, image111, "image111");
    GLADE_HOOKUP_OBJECT(window1, ayuda1, "ayuda1");
    GLADE_HOOKUP_OBJECT(window1, ayuda1_menu, "ayuda1_menu");
    GLADE_HOOKUP_OBJECT(window1, __ndice_de_la_ayuda1,
			"__ndice_de_la_ayuda1");
    GLADE_HOOKUP_OBJECT(window1, image112, "image112");
    GLADE_HOOKUP_OBJECT(window1, separador4, "separador4");
    GLADE_HOOKUP_OBJECT(window1, separador5, "separador5");
    GLADE_HOOKUP_OBJECT(window1, acerca_de1, "acerca_de1");
    GLADE_HOOKUP_OBJECT(window1, image113, "image113");
    GLADE_HOOKUP_OBJECT(window1, scrolledwindow1, "scrolledwindow1");
    GLADE_HOOKUP_OBJECT(window1, viewport1, "viewport1");
    GLADE_HOOKUP_OBJECT(window1, fixed1, "fixed1");
    GLADE_HOOKUP_OBJECT(window1, statusbar1, "statusbar1");*/

    gtk_window_add_accel_group(GTK_WINDOW(ventana_principal->window1), ventana_principal->accel_group);

    return ventana_principal;
}


void on_borrar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    if (confirmacion
	(ventana_principal->window1,
	 "\302\277 Realmente desea borrar los m\303\263dulos elegidos ?"))
    {
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    if (gtk_toggle_button_get_active
		(GTK_TOGGLE_BUTTON(ventana_principal->pipeline->m_elemento[i].m_widget))) {
		borrar(ventana_principal->pipeline, i);
		i--;
		ventana_principal->modificado = TRUE;
	    }
	}
	establecer(ventana_principal->pipeline, ventana_principal);
    }
}


void on_acerca_de1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    acerca_de(ventana_principal->window1);
}


void on_nuevo2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    if (confirmacion
	(ventana_principal->window1,
	 "\302\277 Realmente desea eliminar todos los m\303\263dulos del pipeline ?"))
    {
	vaciar_pipeline(ventana_principal->pipeline);
	ventana_principal->pipeline = crear_pipeline();
	ventana_principal->archivo[0] = '\0';
	mostrar(GTK_STATUSBAR(ventana_principal->statusbar1), 0, &ventana_principal->id);
	ventana_principal->modificado = FALSE;
	gtk_widget_set_sensitive(/*lookup_widget(ventana_principal->window1, "nuevo2")*/ventana_principal->nuevo2, FALSE);
	gtk_widget_set_sensitive(/*lookup_widget(ventana_principal->window1, "guardar2")*/ventana_principal->guardar2,
				 FALSE);
	gtk_widget_set_sensitive(/*lookup_widget(ventana_principal->window1, "guardar_como2")*/ventana_principal->guardar_como2,
				 FALSE);
	establecer(ventana_principal->pipeline, ventana_principal);
    }

}

void abre(char *file, ventana_principal_t * ventana_principal)
{
    if (file != 0) {
	vaciar_pipeline(ventana_principal->pipeline);
	ventana_principal->pipeline = cargar(file);
	strcpy(ventana_principal->archivo, file);
	ventana_principal->modificado = FALSE;
	mostrar(GTK_STATUSBAR(ventana_principal->statusbar1), ventana_principal->archivo, &ventana_principal->id);
	GtkWidget *fixed = ventana_principal->fixed1;
	insertar(GTK_FIXED(fixed), ventana_principal->pipeline);
	gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2,
				 TRUE);
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    g_signal_connect((gpointer) ventana_principal->pipeline->m_elemento[i].m_widget,
			     "toggled", G_CALLBACK(pinchado), ventana_principal);
	}
	establecer(ventana_principal->pipeline, ventana_principal);
    }

}


void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{  
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
  gchar * file = abrir_ventana(ventana_principal->window1);
  if(file) {
    abre(file, ventana_principal);
    g_free(file);
  }
}


void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
  cerrar_todas_bibliotecas(ventana_principal->pipeline);
  gchar *file = guardar_ventana(ventana_principal->window1);
  if(file) {
    guardar_como_aux(ventana_principal->pipeline,
		     file, ventana_principal->window1, ventana_principal->statusbar1, ventana_principal->archivo, &ventana_principal->id, &ventana_principal->modificado, ventana_principal);
    g_free(file);
  }
}


void on_guardar2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    gchar *buffer;
    if (ventana_principal->archivo[0] != '\0') {
	buffer = ventana_principal->archivo;
    } else {
	buffer = guardar_ventana(ventana_principal->window1);
    }
    guardar_como_aux(ventana_principal->pipeline,
		     buffer,
		     ventana_principal->window1, ventana_principal->statusbar1, ventana_principal->archivo, &ventana_principal->id, &ventana_principal->modificado, ventana_principal);

    if (buffer != ventana_principal->archivo) {
	g_free(buffer);
    }
}


void on_salir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    if (salir(ventana_principal->window1, ventana_principal->modificado)) {
	gtk_main_quit();
    }
}


void on_pipeline1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

void pinchado(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    establecer(ventana_principal->pipeline, ventana_principal);
}

void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
 GtkWidget *fixed = ventana_principal->fixed1;//lookup_widget(window1, "fixed1");
    propiedades(-1, GTK_FIXED(fixed), ventana_principal->pipeline, &ventana_principal->modificado);
    if (ventana_principal->modificado) {
      gtk_widget_set_sensitive(/*lookup_widget(window1, "nuevo2")*/ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(/*lookup_widget(window1, "guardar2")*/ventana_principal->guardar2, TRUE);
	gtk_widget_set_sensitive(/*lookup_widget(window1, "guardar_como2")*/ventana_principal->guardar_como2,
				 TRUE);
	g_signal_connect((gpointer) ventana_principal->pipeline->
			 m_elemento[ventana_principal->pipeline->m_numero - 1].m_widget,
			 "toggled", G_CALLBACK(pinchado), ventana_principal);
	establecer(ventana_principal->pipeline, ventana_principal);
    }
}


void on_conectar1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    gint destino = elegir_modulo(ventana_principal->window1, ventana_principal->pipeline);

    if (destino != -1) {
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    if (gtk_toggle_button_get_active
		(GTK_TOGGLE_BUTTON(ventana_principal->pipeline->m_elemento[i].m_widget))) {
		if (conectar(ventana_principal->pipeline, i, destino) == -1) {
		    info(ventana_principal->window1,
			 "No se pudo realizar la conexi\303\263n.");
		}
		else {
		  expose_event(0, 0, ventana_principal);
		}
	    }
	}
	//    establecer(pipeline, window1);
    }

}


void on_ayuda1_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

gboolean
on_window1_delete_event(GtkWidget * widget,
			GdkEvent * event, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    return !salir(ventana_principal->window1, ventana_principal->modificado);
}

void on_window1_destroy(GtkObject * object, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    vaciar_pipeline(ventana_principal->pipeline);
    gtk_main_quit();
}

gint expose_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)data;
  int i, j;

  for(i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
    int x_origen = ventana_principal->pipeline->m_elemento[i].m_x;
    int y_origen = ventana_principal->pipeline->m_elemento[i].m_y;
    for(j = 0; j < ventana_principal->pipeline->m_elemento[i].m_numero_conexiones; ++j) {
      int x_destino = ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_x;
      int y_destino = ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_y;
      gdk_draw_line (ventana_principal->ww, ventana_principal->gc, x_origen,
		     y_origen,
		     x_destino,
		       y_destino);
    }
  }
  
  return FALSE;
}


void on_window1_show(GtkWidget * widget, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
//ventana_principal->status_bar = ventana_principal->statusbar1;//lookup_widget(window1, "statusbar1");
    ventana_principal->id = gtk_statusbar_push(GTK_STATUSBAR(ventana_principal->statusbar1), 0,
			    "Visi\303\263n por computador");
    ventana_principal->pipeline = crear_pipeline();
    senyal(ventana_principal);
    /*if (user_data != 0) {
	abre((char *) user_data);
	}*/

    GtkWidget *fixed = ventana_principal->fixed1;//lookup_widget(window1, "fixed1");
    ventana_principal->ww = fixed->window;
    ventana_principal->gc = gdk_gc_new(ventana_principal->ww);
    gtk_signal_connect (GTK_OBJECT(fixed), "expose_event",
			GTK_SIGNAL_FUNC(expose_event), ventana_principal);
}

void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    GtkWidget *fixed = ventana_principal->fixed1;//lookup_widget(window1, "fixed1");
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->pipeline->m_elemento[i].m_widget))) {
	    propiedades(i, GTK_FIXED(fixed), ventana_principal->pipeline, &ventana_principal->modificado);
	}
    }
    if (ventana_principal->modificado) {
      gtk_widget_set_sensitive(/*lookup_widget(window1, "nuevo2")*/ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(/*lookup_widget(window1, "guardar2")*/ventana_principal->guardar2, TRUE);
	gtk_widget_set_sensitive(/*lookup_widget(window1, "guardar_como2")*/ventana_principal->guardar_como2,
				 TRUE);
    }
}

void
on___ndice_de_la_ayuda1_activate(GtkMenuItem * menuitem,
				 gpointer user_data)
{
}


void on_propiedades_biblioteca_activate(GtkButton * button,
					gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado) {
	    if (ventana_principal->pipeline->m_elemento[i].m_funcion_propiedades) {
		ventana_principal->pipeline->m_elemento[i].m_funcion_propiedades();
	    }
	}
    }

}
void on_ciclos_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    crear_timer(RETARDO);
}

void on_iniciar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->pipeline->m_elemento[i].m_widget))) {
	    iniciar(&ventana_principal->pipeline->m_elemento[i]);

	}
    }
    establecer(ventana_principal->pipeline, ventana_principal);
}

void on_iniciar_todas_biblioteca_activate(GtkButton * button,
					  gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	iniciar(&ventana_principal->pipeline->m_elemento[i]);
    }
    establecer(ventana_principal->pipeline, ventana_principal);
}


void on_parar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
    parar_timer();
}

void on_cerrar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->pipeline->m_elemento[i].m_widget))) {
	    parar(&ventana_principal->pipeline->m_elemento[i]);
	}
    }
    establecer(ventana_principal->pipeline, ventana_principal);
}

void on_cerrar_todas_biblioteca_activate(GtkButton * button,
					 gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	parar(&ventana_principal->pipeline->m_elemento[i]);
    }
    establecer(ventana_principal->pipeline, ventana_principal);
}

void on_ciclo_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    haz_un_ciclo(ventana_principal->pipeline);
}
