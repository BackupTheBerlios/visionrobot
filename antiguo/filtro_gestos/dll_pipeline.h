/*! \file dll_pipeline.h
    \brief Archivo de cabecera para las DLL's del pipeline
    \author Carlos Le�n
    \versi�n 0.1
*/

#ifndef _DLL_PIPELINE_H_INCLUIDO_
#define _DLL_PIPELINE_H_INCLUIDO_

#define DLLEXPORT extern "C" __declspec (dllexport)

/*! \brief Realiza un ciclo, es llamado por el pipeline varias veces por segundo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int ciclo ();

/*! \brief Establece los datos para que la DLL los gestione. Estos datos le llegan de los m�dulos conectados que tienen como salida este m�dulo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int set_datos(const void * datos) ;

/*! \brief Devuelve los datos del m�dulo. Es la salida que se realimenta a los otros m�dulos
    \return Un puntero a void con los datos que se devuelven. Las interfaces del m�dulo han de establecerse.
*/
DLLEXPORT void * get_datos();

/*! \brief Inicia el m�dulo. En esta funci�n hay que crear las estructuras de datos
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int iniciar() ;

/*! \brief Funci�n que muestra propiedades de configuraci�n o cualquier cosa �til algo ajena al pipeline
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int propiedades() ;

/*! \brief Cierra el m�dulo. En esta funci�n hay que liberar recursos, etc.
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT int cerrar();

#endif


