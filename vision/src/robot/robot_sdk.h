/*!
  \file   robot_sdk.h
  \author Carlos León
  
  \brief  Cabecera para implementar un módulo que conecte con un robot.
*/
#ifndef _ROBOT_SDK_H_
#define _ROBOT_SDK_H_

//! Estructura de orden a un robot.
typedef struct {
  char *m_orden; /*!< La cadena de orden */
  char *m_parametro; /*!< La cadena del parámetro */
} robot_in_t;

#endif
