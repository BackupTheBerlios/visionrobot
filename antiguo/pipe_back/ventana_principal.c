#include "ventana_principal.h"

//#include <stdlib.h>
#include <gdk/gdkkeysyms.h>
#include "dialogo_abrir.h"
#include "dialogo_guardar.h"
#include "dialogo_conectar.h"
#include "propiedades_modulo.h"
/*#include <sys/time.h>
#include <signal.h>*/
#include <stdlib.h>
#include <string.h>

#include <glib.h>


/*! \brief Presenta un mensaje de entrada, y devuelve la respuesta
    \param w La ventana padre
    \param pregunta El texto que se presenta
    \return La cadena que el usuario escribe
*/
//    gchar *entrada(GtkWidget * w, const gchar * pregunta);

/*! \brief Presenta un mensaje de confirmación, y devuelve la respuesta
    \param w La ventana padre
    \param texto El texto que se presenta
    \return TRUE si se pulsa sí, FALSE en caso contrario
*/
    gboolean confirmacion(GtkWidget * w, const gchar * texto);

/*! \brief Pide confirmación para salir
    \param w La ventana padre
    \param modificado Variable que dice si el pipeline hay que grabarlo
    \return TRUE si se pulsa sí, FALSE en caso contrario
*/
    gboolean salir(GtkWidget * w, gboolean modificado);

/*! \brief Presenta un mensaje de información
    \param w La ventana padre
    \param texto El texto que se presenta
*/
    void info(GtkWidget * w, const gchar * texto);

/*! \brief Enseña ventana de información de la aplicación
    \param w La ventana padre
*/
    void acerca_de(GtkWidget * w);

//    char *abrir_ventana(GtkWidget * w);

/*! \brief Guardar un archivo, mostrando un cuadro de diálogo
    \param w La ventana padre
    \return El puntero a FILE del archivo abierto
*/

    char *guardar_ventana(GtkWidget * w);
/*! \brief Abre la ventana de propiedades de un elemento
    \param id_elemento El id del elemento a modificar, -1 si es un elemento nuevo    
    \param fixed El GtkFixed en el que está el botón    
    \param pipeline El pipe en el que está el elemento
    \param mod Variable de modificación que vamos a cambiar
*/
    void propiedades(gint id_elemento, /*GtkFixed * fixed,
		     pipeline_t * pipeline, gboolean * mod);*/
		     ventana_principal_t * ventana);

    void insertar(/*GtkFixed * fixed, pipeline_t * pipeline*/ventana_principal_t * ventana);

/*! \brief Muestra un mensaje en el StatusBar     
    \param b El puntero al StatusBar    
    \param info El mensaje
    \param id Un id que tiene que ser siempre el mismo
    \warning Esta función es bastante precaria.
*/
    void mostrar(GtkStatusbar * b, const char *info, guint * id);

/*! \brief Establece unos cuantos menús     
    \param window Y una ventana donde están los botones
*/
    void establecer(/*pipeline_t * pipeline, */ventana_principal_t * ventana_principal);

/*! \brief Guarda un pipeline - Se usa para hacer iguales el guardar y el guardar como     
    \param ventana_principal La ventana
    \param file La ruta que se va a salvar
*/
   /* void guardar_como_aux(pipeline_t * pipeline, char *file,
			  GtkWidget * window, GtkWidget * status_bar,
			  char *archivo, guint * id,
			  gboolean * modificado, ventana_principal_t * ventana_principal);
			  */
	  void guardar_como_aux(ventana_principal_t * ventana_principal, const char *archivo);

    /*! \brief Desactiva la ejecución de un elemento
       \param elemento El elemento que paramos
       \return 0 si todo va bien, -1 en caso de error
     */
    int parar(elemento_t * elemento);
    /*!\brief Activa la ejecución de un elemento
       \param pipeline El pipe, claro
       \param elemento El elemento que activamos
       \return 0 si todo va bien, -1 en caso de error
     */
    int iniciar(pipeline_t * pipeline, elemento_t * elemento);
    /*!\brief Activa el temporizador de ciclos del pipeline. Esta función crea una alarma que realiza ciclos seguidos hasta que se para la alarma.
       \param retardo El retardo en microsegundos
       \return 0 si todo va bien, -1 en caso de error
     */

    //int crear_timer(long int retardo);
    /*!\brief Desactiva el temporizador de ciclos
       \return 0 si todo va bien, -1 en caso de error
     */

    //int parar_timer();
    /*!\brief Inicia el sistema de señales
      \param v La ventana principal que gestionará las señales
       \return 0 si todo va bien, -1 en caso de error
     */

