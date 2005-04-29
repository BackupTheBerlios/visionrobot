#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
//#include <SWI-Prolog.h>

#define PUERTO "entrada_texto"
#define PUERTO_SALIDA "salida_texto"

typedef struct {
  char m_buffer_salida[128];
} prolog_dato_t;

static char *prolog_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  if(!strcmp(PUERTO, puerto)) {
    const char *cadena = (const char *)dato;
    if(cadena) {
      g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, prolog->m_buffer_salida);
      /*term_t a0 = PL_new_term_refs(2);
      static predicate_t p;
      p = PL_predicate("frase", 2, "database");
      PL_put_string_chars(a0, "caca");
      qid_t qid =  PL_open_query(0, PL_Q_NORMAL, p, a0); 
      while(PL_next_solution(qid)) {
      }
      PL_close_query(qid);*/
    }
  }
  return 0;
}

static char *prolog_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  char *plav[2];
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  plav[0] = "pipeline";
  plav[1] = NULL;
  sprintf(prolog->m_buffer_salida, "Sin implementar");
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  //PL_initialise(1, plav);
  return "iniciado";
}
static char *prolog_cerrar(modulo_t *modulo)
{
  //PL_halt(0);
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  free(prolog);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Prolog";
  modulo->m_iniciar = prolog_iniciar;
  modulo->m_cerrar = prolog_cerrar;
  modulo->m_ciclo = prolog_ciclo;
  modulo->m_dato = (prolog_dato_t*)malloc(sizeof(prolog_dato_t));
  return modulo;
}
