/*! \file main.c
    \brief Programa principal
    \author zosco
    \version 0.1
 */

/*! \mainpage Visión por Computador - Pipeline
 *  \section intro Introducción
 *  Este módulo del proyecto es una interfaz gráfica para la depuración y
 *  uso de pipelines de plugins. Cada elemento está enlazado con una biblioteca
 *  de vínculos dinámicos, que tiene unas funciones predefinidas que el programa
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

#include "ventana_principal.h"

int main(int argc, char *argv[])
{
    gtk_set_locale();
    gtk_init(&argc, &argv);

    ventana_principal_t * ventana_principal = ventana_principal_crear();

    gtk_widget_show(ventana_principal->window1);
    
    if(argc > 1) {
            abre(argv[1], ventana_principal);
    }

    gtk_main();

    ventana_principal_borrar(ventana_principal);

    return 0;
}
