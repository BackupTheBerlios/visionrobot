/*! \file dll_pipeline.h
    \brief Archivo de cabecera para las DLL's del pipeline
    \author Carlos Le�n
    \version 0.1
*/

#ifndef _DLL_PIPELINE_H_
#define _DLL_PIPELINE_H_

#include <windows.h>

/*! Si da alg�n fallo, comentar lo de extern "C" y dejar el resto */
#define DLLEXPORT /*extern "C" */__declspec (dllexport)

/*! \brief Realiza un ciclo, es llamado por el pipeline varias veces por segundo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT char* ciclo (const void *in, void **out);
/*! \brief Inicia el m�dulo. En esta funci�n hay que crear las estructuras de datos
    \param argumentos Una lista terminada en 0 de cadenas de caracteres que se le pasan al m�dulo
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT char* iniciar(int argc, const char **argv);

/*! \brief Cierra el m�dulo. En esta funci�n hay que liberar recursos, etc.
    \return 0 si no hay error, -1 en caso de fallo
*/
DLLEXPORT char* cerrar();

#endif
