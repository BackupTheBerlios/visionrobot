#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <SWI-Prolog.h>

#define PUERTO "entrada_texto"

static char *prolog_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  if(!strcmp(PUERTO, puerto)) {
    const char *cadena = (const char *)dato;
    if(cadena) {
      term_t a0 = PL_new_term_refs(2);
      static predicate_t p;
      p = PL_predicate("frase", 2, "database");
      PL_put_string_chars(a0, "caca");
      qid_t qid =  PL_open_query(0, PL_Q_NORMAL, p, a0); 
      while(PL_next_solution(qid)) {
      }
      PL_close_query(qid);
    }
  }
  return 0;
}

static char *prolog_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  char *plav[2];
  plav[0] = "pipeline";
  plav[1] = NULL;

  PL_initialise(1, plav);
  return "iniciado";
}
static char *prolog_cerrar(modulo_t *modulo)
{
  PL_halt(0);
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
  modulo->m_dato = 0;
  return modulo;
}
