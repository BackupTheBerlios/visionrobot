/*! \file post_gestion.c
    \brief Genera la salida real de todo el proceso, para el robot.

          \section modulo Descripción del módulo
	  Se encarga de unificar todos los datos que le llegan, para generar una orden con sentido.

	  \section puertos Puertos
	  El módulo tiene puertos de entrada y de salida:
	   <ul>
	      <li><em>entrada_orden</em>: Un <code>char *</code>, que representa una orden.
	      <li><em>entrada_param</em>: Un <code>char *</code>, que representa un parámetro.
	      <li><em>salida_texto</em>: Un <code>char *</code>, que representa una salida formada por la orden y el parámetro actual.
	      <li><em>salida_robot</em>: Un <code>char *</code>, que representa una orden para un robot (entorno 3D o robot).
	   </ul>

	   \section argumentos Argumentos
	   <ul>
	     <li><em>orden_defecto</em>: Un <code>char *</code>, que representa una orden por defecto.
	     <li><em>param_defecto</em>: Un <code>char *</code>, que representa un parámetro por defecto.
	   </ul> 
  


    \author Carlos León
    \version 1.0
*/  
    
#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <robot_sdk.h>
#include "robot_sdk.h"


/*! \brief El puerto de entrada de la orden, recibe un <code>char *</code> */
#define PUERTO_ORDEN "entrada_orden"

/*! \brief El puerto de entrada del parámetro, recibe un <code>char *</code> */
#define PUERTO_PARAMETRO "entrada_param"

/*! \brief El puerto de salida, una puntero a estructura robot_in_t */
#define PUERTO_SALIDA "salida_robot"

/*! \brief El puerto de salida de texto, una puntero a char */
#define PUERTO_TEXTO "salida_texto"

//! La estructura del módulo.
typedef struct {
  robot_in_t m_robot; /*!< El dato de la salida "robot". */
  char m_buffer[64]; /*!< El dato de "texto". */
} post_gestion_dato_t;



//! Realiza un ciclo en el módulo.
/*! En el ciclo recibe la señal del módulo de gestión de resultados del pipeline.
  
\param modulo El módulo actual.
\param puerto El puerto por el que llega la información.
\param dato La información que llega.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *post_gestion_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  if(dato) {
    post_gestion_dato_t *pgd = (post_gestion_dato_t*)modulo->m_dato;
    robot_in_t * robot = &pgd->m_robot;
    char *palabra = (char *)dato;
    char actualizar = 0;
    char ** destino = 0;
    if(!strcmp(PUERTO_ORDEN, puerto)) {
      destino = &robot->m_orden;
    }
    else if(!strcmp(PUERTO_PARAMETRO, puerto)) {	
      destino = &robot->m_parametro;
    }
    if(strcmp(*destino, palabra)) {
      *destino = palabra;
      actualizar = 1;
    }
    sprintf(pgd->m_buffer, "Robot -> [%s, %s]", robot->m_orden, robot->m_parametro);
    
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, actualizar ? robot : 0);
    g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, actualizar ? pgd->m_buffer : 0);
  }
  else {
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
    g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
  }
  return 0;
}



//! Inicia un módulo.
/*! Crea la memoria, lee los argumentos del XML, y abre el puerto paralelo.
  
\param modulo El módulo actual.
\param argumentos Una tabla con los argumentos.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *post_gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  robot_in_t *robot;
  post_gestion_dato_t *pgd = (post_gestion_dato_t*)modulo->m_dato;
  robot = (robot_in_t*)&pgd->m_robot;
  robot->m_orden = (char*)g_hash_table_lookup(argumentos,"orden_defecto");
  robot->m_parametro = (char*)g_hash_table_lookup(argumentos,"param_defecto");

  
  return "iniciado";
}

//! Cierra un módulo.
/*! Libera toda la memoria creada.
  
\param modulo El módulo actual.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *post_gestion_cerrar(modulo_t *modulo)
{
  post_gestion_dato_t *pgd = (post_gestion_dato_t*)modulo->m_dato;  
  free(pgd);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Post gestion";
  modulo->m_iniciar = post_gestion_iniciar;
  modulo->m_cerrar = post_gestion_cerrar;
  modulo->m_ciclo = post_gestion_ciclo;
  modulo->m_dato = (post_gestion_dato_t *)malloc(sizeof(post_gestion_dato_t));
  return modulo;

}

