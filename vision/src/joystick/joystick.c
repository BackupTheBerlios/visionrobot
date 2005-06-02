/*! \file joystick.c
    \brief Genera una entrada para el robot desde un interfaz de entrada joystick

          \section modulo Módulo
      Este módulo interactúa con un interfaz de joystick, y transforma su entrada a una legible por el robot.
      \section puertos Puertos
      El módulo no dispone de puertos de entrada, y como salida tiene:
      <ul>
        <li><em>salida_robot</em>: Una puntero a estructura de tipo <code>robot_in_t</code>.
		<li><em>salida_texto</em>: Una cadena que define la entrada del joystick.
      </ul>
      \section argumentos Argumentos
        Los únicos argumentos posibles son el número de joystick que se quiere usar (si se omite, se usa el primero encontrado), y los milisegundos del temporizador.
      <ul>
        <li><em>numero</em>: El número de joystick.
        <li><em>milisegundos</em>: Los milisegundos del temporizador de lectura de la entrada de joystick.
      </ul>
    \author Carlos León
    \version 1.0
*/  

#include "pipeline_sdk.h"
#include "robot_sdk.h"
#include <glib.h>
#include <stdlib.h>
#include <SDL/SDL.h>

//! El puerto de salida de este módulo.
#define PUERTO_SALIDA "salida_robot"

//! El puerto de salida de este módulo.
#define PUERTO_TEXTO "salida_texto"

//! Función de retrollamada para el timer.
/*! Gestiona la entrada del joystick.
  \param data El dato, que se le da el módulo actual en esta implementación.
  \return TRUE siempre, para que el timer siga.
 */
gboolean joystick_on_time(gpointer data) {
    SDL_Event event;
	int mueve = 0;
    modulo_t * modulo = (modulo_t *)data;
    robot_in_t * robot = (robot_in_t *)modulo->m_dato;
	g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
//	g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
    while(SDL_PollEvent(&event)) {		
    switch(event.type)
 {  
	case SDL_JOYBUTTONDOWN:
robot->m_orden = "parar";
          robot->m_parametro = "nula";
		  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, "Jostick: parar");
		mueve = 1;
		break;
        case SDL_JOYHATMOTION:
        if ( event.jhat.value & SDL_HAT_UP )  {
			mueve = 1;
          robot->m_orden = "avanzar";
          robot->m_parametro = "alta";		  
		  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, "Jostick: avanzar");
        }

        if ( event.jhat.value & SDL_HAT_DOWN ){
			mueve = 1;
          robot->m_orden = "avanzar";
          robot->m_parametro = "nula";
		  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, "Jostick: retroceder");
        }

        if ( event.jhat.value & SDL_HAT_LEFT ){
			mueve = 1;
          robot->m_orden = "girar";
          robot->m_parametro = "alta";
		  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, "Jostick: girar izquierda");
        }

        if ( event.jhat.value & SDL_HAT_RIGHT )  {
			mueve = 1;
          robot->m_orden = "girar_negativo";
          robot->m_parametro = "alta";
		  g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, "Jostick: girar derecha");
        }		 		
        break;
      }
	if(!mueve) {
			g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
		}
		else {
			g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, robot);
		}
    }
    return TRUE;
}

//! Realiza un ciclo en el módulo.
/*! En el ciclo recibe la señal del módulo de gestión de resultados del pipeline.
  
\param modulo El módulo actual.
\param puerto El puerto por el que llega la información.
\param dato La información que llega.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *joystick_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
	joystick_on_time(modulo);
  return 0;
}


//! Inicia un módulo.
/*! Crea la memoria, lee los argumentos del XML, y abre el puerto paralelo.
  
\param modulo El módulo actual.
\param argumentos Una tabla con los argumentos.

\return Una cadena que indica qué tal ha ido todo.
*/


static char *joystick_iniciar(modulo_t *modulo, GHashTable *argumentos) {
    SDL_Joystick *joystick;	 
	robot_in_t *robot = (robot_in_t*)modulo->m_dato;
    const char * numero = g_hash_table_lookup(argumentos, "numero");
    const char *milisegundos = g_hash_table_lookup(argumentos, "milisegundos");
    int numero_int = numero ? atoi(numero) : 0;
    int milisegundos_int = milisegundos ? atoi(milisegundos) : 50;
    //g_timeout_add(milisegundos_int, joystick_on_time, modulo);
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);	
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(numero_int);

	robot->m_orden = "parar";
    robot->m_parametro = "nula";
	g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);	
	g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
  return "iniciado";
}


//! Cierra un módulo.
/*! Libera toda la memoria creada.
  
\param modulo El módulo actual.

\return Una cadena que indica qué tal ha ido todo.
*/


static char *joystick_cerrar(modulo_t *modulo)
{
    robot_in_t *r = (robot_in_t*)modulo->m_dato;
    free(r);
  free(modulo);
  SDL_Quit();
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Joystick";
  modulo->m_iniciar = joystick_iniciar;
  modulo->m_cerrar = joystick_cerrar;
  modulo->m_ciclo = joystick_ciclo;
  modulo->m_dato = (robot_in_t*)malloc(sizeof(robot_in_t));
  return modulo;

}