//    int senyal(ventana_principal_t * v);
    /*!\brief Efectúa un único ciclo del pipeline
       \param pipeline El pipe sobre el que se ejecuta el ciclo
       \return 0 si todo va bien, -1 en caso de error
     */
     
     
gboolean tick(gpointer data);
int crear_timer(ventana_principal_t  *ventana, guint retardo);
int parar_timer(ventana_principal_t  *ventana);

    //int haz_un_ciclo(pipeline_t * pipeline);

  /*!\brief Presenta una ventana para elegir un módulo destino del pipe
    \param window1 La ventana padre del diálogo
    \param pipeline El pipeline sobre el que se trabaja
    \return El índice del elemento escogido
  */
    int elegir_modulo(GtkWidget * window1, pipeline_t * pipeline);
    
    void abrir(char *file, ventana_principal_t * ventana_principal);


void on_establecer_error(GtkMenuItem * menuitem, gpointer user_data);
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

void pinchado(GtkWidget * menuitem, gpointer user_data);

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
   ventana_principal->m_retardo = 0; 
   ventana_principal->botones = 0;

    ventana_principal->window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(ventana_principal->window1, "window1");
    gtk_window_set_title(GTK_WINDOW(ventana_principal->window1),
			 "Visi\303\263n por computador - Pipeline");
    gtk_window_set_default_size(GTK_WINDOW(ventana_principal->window1), 500, 400);
    
#ifndef WIN32    
    ventana_principal->window1_icon_pixbuf = gdk_pixbuf_new_from_file("pipeline.xpm", 0);
    if (ventana_principal->window1_icon_pixbuf) {
	gtk_window_set_icon(GTK_WINDOW(ventana_principal->window1), ventana_principal->window1_icon_pixbuf);
	gdk_pixbuf_unref(ventana_principal->window1_icon_pixbuf);
    }
#endif

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
			       GDK_p, GDK_CONTROL_MASK,
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
    gtk_widget_add_accelerator(ventana_principal->propiedades_biblioteca, "activate", ventana_principal->accel_group,
			       GDK_f, GDK_CONTROL_MASK,
			       GTK_ACCEL_VISIBLE);

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


    ventana_principal->establecer_error =
	gtk_image_menu_item_new_with_mnemonic("_Establecer m\303\263dulo de error...");
    gtk_widget_set_name(ventana_principal->ciclos_biblioteca, "establecer_error");
    gtk_widget_show(ventana_principal->establecer_error);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->establecer_error);
    gtk_widget_set_sensitive(ventana_principal->establecer_error, FALSE);






    ventana_principal->iniciar_biblioteca =
	gtk_image_menu_item_new_with_mnemonic("Inicia_r m\303\263dulo...");
    gtk_widget_set_name(ventana_principal->iniciar_biblioteca, "iniciar_biblioteca");
    gtk_widget_show(ventana_principal->iniciar_biblioteca);
    gtk_container_add(GTK_CONTAINER(ventana_principal->pipeline1_menu), ventana_principal->iniciar_biblioteca);
    gtk_widget_set_sensitive(ventana_principal->iniciar_biblioteca, FALSE);
        gtk_widget_add_accelerator(ventana_principal->iniciar_biblioteca, "activate", ventana_principal->accel_group,
			       GDK_m, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

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
    g_signal_connect((gpointer) ventana_principal->establecer_error, "activate",
		     G_CALLBACK(on_establecer_error), ventana_principal);
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

    gtk_window_add_accel_group(GTK_WINDOW(ventana_principal->window1), ventana_principal->accel_group);

    return ventana_principal;
}

