#ifndef _VENTANA_PRINCIPAL_H_INCLUIDO_
#define _VENTANA_PRINCIPAL_H_INCLUIDO_

#include <gtk/gtk.h>

#include <pipeline.h>

#define TAM_ARCHIVO 64

typedef struct {
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
    GtkWidget *propiedades_biblioteca;
    GtkWidget *ciclo_biblioteca;
    GtkWidget *ciclos_biblioteca;
    GtkWidget *parar_biblioteca;
    GtkWidget *iniciar_biblioteca;
    GtkWidget *iniciar_todas_biblioteca;
    GtkWidget *establecer_error;
    GtkWidget *cerrar_todas_biblioteca;
    GtkWidget *cerrar_biblioteca;
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
    GtkWidget *separador5;
    GtkWidget *acerca_de1;
    GtkWidget *image113;
    GtkWidget *scrolledwindow1;
    GtkWidget *viewport1;
    GtkWidget *fixed1;
    GtkWidget *statusbar1;
    GtkAccelGroup *accel_group;
    pipeline_t *pipeline;
    guint id;
    char archivo[TAM_ARCHIVO];
    gboolean modificado;
    GdkGC *gc;
    GdkDrawable *ww;
    guint m_timer;
    gboolean m_seguir_timer;
    guint m_retardo;
} ventana_principal_t;

/*! \brief Crea una ventana principal
    \return Un puntero a la estructura de datos de la ventana
*/
ventana_principal_t *ventana_principal_crear();

/*! \brief Libera la memoria de la ventana, y la cierra
    \param ventana_principal La ventana que borramos
*/
void ventana_principal_borrar(ventana_principal_t * ventana_principal);

/*! \brief Abre un archivo en la ventana
    \param ventana_principal La ventana en la que abrimos el archivo
    \param ruta La ruta del archivo que abrimos
*/
void ventana_principal_abrir(ventana_principal_t * ventana_principal,
        const char *ruta);

#endif
