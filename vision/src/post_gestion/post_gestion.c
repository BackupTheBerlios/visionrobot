/*! \file post_gestion.c
    \brief Genera la salida real de todo el proceso, para el robot.

    \author Carlos León
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

/*! \brief El puerto de entrada, recibe un <code>char *</code> */
#define PUERTO_ENTRADA "entrada_texto"


#define PUERTO_SALIDA "salida_robot"

static char *post_gestion_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  if(!strcmp(PUERTO_ENTRADA, puerto)) {
  }

  return 0;
}

static char *post_gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {

  return "iniciado";
}
static char *post_gestion_cerrar(modulo_t *modulo)
{
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Post_Gestion";
  modulo->m_iniciar = post_gestion_iniciar;
  modulo->m_cerrar = post_gestion_cerrar;
  modulo->m_ciclo = post_gestion_ciclo;
  modulo->m_dato = 0;
  return modulo;

}