void borrar_boton(ventana_principal_t * ventana, int id) {
    int i;
    gtk_widget_destroy(GTK_WIDGET(ventana->botones[id].m_widget));

    for (i = id; i < ventana->pipeline->m_numero; ++i) {
      	ventana->botones[i] = ventana->botones[i + 1];
    }
    ventana->botones = (botones_t*)realloc(ventana->botones,
                            sizeof(botones_t) * ventana->pipeline->m_numero - 1);
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
		(GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
		pipeline_borrar(ventana_principal->pipeline, i);
		borrar_boton(ventana_principal, i);		
		i--;
		ventana_principal->modificado = TRUE;
	    }
	}
	establecer(/*ventana_principal->pipeline, */ventana_principal);
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
	pipeline_vaciar(ventana_principal->pipeline);
	ventana_principal->pipeline = pipeline_crear();
	ventana_principal->archivo[0] = '\0';
	mostrar(GTK_STATUSBAR(ventana_principal->statusbar1), 0, &ventana_principal->id);
	ventana_principal->modificado = FALSE;
	gtk_widget_set_sensitive(ventana_principal->nuevo2, FALSE);
	gtk_widget_set_sensitive(ventana_principal->guardar2,
				 FALSE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2,
				 FALSE);
	establecer(/*ventana_principal->pipeline, */ventana_principal);
    }

}
//#include <windows.h>

void on_establecer_error(GtkMenuItem * menuitem, gpointer user_data){     
     ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    gint destino = elegir_modulo(ventana_principal->window1, ventana_principal->pipeline);

    ventana_principal->pipeline->m_error = destino;
}

void on_abrir2_activate(GtkMenuItem * menuitem, gpointer user_data)
{  
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
  gchar * file = dialogo_abrir_ventana(ventana_principal->window1);
  if(file) {
    abrir(file, ventana_principal);
    g_free(file);
  }
}


