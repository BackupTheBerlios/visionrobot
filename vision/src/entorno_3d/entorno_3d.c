/*! \file entorno_3d.c
    \brief Módulo de entorno 3D para la presentación de la salida de "Visión por computador"

    \author Jorge Mendoza
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include <robot_sdk.h>

/*! \brief El puerto de entrada de la orden, recibe un <code>char *</code> */
#define PUERTO_ENTRADA "entrada_robot"

static char *entorno_3d_ciclo(modulo_t *modulo, const char *puerto, const void *dato){
  if(!strcmp(PUERTO_ENTRADA, puerto)) {
  }
  return 0;
}

static char *entorno_3d_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  return "iniciado";
}
static char *entorno_3d_cerrar(modulo_t *modulo)
{
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Entorno 3D";
  modulo->m_iniciar = entorno_3d_iniciar;
  modulo->m_cerrar = entorno_3d_cerrar;
  modulo->m_ciclo = entorno_3d_ciclo;
  modulo->m_dato = 0;
  return modulo;

}

