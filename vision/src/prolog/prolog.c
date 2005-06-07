#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SWI-Prolog.h>

#define PUERTO "entrada_texto"

#define PUERTO_SALIDA "salida_texto"

#define PUERTO_ORDEN "salida_orden"

#define PUERTO_PARAMETRO "salida_parametro"

typedef struct {
  char m_buffer_salida[128];
} prolog_dato_t;

//! Pasa una cadena a mayúsculas
/*! \param cadena1 La cadena
 *  \return La cadena, en mayúsculas
 */
 
 static char * prolog_mayusculas(const char * cadena1) {
  char *cadena = strdup(cadena1);  
  unsigned int i = 0;
  while(i < strlen(cadena)) {
    if(cadena[i] >= 'a' && cadena[i] <= 'z') {
      cadena[i] -= 'a' - 'A';
    }
    i++;
  }
  return cadena;
}
static char *prolog_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  predicate_t pred;
  term_t h0;
  double f = 0.0f;
  
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  
  if(!strcmp(PUERTO, puerto)) {
    char *cadena = (char *)dato;
    if(cadena) {
      char *cadena_aux = prolog_mayusculas(cadena);
      if(!strcmp(cadena_aux, "AVANZAR")) {
        g_hash_table_insert(modulo->m_tabla, PUERTO_ORDEN, "avanzar");
        g_hash_table_insert(modulo->m_tabla, PUERTO_PARAMETRO, "media");
      }
      else if(!strcmp(cadena_aux, "RETROCEDER")) {
        g_hash_table_insert(modulo->m_tabla, PUERTO_ORDEN, "avanzar");
        g_hash_table_insert(modulo->m_tabla, PUERTO_PARAMETRO, "nula");
      }
      else if(!strcmp(cadena_aux, "GIRAR IZQUIERDA")) {
        g_hash_table_insert(modulo->m_tabla, PUERTO_ORDEN, "girar");
        g_hash_table_insert(modulo->m_tabla, PUERTO_PARAMETRO, "alta");
      }
      else if(!strcmp(cadena_aux, "GIRAR DERECHA")) {
        g_hash_table_insert(modulo->m_tabla, PUERTO_ORDEN, "girar_negativo");
        g_hash_table_insert(modulo->m_tabla, PUERTO_PARAMETRO, "alta");
      }
      else {	     
        pred = PL_predicate("camaron", 2, "dcg");
        h0 = PL_new_term_refs(2);
        PL_put_list_codes(h0, cadena);
        if(PL_call_predicate(NULL, PL_Q_NORMAL, pred, h0)) {
            PL_get_float(h0 + 1, &f);
            sprintf(prolog->m_buffer_salida, "Resultado: %f.", f);
            g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, prolog->m_buffer_salida);
        } 
        else {
            g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
        }    
      }
      free(cadena_aux);
    }
  }
  return 0;
}

static char *prolog_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  char *plav[] = {""};
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  g_hash_table_insert(modulo->m_tabla, PUERTO_ORDEN, 0);
  g_hash_table_insert(modulo->m_tabla, PUERTO_PARAMETRO, 0);
  PL_initialise(1, plav);
  return "iniciado";
}
static char *prolog_cerrar(modulo_t *modulo)
{  
  prolog_dato_t *prolog = (prolog_dato_t*)modulo->m_dato;
  PL_halt(0);
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
