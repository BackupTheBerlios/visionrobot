#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"  {
#endif

#include <glib.h>
  
  typedef struct modulo_s{
    char *m_nombre;
    void *m_dato;
    GHashTable *m_tabla;
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
