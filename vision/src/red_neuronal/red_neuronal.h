
/*! \file red_neuronal.h
    \brief Cabecera del modulo de red
    \author Diego Sanchez
    \version 0.2
 */  
    
/*! \mainpage
  \section intro Introduccion
  Esta seccion forma el modulo de red, es incluido en el pipeline al
  iniciarse el programa con un archivo que contiene los pesos de una
  red ya entrenada para reconocer imagenes de gestos filtradas.
  Este modulo solo contiene codigo para la propagacion de datos desde
  las neuronas de entrada hasta las de salida, pero no posee implementacion
  de entrenamiento.
  \section licencia Licencia
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/ 


#ifndef _RED_NEURONAL_H_
#define _RED_NEURONAL_H_


//typedef enum { ORDEN, PARAM } tipo_foto_t;


typedef struct {
  int numEntrada;
  int numOculta;
  int numSalida;
  double *capaEntrada;
  double *capaOculta;
  double *capaSalida;
  double *pesosEntrada;
  double *pesosOculta;
  // Esto debería ser una lista dinámica,
  // pero ya que la implementación está para 4 neuronas
  // lo dejo así
  // Carlos
  const char *m_salida[4];
  const char *m_no_gesto;
} red_neuronal_t;


red_neuronal_t * red_neuronal_crear(int en, int oc, int sa, const char *salida[], const char *no_gesto);
void red_neuronal_borrar(red_neuronal_t **);
void red_neuronal_computar_capas(red_neuronal_t * red);
red_neuronal_t * red_neuronal_abrir(const char *file, const char *salida[], const char *no_gesto);
const char *red_neuronal_reconocer(red_neuronal_t * red, char *dibujo,
			      int ancho, int alto, int bytes/*,tipo_foto_t tipo*/);

#endif

		



