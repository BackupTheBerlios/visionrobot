/*! \file dll_pipeline.h
    \brief Archivo de cabecera para las DLL's del pipeline
    \author Carlos Le�n
    \version 0.1
*/

#ifndef _DLL_PIPELINE_H_INCLUIDO_
#define _DLL_PIPELINE_H_INCLUIDO_

#include <windows.h>

/*! Si da alg�n fallo, comentar lo de extern "C" y dejar el resto */
#define DLLEXPORT extern "C" __declspec (dllexport)

/*! \brief Realiza un ciclo, es llamado por el pipeline varias veces por segundo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int ciclo();

/*! \brief Establece los datos para que la DLL los gestione. Estos datos le llegan de los m�dulos conectados que tienen como salida este m�dulo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int set_datos(const void *datos);

/*! \brief Devuelve los datos del m�dulo. Es la salida que se realimenta a los otros m�dulos
    \return Un puntero a void con los datos que se devuelven. Las interfaces del m�dulo han de establecerse.
*/
DLLEXPORT void *get_datos();

/*! \brief Inicia el m�dulo. En esta funci�n hay que crear las estructuras de datos
    \param argumentos Una lista terminada en 0 de cadenas de caracteres que se le pasan al m�dulo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int iniciar(const char **argumentos) ;

/*! \brief Funci�n que muestra propiedades de configuraci�n o cualquier cosa �til algo ajena al pipeline
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int propiedades();

/*! \brief Cierra el m�dulo. En esta funci�n hay que liberar recursos, etc.
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int cerrar();

/*! \brief Devuelve una cadena de error o informaci�n, para depurar
    \return 0 si no hay que escribir nada, un puntero a char si hay informaci�n
*/
DLLEXPORT char *error();

#endif
