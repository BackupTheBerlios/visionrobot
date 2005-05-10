/*! \file paralelo.h
    \brief librería de control del puerto paralelo multiplataforma.

    \section detalles Detalles
    Esta librería es un "wrapper" de libparapin para linux, y de las llamadas a inpout.dll de Windows, haciendo que, de forma transparente, se puede acceder al puerto paralelo, y a sus pines, de forma independiente, fácilmente, y sin preocuparse de la arquitectura. Tiene su interés principalmente cuando se usa el puerto paralelo como control de robots.

    \author Carlos León
    \version 1.0
*/
#ifndef _PARALELO_H_
#define _PARALELO_H_

//! Enumeración de puertos posibles
typedef enum {PARALELO_LPT1 = 0,
      PARALELO_LPT2} paralelo_puerto_t;

//! Inicia el puerto paralelo.
/*!
  \param puerto LPT1 o LPT2, según paralelo_puerto_t.
  \return 0 si todo va bien, otra cosa si no.
*/
int paralelo_iniciar(paralelo_puerto_t puerto);

//! Pone un pin a 1.
/*!
  \param index El número de pin.
 */
void paralelo_sube_pin(int index);

//! Pone un pin a baja.
/*!
  \param index El pin.
 */

void paralelo_baja_pin(int index);

//! Establece que un pin sea de salida.
/*!
  \param index El pin.
 */
void paralelo_pin_salida(int index);

//! Cierra la librería.
void paralelo_terminar();

#endif //_PARALELO_H_
