/*! \file dll_pipeline.h
    \brief Archivo de cabecera para las DLL's del pipeline
    \author Carlos León
    \versión 0.1
*/

#ifndef _DLL_PIPELINE_H_INCLUIDO_
#define _DLL_PIPELINE_H_INCLUIDO_

#include <windows.h>

/*! Si da algún fallo, comentar lo de extern "C" y dejar el resto */
#define DLLEXPORT extern "C" __declspec (dllexport)

/*! \brief Realiza un ciclo, es llamado por el pipeline varias veces por segundo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int ciclo ();

/*! \brief Establece los datos para que la DLL los gestione. Estos datos le llegan de los módulos conectados que tienen como salida este módulo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int set_datos(const void * datos) ;

/*! \brief Devuelve los datos del módulo. Es la salida que se realimenta a los otros módulos
    \return Un puntero a void con los datos que se devuelven. Las interfaces del módulo han de establecerse.
*/
DLLEXPORT void * get_datos();

/*! \brief Inicia el módulo. En esta función hay que crear las estructuras de datos
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int iniciar() ;

/*! \brief Función que muestra propiedades de configuración o cualquier cosa útil algo ajena al pipeline
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int propiedades() ;

/*! \brief Cierra el módulo. En esta función hay que liberar recursos, etc.
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int cerrar();
/*! \brief Devuelve una cadena de error o información, para depurar
    \return 0 si no hay que escribir nada, un puntero a char si hay información
*/
DLLEXPORT char * error();

#endif
