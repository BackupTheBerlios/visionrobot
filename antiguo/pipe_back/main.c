/*! \file main.c
    \brief Programa principal
    \author zosco
    \version 0.1
 */

/*! \mainpage Visi�n por Computador - Pipeline
 *  \section intro Introducci�n
 *  Este m�dulo del proyecto es una interfaz gr�fica para la depuraci�n y
 *  uso de pipelines de plugins. Cada elemento est� enlazado con una biblioteca
 *  de v�nculos din�micos, que tiene unas funciones predefinidas que el programa
 *  se encarga de gestionar.
 *
 *  \section licencia Licencia
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"
#include "pipeline.h"
#include "callbacks.h"
#include "auxiliar.h"

GtkWidget *window1;

int main(int argc, char *argv[])
{
    gtk_set_locale();
    gtk_init(&argc, &argv);
    add_pixmap_directory("./");

    window1 = create_window1(argc > 1 ? argv[1] : 0);

    gtk_widget_show(window1);

    gtk_main();
    return 0;
}
