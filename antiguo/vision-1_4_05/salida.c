/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos León
    \version 0.1
*/  
    
#include "pipeline_sdk.h"
#include <stdio.h>
#include <string.h>


static char *salida_ciclo(modulo_t *modulo, const pipeline_dato_t *in, pipeline_dato_t *out)
{
  if (in->m_dato) {
    printf("%s\n", (char *) in->m_dato);
  }
  fflush(stdout);
  return 0;
}
static char *salida_iniciar(modulo_t *modulo, int argc, const char **argv)
{
  int i;
  for (i = 0; i < argc; i += 2) {
    if (!strcmp("texto", argv[i])) {
      printf("%s\n", argv[i + 1]);
    }
  }
  return "iniciado";
}
static char *salida_cerrar(modulo_t *modulo)
{
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Salida";
  modulo->m_iniciar = salida_iniciar;
  modulo->m_cerrar = salida_cerrar;
  modulo->m_ciclo = salida_ciclo;
  modulo->m_dato = 0;
  return modulo;

}

