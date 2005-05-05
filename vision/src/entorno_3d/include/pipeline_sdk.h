/*!
  \file   pipeline_sdk.h
  \author Carlos Le�n
  \date   Fri Mar  4 01:29:01 2005
  
  \brief  La cabecera que los m�dulos del pipeline tienen que implementar
  
          \section descripcion Descripci�n del proceso
	  Cada m�dulo del pipeline tiene que incluir este archivo e implementar el m�todo <code>get_modulo()</code>. De esta forma, el pipeline puede m�s tarde acceder a una copia de las funciones del mismo. Adem�s, dentro de la funci�n hay que iniciar las variables pertinentes.
  
  
*/

#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"  {
#endif

#include <glib.h>
  
  /*! \brief La estructura de datos del m�dulo */
  typedef struct modulo_s{
    char *m_nombre;		/*!< El nombre del m�dulo, que ser� cargado del XML. Es �nico en el pipeline */
    void *m_dato;		/*!< Un puntero que aloja cualquier dato que sea necesario para el m�dulo, el pipeline no hace nada con �l */
    GHashTable *m_tabla;	/*!< La tabla de cada m�dulo. No hay que crearla ni destruirla, s�lo hay que insertar en ella los punteros de las salidas de los puertos, indexadas por cadenas que representar�n el nombre de los mismos */
    char *(*m_iniciar) (struct modulo_s* modulo, GHashTable *argumentos);
    char *(*m_ciclo) (struct modulo_s* modulo, const char *puerto, const void *dato);
    char *(*m_cerrar) (struct modulo_s* modulo);
  } modulo_t;  
  
  /*! \brief Devuelve un puntero a un nuevo m�dulo creado. Cada m�dulo que implemente esta funci�n debe crear el m�dulo, establecer el nombre, el dato y los punteros a funci�n (la tabla es gestionada por el pipeline)
      \return Un nuevo m�dulo ya creado y listo para llamar a <em>iniciar</em>
  */
#ifdef _MSC_VER
  __declspec(dllexport)
#endif
	  modulo_t * get_modulo();
  
#ifdef __cplusplus
} 
#endif

#endif