void on_guardar_como2_activate(GtkMenuItem * menuitem, gpointer user_data)
{
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
  pipeline_cerrar_todas_bibliotecas(ventana_principal->pipeline);
  gchar *file = guardar_ventana(ventana_principal->window1);
  if(file) {
		guardar_como_aux(ventana_principal, file);     
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
    guardar_como_aux(ventana_principal, buffer);

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



void on_crear1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
 GtkWidget *fixed = ventana_principal->fixed1;
    propiedades(-1, /*GTK_FIXED(fixed), ventana_principal->pipeline, &ventana_principal->modificado*/ventana_principal);
    if (ventana_principal->modificado) {
      gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2,
				 TRUE);
	g_signal_connect((gpointer) ventana_principal->botones/*pipeline->
			 m_elemento*/[ventana_principal->pipeline->m_numero - 1].m_widget,
			 "toggled", G_CALLBACK(pinchado), ventana_principal);
	establecer(/*ventana_principal->pipeline, */ventana_principal);
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
		(GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
		if (pipeline_conectar(ventana_principal->pipeline, i, destino) == -1) {
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
    pipeline_vaciar(ventana_principal->pipeline);
    gtk_main_quit();
}

gint expose_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)data;
  int i, j;

  for(i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
    int x_origen = ventana_principal->botones/*pipeline->m_elemento*/[i].m_x;
    int y_origen = ventana_principal->botones/*pipeline->m_elemento*/[i].m_y;
    for(j = 0; j < ventana_principal->pipeline->m_elemento[i].m_numero_conexiones; ++j) {
      int x_destino = //ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_x;
        ventana_principal->botones[ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_id].m_x;
      int y_destino = //ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_y;
      ventana_principal->botones[ventana_principal->pipeline->m_elemento[i].m_destino[j]->m_id].m_y;
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
  
  ventana_principal->id = gtk_statusbar_push(GTK_STATUSBAR(ventana_principal->statusbar1), 0,
					     "Visi\303\263n por computador");
  ventana_principal->pipeline = pipeline_crear();
//  senyal(ventana_principal);
/*ventana = ventana_principal;
  #ifndef WIN32
    signal(SIGALRM, catch_alarm);    
    #endif*/
  
  /** warning */
  ventana_principal->archivo[0] = '\0';
  
  /*if (user_data != 0) {
	abre((char *) user_data);
	}*/
  
  GtkWidget *fixed = ventana_principal->fixed1;
  ventana_principal->ww = fixed->window;
  ventana_principal->gc = gdk_gc_new(ventana_principal->ww);
  gtk_signal_connect (GTK_OBJECT(fixed), "expose_event",
		      GTK_SIGNAL_FUNC(expose_event), ventana_principal);
}

void on_propiedades1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
  ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
  int i;
  GtkWidget *fixed = ventana_principal->fixed1;
  for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
    if (gtk_toggle_button_get_active
	(GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
      propiedades(i, /*GTK_FIXED(fixed), ventana_principal->pipeline, &ventana_principal->modificado*/ventana_principal);
    }
  }
  if (ventana_principal->modificado) {
    gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
    gtk_widget_set_sensitive(ventana_principal->guardar2, TRUE);
    gtk_widget_set_sensitive(ventana_principal->guardar_como2,
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
     if (gtk_toggle_button_get_active
	(GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
	    if (ventana_principal->pipeline->m_elemento[i].m_funcion_propiedades) {
		ventana_principal->pipeline->m_elemento[i].m_funcion_propiedades();
	    }
     }
	}
    }

}
void on_ciclos_biblioteca_activate(GtkButton * button, gpointer user_data)
{
     ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    crear_timer(ventana_principal, ventana_principal->m_retardo);
}

void on_iniciar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
	    pipeline_iniciar(ventana_principal->pipeline, &ventana_principal->pipeline->m_elemento[i]);

	}
    }
    establecer(/*ventana_principal->pipeline, */ventana_principal);
}

void on_iniciar_todas_biblioteca_activate(GtkButton * button,
					  gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    pipeline_iniciar(ventana_principal->pipeline,
        &ventana_principal->pipeline->m_elemento[ventana_principal->pipeline->m_error]);
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if(i != ventana_principal->pipeline->m_error) {
    pipeline_iniciar(ventana_principal->pipeline, &ventana_principal->pipeline->m_elemento[i]);
    }
    }
    establecer(/*ventana_principal->pipeline, */ventana_principal);
}


void on_parar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;     
    parar_timer(ventana_principal);
}

void on_cerrar_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->botones/*pipeline->m_elemento*/[i].m_widget))) {
	    pipeline_parar(ventana_principal->pipeline, &ventana_principal->pipeline->m_elemento[i]);
	}
    }
    establecer(/*ventana_principal->pipeline, */ventana_principal);
}

void on_cerrar_todas_biblioteca_activate(GtkButton * button,
					 gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    int i;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	pipeline_parar(ventana_principal->pipeline, &ventana_principal->pipeline->m_elemento[i]);
    }
    establecer(/*ventana_principal->pipeline, */ventana_principal);
}

void on_ciclo_biblioteca_activate(GtkButton * button, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    pipeline_ciclo(ventana_principal->pipeline);
}



gboolean confirmacion(GtkWidget * w, const gchar * texto)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(w),
					       GTK_DIALOG_MODAL |
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_QUESTION,
					       GTK_BUTTONS_YES_NO,
					       texto);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result == GTK_RESPONSE_YES;
}

gboolean salir(GtkWidget * w, gboolean modificado)
{
    return modificado ? confirmacion(w,
				     "El pipeline ha sido modificado y no se ha guardado.\n\302\277 Realmente desea salir ?")
	: TRUE;
}
void info(GtkWidget * w, const gchar * texto)
{

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(w),
					       GTK_DIALOG_MODAL |
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_INFO,
					       GTK_BUTTONS_CLOSE,
					       texto);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void acerca_de(GtkWidget * w)
{
    char *buffer =
	"Visi\303\263n por computador - Pipeline\nVersi\303\263n 0.1\nCarlos Le\303\263n, Jorge Mendoza, Diego S\303\241nchez\nSistemas Inform\303\241ticos 2004/2005\nFacultad de Inform\303\241tica (UCM)";
    info(w, buffer);
}



