#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"  {
#endif

#include <glib.h>

  typedef struct {
    char m_tipo;
    void *m_dato;
  } pipeline_dato_t;  

  typedef struct modulo_s{
    char *m_nombre;
    void *m_dato;
    char *(*m_iniciar) (struct modulo_s* modulo, GHashTable *argumentos);
    char *(*m_ciclo) (struct modulo_s* modulo, const pipeline_dato_t *in, pipeline_dato_t *out);
    char *(*m_cerrar) (struct modulo_s* modulo);
  } modulo_t;  
  
  modulo_t * get_modulo();

  
#ifdef __cplusplus
} 
#endif

#endif
