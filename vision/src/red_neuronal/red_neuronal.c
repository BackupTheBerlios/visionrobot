/*! \file red_neuronal.c
    \brief Implementación de la red.
    \author Diego Sánchez
    \version 0.2
 */  
    
/*
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


#include "red_neuronal.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//---------------------------------------------------------------------------

red_neuronal_t * red_neuronal_crear(int en, int oc, int sa, const char *salida[], const char *no_gesto) 
{
    red_neuronal_t * red = (red_neuronal_t *) malloc(sizeof(red_neuronal_t));
    red->numEntrada = en;
    red->numOculta = oc;
    red->numSalida = sa;
    red->capaEntrada = (double *) malloc(sizeof(double) * (red->numEntrada + 1));
    red->capaOculta = (double *) malloc(sizeof(double) * (red->numOculta + 1));
    red->capaSalida = (double *) malloc(sizeof(double) * (red->numSalida + 1));
    /*    red->pesosEntrada = (double *) malloc(sizeof(double) * ((red->numEntrada + 1) * (red->numOculta + 1)));
	  red->pesosOculta = (double *) malloc(sizeof(double) * ((red->numOculta + 1) * (red->numSalida + 1)));*/
    red->m_salida[0] = salida[0];
    red->m_salida[1] = salida[1];
    red->m_salida[2] = salida[2];
    red->m_salida[3] = salida[3];
    red->m_no_gesto = no_gesto;
    return red;
}


//---------------------------------------------------------------------------

void red_neuronal_borrar(red_neuronal_t ** red)
{
    if (red && (*red)) {
	free((*red)->capaOculta);
	free((*red)->capaEntrada);
	free((*red)->capaSalida);
	free((*red)->pesosEntrada);
	free((*red)->pesosOculta);
	free(*red);
	*red = 0;
    }
}


//---------------------------------------------------------------------------
//Calcula el valor de la unidades de la capa2
void red_neuronal_calcular_capa(int num1, int num2, double *capa1, double *capa2,
				 double *pesos) 
{
    double sum;
    int j,k;
    capa1[0] = 1;
    for (j = 1; j <= num2; j++) {
	sum = 0;
	for (k = 0; k <= num1; k++)
	    sum += pesos[(k * (num2 + 1)) + j] * capa1[k];
	capa2[j] = (1 / (1 + exp(-sum)));
    }
}


//---------------------------------------------------------------------------
//Calcula el valor de la capa oculta y la de salida
void red_neuronal_computar_capas(red_neuronal_t * red) 
{
    red_neuronal_calcular_capa(red->numEntrada, red->numOculta,
				red->capaEntrada, red->capaOculta,
				red->pesosEntrada);
    red_neuronal_calcular_capa(red->numOculta, red->numSalida,
				red->capaOculta, red->capaSalida,
				red->pesosOculta);
} 

//---------------------------------------------------------------------------
//Lee de un archivo una red ya entrenada
red_neuronal_t * red_neuronal_abrir(const char *file, const char *salida[], const char *no_gesto)
{
    FILE * archivo;
    int ne, no, ns;
    double numero;
    red_neuronal_t * redA;
    double *pesosEntrada2 ;
    double *pesosOculta2 ;
    int i;
    archivo = fopen(file, "rb");
    if (!archivo) {
	return 0;
    }
    fscanf(archivo, "%d %d %d", &ne, &no, &ns);
     redA = red_neuronal_crear(ne, no, ns, salida, no_gesto);
    pesosEntrada2 = (double *) malloc(sizeof(double) * ((ne + 1) * (no + 1)));
    pesosOculta2 = (double *) malloc(sizeof(double) * ((no + 1) * (ns + 1)));

    for (i = 0; i <= ne; i++) {
	int j;
	for (j = 0; j <= no; j++) {
	    fread(&numero, sizeof(numero), 1, archivo);
	    pesosEntrada2[(i * (no + 1)) + j] = numero;
	}
    }
    for (i = 0; i <= no; i++) {
	int j;
	for (j = 0; j <= ns; j++) {
	    fread(&numero, sizeof(numero), 1, archivo);
	    pesosOculta2[(i * (ns + 1)) + j] = numero;
	}
    }
    redA->pesosEntrada = pesosEntrada2;
    redA->pesosOculta = pesosOculta2;
    fclose(archivo);
    return redA;
}


//---------------------------------------------------------------------------
//Carga un vector de char que reprentan la imagen, en la entrada de la red
void red_neuronal_cargar_input_imagen(red_neuronal_t * red, char *dibujo,
					int ancho, int alto, int bytes) 
{
    int k = 0;
    double *entrada = red->capaEntrada;
    int i;
    if (!red)
	return;

    for (i = 0; i < alto; i++) {
	int j;
	for (j = 0; j < ancho * bytes; j++) {
	    entrada[k] = (double) ((unsigned char)dibujo[(i * ancho * bytes) + j] / 255);
	    k++;
	    j += bytes - 1;
	}	
    }
}


//---------------------------------------------------------------------------
/*Propaga la informacion desde la entrada hasta la salida de la red
y genera un salida dependiendo de lo qua ha reconocido*/
const char *red_neuronal_reconocer(red_neuronal_t * red, char *dibujo,
			     int ancho, int alto, int bytes/*,*/
			     /*tipo_foto_t tipo*/) {
  int i;
  if(!red)
    return "red no cargada";
  if (!dibujo)
    return "no hay dibujo";
  red_neuronal_cargar_input_imagen(red, dibujo, ancho, alto, bytes);
  red_neuronal_computar_capas(red);
  for(i = 0; i < 4; ++i) {
    if(red->capaSalida[i + 1] > 0.5) {
      return red->m_salida[i];
    }
  }
  /*switch (tipo) {
    case ORDEN:
    if (red->capaSalida[1] > 0.5)
      return "parar";
      
      else if (red->capaSalida[2] > 0.5)
      return "girar";
      
      else if (red->capaSalida[3] > 0.5)
      return "girar_negativo";
      
    else if (red->capaSalida[4] > 0.5)
    return "avanzar";
    
    else
    return "no_gesto";
    case PARAM:
    if (red->capaSalida[1] > 0.5)
    return "nula";
    
    else if (red->capaSalida[2] > 0.5)
    return "baja";
    
    else if (red->capaSalida[3] > 0.5)
    return "media";
    
    else if (red->capaSalida[4] > 0.5)
    return "alta";
    
    else
    return "maxima";
    }*/
  return red->m_no_gesto;//"no_gesto";
}

//---------------------------------------------------------------------------