char *guardar_ventana(GtkWidget * w)
{
  dialogo_guardar_t *dialogo_guardar = dialogo_guardar_crear();
  GtkWidget *dialog = dialogo_guardar->filechooserdialog2;
  char *filename = 0;
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }
  dialogo_guardar_cerrar(dialogo_guardar);
  return filename;
}

/*gchar *entrada(GtkWidget * w, const gchar * pregunta)
{
    GtkWidget *d = create_dialog1();
    gchar *respuesta = 0;
    GtkWidget *label = lookup_widget(d, "label3");
    gtk_label_set_text(GTK_LABEL(label), pregunta);
    gchar *res = 0;
    if (gtk_dialog_run(GTK_DIALOG(d)) == GTK_RESPONSE_OK) {
	GtkWidget *entry = lookup_widget(d, "entry1");
	respuesta = (gchar *) gtk_entry_get_text(GTK_ENTRY(entry));
	res = strdup(respuesta);
    }

    gtk_widget_destroy(d);
    return res;
}*/

void insertar(/*GtkFixed * fixed, */ventana_principal_t * ventana)
{
    int id;
    for (id = 0; id < ventana->pipeline->m_numero; ++id) {
	gtk_fixed_put(GTK_FIXED(ventana->fixed1),
		      GTK_WIDGET(ventana->botones/*m_elemento*/[id].m_widget),
		      ventana->botones[id].m_x, ventana->botones[id].m_y);
/*		      pipeline->m_elemento[id].m_x,
		      pipeline->m_elemento[id].m_y);*/
    }
}

