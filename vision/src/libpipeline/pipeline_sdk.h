/*!
  \file   pipeline_sdk.h
  \author Carlos León
  \date   Fri Mar  4 01:29:01 2005
  
  \brief  La cabecera que los módulos del pipeline tienen que implementar
  
          \section descripcion Descripción del proceso
	  Cada módulo del pipeline tiene que incluir este archivo e implementar el método <code>get_modulo()</code>. De esta forma, el pipeline puede más tarde acceder a una copia de las funciones del mismo. Además, dentro de la función hay que iniciar las variables pertinentes.
  
  
*/

#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"  {
#endif

#include <glib.h>
  
  /*! \brief La estructura de datos del módulo */
  typedef struct modulo_s{
    char *m_nombre;		/*!< El nombre del módulo, que será cargado del XML. Es único en el pipeline */
    void *m_dato;		/*!< Un puntero que aloja cualquier dato que sea necesario para el módulo, el pipeline no hace nada con él */
    GHashTable *m_tabla;	/*!< La tabla de cada módulo. No hay que crearla ni destruirla, sólo hay que insertar en ella los punteros de las salidas de los puertos, indexadas por cadenas que representarán el nombre de los mismos */
    char *(*m_iniciar) (struct modulo_s* modulo, GHashTable *argumentos);
    char *(*m_ciclo) (struct modulo_s* modulo, const char *puerto, const void *dato);
    char *(*m_cerrar) (struct modulo_s* modulo);
  } modulo_t;  
  
  /*! \brief Devuelve un puntero a un nuevo módulo creado. Cada módulo que implemente esta función debe crear el módulo, establecer el nombre, el dato y los punteros a función (la tabla es gestionada por el pipeline)
      \return Un nuevo módulo ya creado y listo para llamar a <em>iniciar</em>
  */
  modulo_t * get_modulo();

  
#ifdef __cplusplus
} 
#endif

#endif
