#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#define PUERTO_ENTRADA "entrada_red"
#define PUERTO_SALIDA  "salida_texto"

typedef struct {
  char m_salida[128];
  char m_anterior[128];
} gestion_dato_t;

static char *gestion_ciclo(modulo_t *modulo, const char *puerto, const void *entrada)
{
  if(entrada) {
    if(!strcmp(PUERTO_ENTRADA, puerto)) {
      const char *cadena = (const char *)entrada;
      gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
      if(strcmp(entrada, dato->m_frase_anterio) != 0) {
      }

      // si no quiero enviar nada
      g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
      // else
      g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, &dato->m_salida);
    }
  }
  else {
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  }

  return 0;
}

static char *gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);

  char *argumento = g_hash_table_lookup(argumentos,"anterior");
  strcpy(m_anterior, argumento);

  return "iniciado";
}
static char *gestion_cerrar(modulo_t *modulo)
{
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Gestion";
  modulo->m_iniciar = gestion_iniciar;
  modulo->m_cerrar = gestion_cerrar;
  modulo->m_ciclo = gestion_ciclo;
  modulo->m_dato = (gestion_dato_t *)malloc(sizeof(gestion_dato_t));
  return modulo;
}