void propiedades(gint id_elemento, ventana_principal_t * ventana/*GtkFixed * fixed, pipeline_t * pipeline,
		 gboolean * mod*/)
{
  propiedades_modulo_t * propiedades_modulo_ = propiedades_modulo_crear();
  GtkWidget *dialog = propiedades_modulo_->propiedades_modulo;
  GtkWidget *nombre = propiedades_modulo_->entry2;
  GtkWidget *x = propiedades_modulo_->entry3;
  GtkWidget *y = propiedades_modulo_->entry4;
  GtkWidget *modulo = propiedades_modulo_->entry5;
    gchar buffer[8];
    if (id_elemento != -1) {
	gtk_entry_set_text(GTK_ENTRY(nombre),
			   ventana->pipeline->m_elemento[id_elemento].m_nombre);
	gtk_entry_set_text(GTK_ENTRY(modulo),
			   ventana->pipeline->m_elemento[id_elemento].m_ruta);
	sprintf(buffer, "%i", ventana->botones/*pipeline->m_elemento*/[id_elemento].m_x);
	gtk_entry_set_text(GTK_ENTRY(x), buffer);
	sprintf(buffer, "%i", ventana->botones/*pipeline->m_elemento*/[id_elemento].m_y);
	gtk_entry_set_text(GTK_ENTRY(y), buffer);
    }
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	ventana->modificado/**mod*/ = TRUE;
	gchar *nombre_v = (gchar *) gtk_entry_get_text(GTK_ENTRY(nombre));
	gchar *ruta_v = (gchar *) gtk_entry_get_text(GTK_ENTRY(modulo));
	gint x_v = atoi((gchar *) gtk_entry_get_text(GTK_ENTRY(x)));
	gint y_v = atoi((gchar *) gtk_entry_get_text(GTK_ENTRY(y)));

	if (ventana->fixed1 != NULL) {
	    if (id_elemento == -1) {
		     elemento_t *elemento =
		          pipeline_nuevo(ventana->pipeline, nombre_v/*, x_v, y_v*/, ruta_v);
		    
		          ventana->botones = (botones_t *)realloc(ventana->botones,
                          sizeof(botones_t) * ventana->pipeline->m_numero);
		          ventana->botones[ventana->pipeline->m_numero - 1].m_widget =
	                       gtk_toggle_button_new_with_label(nombre_v);
              gtk_widget_show(ventana->botones[ventana->pipeline->m_numero - 1].m_widget);
              ventana->botones[ventana->pipeline->m_numero - 1].m_x = x_v;
              ventana->botones[ventana->pipeline->m_numero - 1].m_y = y_v;
		    		    
		         gtk_fixed_put(GTK_FIXED(ventana->fixed1),
			              GTK_WIDGET(ventana->botones[ventana->pipeline->m_numero - 1].m_widget),
			              ventana->botones[ventana->pipeline->m_numero - 1].m_x,
                    ventana->botones[ventana->pipeline->m_numero - 1].m_y);
	    } else {
		ventana->botones[id_elemento].m_x = x_v;
		ventana->botones[id_elemento].m_y = y_v;
		strcpy(ventana->pipeline->m_elemento[id_elemento].m_ruta, ruta_v);
		strcpy(ventana->pipeline->m_elemento[id_elemento].m_nombre,
		       nombre_v);
		gtk_button_set_label(GTK_BUTTON
				     (ventana->botones[id_elemento].
				      m_widget),
				     ventana->pipeline->m_elemento[id_elemento].
				     m_nombre);
		gtk_fixed_move(GTK_FIXED(ventana->fixed1),
			       GTK_WIDGET(ventana->botones[id_elemento].
					  m_widget),
			       ventana->botones[id_elemento].m_x,
			       ventana->botones[id_elemento].m_y);
		pipeline_cambiar_biblioteca(&ventana->pipeline->m_elemento[id_elemento]/*pipeline, id_elemento*/);
	    }
	}
    } else {
	ventana->modificado = FALSE;
    }
    propiedades_modulo_cerrar(propiedades_modulo_);
}
void mostrar(GtkStatusbar * b, const char *info, guint * id)
{
    char buffer[128];
    sprintf(buffer, "Visi\303\263n por computador");
    if (info) {
	sprintf(buffer, "%s - %s", buffer, info);
    }
    gtk_statusbar_pop(GTK_STATUSBAR(b), *id);
    *id = gtk_statusbar_push(GTK_STATUSBAR(b), 0, buffer);
}


void establecer(/*pipeline_t * pipeline, */ventana_principal_t * ventana_principal)
{
    int i, si = 0, cual;
    gboolean todas_iniciadas = TRUE;
    gboolean todas_paradas = TRUE;
    for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	if (gtk_toggle_button_get_active
	    (GTK_TOGGLE_BUTTON(ventana_principal->botones[i].m_widget))) {
	    si++;
	    cual = i;
	}
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado == 1) {
	    todas_paradas = FALSE;
	}
	if (ventana_principal->pipeline->m_elemento[i].m_iniciado == 0) {
	    todas_iniciadas = FALSE;
	}
    }
    gboolean b = ventana_principal->pipeline->m_numero > 0;
    
    gtk_widget_set_sensitive(ventana_principal->establecer_error, b);
			     
    gtk_widget_set_sensitive(ventana_principal->iniciar_todas_biblioteca, b
			     && !todas_iniciadas);
    gtk_widget_set_sensitive(ventana_principal->cerrar_todas_biblioteca, b
			     && !todas_paradas);
    gtk_widget_set_sensitive(ventana_principal->ciclos_biblioteca,
			     b);
    gtk_widget_set_sensitive(ventana_principal->ciclo_biblioteca,
			     b);
    gtk_widget_set_sensitive(ventana_principal->parar_biblioteca,
			     b);

    gtk_widget_set_sensitive(ventana_principal->conectar1, si == 1);
    gtk_widget_set_sensitive(ventana_principal->propiedades1,
			     si == 1);
    gtk_widget_set_sensitive(ventana_principal->propiedades_biblioteca, si == 1);
    gtk_widget_set_sensitive(ventana_principal->iniciar_biblioteca,
			     si == 1
			     && ventana_principal->pipeline->m_elemento[cual].m_iniciado ==
			     0);
    gtk_widget_set_sensitive(ventana_principal->cerrar_biblioteca,
			     si == 1
			     && ventana_principal->pipeline->m_elemento[cual].m_iniciado ==
			     1);

    gtk_widget_set_sensitive(ventana_principal->borrar1, si > 0);
}

