/*! \file interface.c
    \brief Funciones auxiliares para la aplicación de pipeline. Generado inicialmente por Glade
    \author zosco
    \version 0.1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget *create_window1(void)
{
    GtkWidget *window1;
    GdkPixbuf *window1_icon_pixbuf;
    GtkWidget *vbox1;
    GtkWidget *menubar1;
    GtkWidget *archivo1;
    GtkWidget *archivo1_menu;
    GtkWidget *nuevo2;
    GtkWidget *image103;
    GtkWidget *abrir2;
    GtkWidget *image104;
    GtkWidget *separador1;
    GtkWidget *guardar_como2;
    GtkWidget *image105;
    GtkWidget *guardar2;
    GtkWidget *image106;
    GtkWidget *separador2;
    GtkWidget *salir2;
    GtkWidget *image107;
    GtkWidget *pipeline1;
    GtkWidget *pipeline1_menu;
    GtkWidget *crear1;
    GtkWidget *image108;
    GtkWidget *propiedades1;
    GtkWidget *image109;
    GtkWidget *conectar1;
    GtkWidget *image110;
    GtkWidget *separador3;
    GtkWidget *borrar1;
    GtkWidget *image111;
    GtkWidget *ayuda1;
    GtkWidget *ayuda1_menu;
    GtkWidget *__ndice_de_la_ayuda1;
    GtkWidget *image112;
    GtkWidget *separador4;
    GtkWidget *acerca_de1;
    GtkWidget *image113;
    GtkWidget *scrolledwindow1;
    GtkWidget *viewport1;
    GtkWidget *fixed1;
    GtkWidget *statusbar1;
    GtkAccelGroup *accel_group;

    accel_group = gtk_accel_group_new();

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(window1, "window1");
    gtk_window_set_title(GTK_WINDOW(window1),
			 "Visi\303\263n por computador - Pipeline");
    gtk_window_set_default_size(GTK_WINDOW(window1), 500, 400);
    window1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (window1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(window1), window1_icon_pixbuf);
	gdk_pixbuf_unref(window1_icon_pixbuf);
    }

    vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(vbox1, "vbox1");
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(window1), vbox1);

    menubar1 = gtk_menu_bar_new();
    gtk_widget_set_name(menubar1, "menubar1");
    gtk_widget_show(menubar1);
    gtk_box_pack_start(GTK_BOX(vbox1), menubar1, FALSE, FALSE, 0);

    archivo1 = gtk_menu_item_new_with_mnemonic("_Archivo");
    gtk_widget_set_name(archivo1, "archivo1");
    gtk_widget_show(archivo1);
    gtk_container_add(GTK_CONTAINER(menubar1), archivo1);

    archivo1_menu = gtk_menu_new();
    gtk_widget_set_name(archivo1_menu, "archivo1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(archivo1), archivo1_menu);

    nuevo2 = gtk_image_menu_item_new_with_mnemonic("_Nuevo");
    gtk_widget_set_name(nuevo2, "nuevo2");
    gtk_widget_show(nuevo2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), nuevo2);
    gtk_widget_set_sensitive(nuevo2, FALSE);
    gtk_widget_add_accelerator(nuevo2, "activate", accel_group,
			       GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    image103 = gtk_image_new_from_stock("gtk-new", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image103, "image103");
    gtk_widget_show(image103);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(nuevo2), image103);

    abrir2 = gtk_image_menu_item_new_with_mnemonic("_Abrir...");
    gtk_widget_set_name(abrir2, "abrir2");
    gtk_widget_show(abrir2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), abrir2);
    gtk_widget_add_accelerator(abrir2, "activate", accel_group,
			       GDK_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    image104 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image104, "image104");
    gtk_widget_show(image104);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(abrir2), image104);

    separador1 = gtk_separator_menu_item_new();
    gtk_widget_set_name(separador1, "separador1");
    gtk_widget_show(separador1);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), separador1);
    gtk_widget_set_sensitive(separador1, FALSE);

    guardar_como2 =
	gtk_image_menu_item_new_with_mnemonic("Guardar _como...");
    gtk_widget_set_name(guardar_como2, "guardar_como2");
    gtk_widget_show(guardar_como2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), guardar_como2);
    gtk_widget_set_sensitive(guardar_como2, FALSE);
    gtk_widget_add_accelerator(guardar_como2, "activate", accel_group,
			       GDK_g, GDK_CONTROL_MASK | GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    image105 = gtk_image_new_from_stock("gtk-save-as", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image105, "image105");
    gtk_widget_show(image105);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(guardar_como2),
				  image105);

    guardar2 = gtk_image_menu_item_new_with_mnemonic("_Guardar");
    gtk_widget_set_name(guardar2, "guardar2");
    gtk_widget_show(guardar2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), guardar2);
    gtk_widget_set_sensitive(guardar2, FALSE);
    gtk_widget_add_accelerator(guardar2, "activate", accel_group,
			       GDK_g, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    image106 = gtk_image_new_from_stock("gtk-save", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image106, "image106");
    gtk_widget_show(image106);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(guardar2), image106);

    separador2 = gtk_separator_menu_item_new();
    gtk_widget_set_name(separador2, "separador2");
    gtk_widget_show(separador2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), separador2);
    gtk_widget_set_sensitive(separador2, FALSE);

    salir2 = gtk_image_menu_item_new_with_mnemonic("_Salir");
    gtk_widget_set_name(salir2, "salir2");
    gtk_widget_show(salir2);
    gtk_container_add(GTK_CONTAINER(archivo1_menu), salir2);
    gtk_widget_add_accelerator(salir2, "activate", accel_group,
			       GDK_F4, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);

    image107 = gtk_image_new_from_stock("gtk-quit", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image107, "image107");
    gtk_widget_show(image107);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(salir2), image107);

    pipeline1 = gtk_menu_item_new_with_mnemonic("_Pipeline");
    gtk_widget_set_name(pipeline1, "pipeline1");
    gtk_widget_show(pipeline1);
    gtk_container_add(GTK_CONTAINER(menubar1), pipeline1);

    pipeline1_menu = gtk_menu_new();
    gtk_widget_set_name(pipeline1_menu, "pipeline1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(pipeline1), pipeline1_menu);

    crear1 = gtk_image_menu_item_new_with_mnemonic("_Crear");
    gtk_widget_set_name(crear1, "crear1");
    gtk_widget_show(crear1);
    gtk_container_add(GTK_CONTAINER(pipeline1_menu), crear1);
    gtk_widget_add_accelerator(crear1, "activate", accel_group,
			       GDK_Insert, 0, GTK_ACCEL_VISIBLE);

    image108 = gtk_image_new_from_stock("gtk-add", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image108, "image108");
    gtk_widget_show(image108);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(crear1), image108);

    propiedades1 = gtk_image_menu_item_new_with_mnemonic("_Propiedades");
    gtk_widget_set_name(propiedades1, "propiedades1");
    gtk_widget_show(propiedades1);
    gtk_container_add(GTK_CONTAINER(pipeline1_menu), propiedades1);
    gtk_widget_set_sensitive(propiedades1, FALSE);
    gtk_widget_add_accelerator(propiedades1, "activate", accel_group,
			       GDK_Return, GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    image109 =
	gtk_image_new_from_stock("gtk-preferences", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image109, "image109");
    gtk_widget_show(image109);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(propiedades1),
				  image109);

    conectar1 = gtk_image_menu_item_new_with_mnemonic("C_onectar");
    gtk_widget_set_name(conectar1, "conectar1");
    gtk_widget_show(conectar1);
    gtk_container_add(GTK_CONTAINER(pipeline1_menu), conectar1);
    gtk_widget_set_sensitive(conectar1, FALSE);
    gtk_widget_add_accelerator(conectar1, "activate", accel_group,
			       GDK_space, 0, GTK_ACCEL_VISIBLE);

    image110 = gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image110, "image110");
    gtk_widget_show(image110);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(conectar1),
				  image110);

    separador3 = gtk_separator_menu_item_new();
    gtk_widget_set_name(separador3, "separador3");
    gtk_widget_show(separador3);
    gtk_container_add(GTK_CONTAINER(pipeline1_menu), separador3);
    gtk_widget_set_sensitive(separador3, FALSE);

    borrar1 = gtk_image_menu_item_new_with_mnemonic("_Borrar");
    gtk_widget_set_name(borrar1, "borrar1");
    gtk_widget_show(borrar1);
    gtk_container_add(GTK_CONTAINER(pipeline1_menu), borrar1);
    gtk_widget_set_sensitive(borrar1, FALSE);
    gtk_widget_add_accelerator(borrar1, "activate", accel_group,
			       GDK_Delete, 0, GTK_ACCEL_VISIBLE);

    image111 = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image111, "image111");
    gtk_widget_show(image111);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(borrar1), image111);

    ayuda1 = gtk_menu_item_new_with_mnemonic("_Ayuda");
    gtk_widget_set_name(ayuda1, "ayuda1");
    gtk_widget_show(ayuda1);
    gtk_container_add(GTK_CONTAINER(menubar1), ayuda1);

    ayuda1_menu = gtk_menu_new();
    gtk_widget_set_name(ayuda1_menu, "ayuda1_menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ayuda1), ayuda1_menu);

    __ndice_de_la_ayuda1 =
	gtk_image_menu_item_new_with_mnemonic("\303\215ndice de la ayuda");
    gtk_widget_set_name(__ndice_de_la_ayuda1, "__ndice_de_la_ayuda1");
    gtk_widget_show(__ndice_de_la_ayuda1);
    gtk_container_add(GTK_CONTAINER(ayuda1_menu), __ndice_de_la_ayuda1);
    gtk_widget_add_accelerator(__ndice_de_la_ayuda1, "activate",
			       accel_group, GDK_F1, 0, GTK_ACCEL_VISIBLE);

    image112 = gtk_image_new_from_stock("gtk-index", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image112, "image112");
    gtk_widget_show(image112);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM
				  (__ndice_de_la_ayuda1), image112);

    separador4 = gtk_separator_menu_item_new();
    gtk_widget_set_name(separador4, "separador4");
    gtk_widget_show(separador4);
    gtk_container_add(GTK_CONTAINER(ayuda1_menu), separador4);
    gtk_widget_set_sensitive(separador4, FALSE);

    acerca_de1 = gtk_image_menu_item_new_with_mnemonic("_Acerca de...");
    gtk_widget_set_name(acerca_de1, "acerca_de1");
    gtk_widget_show(acerca_de1);
    gtk_container_add(GTK_CONTAINER(ayuda1_menu), acerca_de1);
    gtk_widget_add_accelerator(acerca_de1, "activate", accel_group,
			       GDK_i, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    image113 =
	gtk_image_new_from_stock("gtk-dialog-info", GTK_ICON_SIZE_MENU);
    gtk_widget_set_name(image113, "image113");
    gtk_widget_show(image113);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(acerca_de1),
				  image113);

    scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(scrolledwindow1, "scrolledwindow1");
    gtk_widget_show(scrolledwindow1);
    gtk_box_pack_start(GTK_BOX(vbox1), scrolledwindow1, TRUE, TRUE, 0);

    viewport1 = gtk_viewport_new(NULL, NULL);
    gtk_widget_set_name(viewport1, "viewport1");
    gtk_widget_show(viewport1);
    gtk_container_add(GTK_CONTAINER(scrolledwindow1), viewport1);

    fixed1 = gtk_fixed_new();
    gtk_widget_set_name(fixed1, "fixed1");
    gtk_widget_show(fixed1);
    gtk_container_add(GTK_CONTAINER(viewport1), fixed1);

    statusbar1 = gtk_statusbar_new();
    gtk_widget_set_name(statusbar1, "statusbar1");
    gtk_widget_show(statusbar1);
    gtk_box_pack_start(GTK_BOX(vbox1), statusbar1, FALSE, FALSE, 0);

    g_signal_connect((gpointer) window1, "delete_event",
		     G_CALLBACK(on_window1_delete_event), NULL);
    g_signal_connect((gpointer) window1, "destroy",
		     G_CALLBACK(on_window1_destroy), NULL);
    g_signal_connect((gpointer) window1, "show",
		     G_CALLBACK(on_window1_show), NULL);
    g_signal_connect((gpointer) nuevo2, "activate",
		     G_CALLBACK(on_nuevo2_activate), NULL);
    g_signal_connect((gpointer) abrir2, "activate",
		     G_CALLBACK(on_abrir2_activate), NULL);
    g_signal_connect((gpointer) guardar_como2, "activate",
		     G_CALLBACK(on_guardar_como2_activate), NULL);
    g_signal_connect((gpointer) guardar2, "activate",
		     G_CALLBACK(on_guardar2_activate), NULL);
    g_signal_connect((gpointer) salir2, "activate",
		     G_CALLBACK(on_salir2_activate), NULL);
    g_signal_connect((gpointer) pipeline1, "activate",
		     G_CALLBACK(on_pipeline1_activate), NULL);
    g_signal_connect((gpointer) crear1, "activate",
		     G_CALLBACK(on_crear1_activate), NULL);
    g_signal_connect((gpointer) propiedades1, "activate",
		     G_CALLBACK(on_propiedades1_activate), NULL);
    g_signal_connect((gpointer) conectar1, "activate",
		     G_CALLBACK(on_conectar1_activate), NULL);
    g_signal_connect((gpointer) borrar1, "activate",
		     G_CALLBACK(on_borrar1_activate), NULL);
    g_signal_connect((gpointer) ayuda1, "activate",
		     G_CALLBACK(on_ayuda1_activate), NULL);
    g_signal_connect((gpointer) __ndice_de_la_ayuda1, "activate",
		     G_CALLBACK(on___ndice_de_la_ayuda1_activate), NULL);
    g_signal_connect((gpointer) acerca_de1, "activate",
		     G_CALLBACK(on_acerca_de1_activate), NULL);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(window1, window1, "window1");
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
    GLADE_HOOKUP_OBJECT(window1, acerca_de1, "acerca_de1");
    GLADE_HOOKUP_OBJECT(window1, image113, "image113");
    GLADE_HOOKUP_OBJECT(window1, scrolledwindow1, "scrolledwindow1");
    GLADE_HOOKUP_OBJECT(window1, viewport1, "viewport1");
    GLADE_HOOKUP_OBJECT(window1, fixed1, "fixed1");
    GLADE_HOOKUP_OBJECT(window1, statusbar1, "statusbar1");

    gtk_window_add_accel_group(GTK_WINDOW(window1), accel_group);

    return window1;
}

GtkWidget *create_filechooserdialog1(void)
{
    GtkWidget *filechooserdialog1;
    GdkPixbuf *filechooserdialog1_icon_pixbuf;
    GtkWidget *dialog_vbox1;
    GtkWidget *dialog_action_area1;
    GtkWidget *button2;
    GtkWidget *button3;

    filechooserdialog1 =
	gtk_file_chooser_dialog_new("Abrir...", NULL,
				    GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
    gtk_widget_set_name(filechooserdialog1, "filechooserdialog1");
    filechooserdialog1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (filechooserdialog1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(filechooserdialog1),
			    filechooserdialog1_icon_pixbuf);
	gdk_pixbuf_unref(filechooserdialog1_icon_pixbuf);
    }

    dialog_vbox1 = GTK_DIALOG(filechooserdialog1)->vbox;
    gtk_widget_set_name(dialog_vbox1, "dialog_vbox1");
    gtk_widget_show(dialog_vbox1);

    dialog_action_area1 = GTK_DIALOG(filechooserdialog1)->action_area;
    gtk_widget_set_name(dialog_action_area1, "dialog_action_area1");
    gtk_widget_show(dialog_action_area1);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area1),
			      GTK_BUTTONBOX_END);

    button2 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(button2, "button2");
    gtk_widget_show(button2);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog1), button2,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button2, GTK_CAN_DEFAULT);

    button3 = gtk_button_new_from_stock("gtk-open");
    gtk_widget_set_name(button3, "button3");
    gtk_widget_show(button3);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog1), button3,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button3, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, filechooserdialog1,
			       "filechooserdialog1");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_vbox1,
			       "dialog_vbox1");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_action_area1,
			       "dialog_action_area1");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button2, "button2");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button3, "button3");

    gtk_widget_grab_default(button3);
    return filechooserdialog1;
}

GtkWidget *create_filechooserdialog2(void)
{
    GtkWidget *filechooserdialog2;
    GdkPixbuf *filechooserdialog2_icon_pixbuf;
    GtkWidget *dialog_vbox2;
    GtkWidget *dialog_action_area2;
    GtkWidget *button4;
    GtkWidget *button5;

    filechooserdialog2 =
	gtk_file_chooser_dialog_new("Guardar como...", NULL,
				    GTK_FILE_CHOOSER_ACTION_SAVE, NULL);
    gtk_widget_set_name(filechooserdialog2, "filechooserdialog2");
    filechooserdialog2_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (filechooserdialog2_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(filechooserdialog2),
			    filechooserdialog2_icon_pixbuf);
	gdk_pixbuf_unref(filechooserdialog2_icon_pixbuf);
    }

    dialog_vbox2 = GTK_DIALOG(filechooserdialog2)->vbox;
    gtk_widget_set_name(dialog_vbox2, "dialog_vbox2");
    gtk_widget_show(dialog_vbox2);

    dialog_action_area2 = GTK_DIALOG(filechooserdialog2)->action_area;
    gtk_widget_set_name(dialog_action_area2, "dialog_action_area2");
    gtk_widget_show(dialog_action_area2);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area2),
			      GTK_BUTTONBOX_END);

    button4 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(button4, "button4");
    gtk_widget_show(button4);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog2), button4,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button4, GTK_CAN_DEFAULT);

    button5 = gtk_button_new_from_stock("gtk-open");
    gtk_widget_set_name(button5, "button5");
    gtk_widget_show(button5);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog2), button5,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button5, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, filechooserdialog2,
			       "filechooserdialog2");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, dialog_vbox2,
			       "dialog_vbox2");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog2, dialog_action_area2,
			       "dialog_action_area2");
    GLADE_HOOKUP_OBJECT(filechooserdialog2, button4, "button4");
    GLADE_HOOKUP_OBJECT(filechooserdialog2, button5, "button5");

    gtk_widget_grab_default(button5);
    return filechooserdialog2;
}

GtkWidget *create_dialog1(void)
{
    GtkWidget *dialog1;
    GdkPixbuf *dialog1_icon_pixbuf;
    GtkWidget *dialog_vbox3;
    GtkWidget *vbox3;
    GtkWidget *label3;
    GtkWidget *entry1;
    GtkWidget *dialog_action_area3;
    GtkWidget *cancelbutton1;
    GtkWidget *okbutton1;

    dialog1 = gtk_dialog_new();
    gtk_widget_set_name(dialog1, "dialog1");
    gtk_window_set_title(GTK_WINDOW(dialog1), "Entrada");
    gtk_window_set_modal(GTK_WINDOW(dialog1), TRUE);
    dialog1_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (dialog1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(dialog1), dialog1_icon_pixbuf);
	gdk_pixbuf_unref(dialog1_icon_pixbuf);
    }

    dialog_vbox3 = GTK_DIALOG(dialog1)->vbox;
    gtk_widget_set_name(dialog_vbox3, "dialog_vbox3");
    gtk_widget_show(dialog_vbox3);

    vbox3 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(vbox3, "vbox3");
    gtk_widget_show(vbox3);
    gtk_box_pack_start(GTK_BOX(dialog_vbox3), vbox3, TRUE, TRUE, 0);

    label3 = gtk_label_new("label3");
    gtk_widget_set_name(label3, "label3");
    gtk_widget_show(label3);
    gtk_box_pack_start(GTK_BOX(vbox3), label3, FALSE, FALSE, 0);

    entry1 = gtk_entry_new();
    gtk_widget_set_name(entry1, "entry1");
    gtk_widget_show(entry1);
    gtk_box_pack_start(GTK_BOX(vbox3), entry1, FALSE, FALSE, 0);

    dialog_action_area3 = GTK_DIALOG(dialog1)->action_area;
    gtk_widget_set_name(dialog_action_area3, "dialog_action_area3");
    gtk_widget_show(dialog_action_area3);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area3),
			      GTK_BUTTONBOX_END);

    cancelbutton1 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(cancelbutton1, "cancelbutton1");
    gtk_widget_show(cancelbutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog1), cancelbutton1,
				 GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(cancelbutton1, GTK_CAN_DEFAULT);

    okbutton1 = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_set_name(okbutton1, "okbutton1");
    gtk_widget_show(okbutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog1), okbutton1,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(okbutton1, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog1, "dialog1");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog_vbox3, "dialog_vbox3");
    GLADE_HOOKUP_OBJECT(dialog1, vbox3, "vbox3");
    GLADE_HOOKUP_OBJECT(dialog1, label3, "label3");
    GLADE_HOOKUP_OBJECT(dialog1, entry1, "entry1");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog1, dialog_action_area3,
			       "dialog_action_area3");
    GLADE_HOOKUP_OBJECT(dialog1, cancelbutton1, "cancelbutton1");
    GLADE_HOOKUP_OBJECT(dialog1, okbutton1, "okbutton1");

    gtk_widget_grab_focus(entry1);
    gtk_widget_grab_default(okbutton1);
    return dialog1;
}




GtkWidget *create_propiedades_modulo(void)
{
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

    propiedades_modulo = gtk_dialog_new();
    gtk_widget_set_name(propiedades_modulo, "propiedades_modulo");
    gtk_container_set_border_width(GTK_CONTAINER(propiedades_modulo), 2);
    gtk_window_set_title(GTK_WINDOW(propiedades_modulo),
			 "Propiedades del m\303\263dulo");
    propiedades_modulo_icon_pixbuf = create_pixbuf("pipeline.xpm");
    if (propiedades_modulo_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(propiedades_modulo),
			    propiedades_modulo_icon_pixbuf);
	gdk_pixbuf_unref(propiedades_modulo_icon_pixbuf);
    }

    dialog_vbox4 = GTK_DIALOG(propiedades_modulo)->vbox;
    gtk_widget_set_name(dialog_vbox4, "dialog_vbox4");
    gtk_widget_show(dialog_vbox4);

    vbox4 = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(vbox4, "vbox4");
    gtk_widget_show(vbox4);
    gtk_box_pack_start(GTK_BOX(dialog_vbox4), vbox4, TRUE, TRUE, 0);

    label4 = gtk_label_new_with_mnemonic("_Etiqueta");
    gtk_widget_set_name(label4, "label4");
    gtk_widget_show(label4);
    gtk_box_pack_start(GTK_BOX(vbox4), label4, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(label4), 0, 2);

    entry2 = gtk_entry_new();
    gtk_widget_set_name(entry2, "entry2");
    gtk_widget_show(entry2);
    gtk_box_pack_start(GTK_BOX(vbox4), entry2, FALSE, FALSE, 0);

    hseparator1 = gtk_hseparator_new();
    gtk_widget_set_name(hseparator1, "hseparator1");
    gtk_widget_show(hseparator1);
    gtk_box_pack_start(GTK_BOX(vbox4), hseparator1, TRUE, TRUE, 2);

    label5 = gtk_label_new("Coordenadas");
    gtk_widget_set_name(label5, "label5");
    gtk_widget_show(label5);
    gtk_box_pack_start(GTK_BOX(vbox4), label5, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(label5), 0, 2);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_name(hbox1, "hbox1");
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox4), hbox1, TRUE, TRUE, 0);

    label6 = gtk_label_new_with_mnemonic("_X");
    gtk_widget_set_name(label6, "label6");
    gtk_widget_show(label6);
    gtk_box_pack_start(GTK_BOX(hbox1), label6, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(label6), 5, 0);

    entry3 = gtk_entry_new();
    gtk_widget_set_name(entry3, "entry3");
    gtk_widget_show(entry3);
    gtk_box_pack_start(GTK_BOX(hbox1), entry3, TRUE, TRUE, 0);

    label7 = gtk_label_new_with_mnemonic("_Y");
    gtk_widget_set_name(label7, "label7");
    gtk_widget_show(label7);
    gtk_box_pack_start(GTK_BOX(hbox1), label7, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(label7), 5, 0);

    entry4 = gtk_entry_new();
    gtk_widget_set_name(entry4, "entry4");
    gtk_widget_show(entry4);
    gtk_box_pack_start(GTK_BOX(hbox1), entry4, TRUE, TRUE, 0);

    hseparator2 = gtk_hseparator_new();
    gtk_widget_set_name(hseparator2, "hseparator2");
    gtk_widget_show(hseparator2);
    gtk_box_pack_start(GTK_BOX(vbox4), hseparator2, TRUE, TRUE, 2);

    label8 = gtk_label_new_with_mnemonic("_Ruta del m\303\263dulo");
    gtk_widget_set_name(label8, "label8");
    gtk_widget_show(label8);
    gtk_box_pack_start(GTK_BOX(vbox4), label8, FALSE, FALSE, 0);
    gtk_misc_set_padding(GTK_MISC(label8), 0, 2);

    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_name(hbox2, "hbox2");
    gtk_widget_show(hbox2);
    gtk_box_pack_start(GTK_BOX(vbox4), hbox2, TRUE, TRUE, 0);

    entry5 = gtk_entry_new();
    gtk_widget_set_name(entry5, "entry5");
    gtk_widget_show(entry5);
    gtk_box_pack_start(GTK_BOX(hbox2), entry5, TRUE, TRUE, 0);

    button6 = gtk_button_new_with_mnemonic("_...");
    gtk_widget_set_name(button6, "button6");
    gtk_widget_show(button6);
    gtk_box_pack_start(GTK_BOX(hbox2), button6, FALSE, FALSE, 0);
    g_signal_connect((gpointer) button6, "clicked",
		     G_CALLBACK(on_button6_activate), propiedades_modulo);

    dialog_action_area4 = GTK_DIALOG(propiedades_modulo)->action_area;
    gtk_widget_set_name(dialog_action_area4, "dialog_action_area4");
    gtk_widget_show(dialog_action_area4);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area4),
			      GTK_BUTTONBOX_END);

    cancelbutton2 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_set_name(cancelbutton2, "cancelbutton2");
    gtk_widget_show(cancelbutton2);
    gtk_dialog_add_action_widget(GTK_DIALOG(propiedades_modulo),
				 cancelbutton2, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(cancelbutton2, GTK_CAN_DEFAULT);

    okbutton2 = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_set_name(okbutton2, "okbutton2");
    gtk_widget_show(okbutton2);
    gtk_dialog_add_action_widget(GTK_DIALOG(propiedades_modulo), okbutton2,
				 GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(okbutton2, GTK_CAN_DEFAULT);

    gtk_label_set_mnemonic_widget(GTK_LABEL(label4), entry2);
    gtk_label_set_mnemonic_widget(GTK_LABEL(label6), entry3);
    gtk_label_set_mnemonic_widget(GTK_LABEL(label7), entry4);
    gtk_label_set_mnemonic_widget(GTK_LABEL(label8), entry5);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(propiedades_modulo, propiedades_modulo,
			       "propiedades_modulo");
    GLADE_HOOKUP_OBJECT_NO_REF(propiedades_modulo, dialog_vbox4,
			       "dialog_vbox4");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, vbox4, "vbox4");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, label4, "label4");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, entry2, "entry2");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, hseparator1, "hseparator1");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, label5, "label5");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, hbox1, "hbox1");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, label6, "label6");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, entry3, "entry3");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, label7, "label7");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, entry4, "entry4");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, hseparator2, "hseparator2");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, label8, "label8");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, hbox2, "hbox2");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, entry5, "entry5");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, button6, "button6");
    GLADE_HOOKUP_OBJECT_NO_REF(propiedades_modulo, dialog_action_area4,
			       "dialog_action_area4");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, cancelbutton2,
			"cancelbutton2");
    GLADE_HOOKUP_OBJECT(propiedades_modulo, okbutton2, "okbutton2");

    gtk_widget_grab_default(okbutton2);
    return propiedades_modulo;
}
