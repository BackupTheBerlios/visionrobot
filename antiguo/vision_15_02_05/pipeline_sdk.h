#ifndef _PIPELINE_SDK_H_
#define _PIPELINE_SDK_H_

#ifdef __cplusplus
extern "C"  {
#endif
  
#include <glib.h>
  
  typedef struct modulo_s{
    char *m_nombre;
    void *m_dato;
    char m_tipo;
    GHashTable *m_tabla;
    char *(*m_iniciar) (struct modulo_s* modulo, GHashTable *argumentos);
    char *(*m_ciclo) (struct modulo_s* modulo, char tipo, GHashTable *lista);//const pipeline_dato_t *in/*, pipeline_dato_t *out*/);
    char *(*m_cerrar) (struct modulo_s* modulo);
  } modulo_t;  
  
  modulo_t * get_modulo();

  
#ifdef __cplusplus
} 
#endif

#endif