void guardar_como_aux(ventana_principal_t * ventana_principal, const char *file){
    if (file != 0) {
	pipeline_guardar(ventana_principal->pipeline, file);
	strcpy(ventana_principal->archivo, file);
	ventana_principal->modificado = FALSE;
	mostrar(GTK_STATUSBAR(ventana_principal->statusbar1), ventana_principal->archivo, &ventana_principal->id);
	gtk_widget_set_sensitive(ventana_principal->guardar2,
				 FALSE);
    }
}

gboolean tick(gpointer data) {
         ventana_principal_t * ventana = (ventana_principal_t*)data;
         pipeline_ciclo(ventana->pipeline);
         return ventana->m_seguir_timer;
}

int crear_timer(ventana_principal_t  *ventana, guint retardo)
{
    ventana->m_timer = g_timeout_add (retardo,
                                      tick,
                                      ventana);
    ventana->m_seguir_timer = TRUE;
    return 0;

}

int parar_timer(ventana_principal_t  *ventana)
{
   ventana->m_seguir_timer = FALSE;
return 0;
}



int elegir_modulo(GtkWidget * window1, pipeline_t * pipeline)
{

  dialogo_conectar_t * dialogo_conectar = dialogo_conectar_crear();
  GtkWidget *dialog = dialogo_conectar->dialog1;
  GtkWidget *combo = dialogo_conectar->comboboxentry1;//lookup_widget(dialog, "comboboxentry1");

    int i;
    for (i = 0; i < pipeline->m_numero; ++i) {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),
				  pipeline->m_elemento[i].m_nombre);
    }

    int respuesta =
	gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK ?
	respuesta = gtk_combo_box_get_active(GTK_COMBO_BOX(combo)) : -1;

    //gtk_widget_destroy(dialog);
    dialogo_conectar_cerrar(dialogo_conectar);

    return respuesta;
}

void pinchado(GtkWidget * menuitem, gpointer user_data)
{
ventana_principal_t * ventana_principal = (ventana_principal_t *)user_data;
    establecer(/*ventana_principal->pipeline, */ventana_principal);
}

void abrir(char *file, ventana_principal_t * ventana_principal)
{
    if (file != 0) {
	pipeline_vaciar(ventana_principal->pipeline);
	ventana_principal->pipeline = pipeline_cargar(file);
	if(!ventana_principal->pipeline)
	    ventana_principal->pipeline = pipeline_crear();                            
/*	char buf[23];
	sprintf(buf,"ventana_principal->pipeline->m_error = %i",ventana_principal->pipeline->m_error); 
	MessageBox(0, buf, "info", 0);*/
	strcpy(ventana_principal->archivo, file);
	ventana_principal->modificado = FALSE;
	mostrar(GTK_STATUSBAR(ventana_principal->statusbar1), ventana_principal->archivo, &ventana_principal->id);
	//GtkWidget *fixed = ventana_principal->fixed1;
	insertar(ventana_principal);//*GTK_FIXED(fixed), ventana_principal->pipeline);
	gtk_widget_set_sensitive(ventana_principal->nuevo2, TRUE);
	gtk_widget_set_sensitive(ventana_principal->guardar_como2,
				 TRUE);
	int i;
	for (i = 0; i < ventana_principal->pipeline->m_numero; ++i) {
	    g_signal_connect((gpointer) ventana_principal->botones[i].m_widget,
			     "toggled", G_CALLBACK(pinchado), ventana_principal);
	}
	establecer(ventana_principal/*->pipeline, ventana_principal*/);
    }

}
