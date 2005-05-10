/*!
  \file   robot.c
  \author Carlos Le�n
  
  \brief  M�dulo que se encarga de gestionar un script que define el comportamiento de un robot enchufado al puerto paralelo.

          \section modulo Descripci�n del m�dulo
	     Este m�dulo define funciones de gesti�n del puerto paralelo para que puedan ser llamadas por un script en Lua (que se pasa como par�metro en el XML), y se encarga de iniciar y finalizar el puerto.

	  \section puertos Puertos
	  El m�dulo s�lo tiene puertos de entrada:
	   <ul>
	      <li><em>entrada_robot</em>: Una entrada de tipo <code>robot_in_t</code>, que define el comportamiento (orden y par�metro) que debe efectuar el robot.
	   </ul>

	   \section argumentos Argumentos
	   <ul>
	     <li><em>guion</em>: La ruta del gui�n, absoluta o respecto del XML.
	     <li><em>funcion_ciclo</em>: El nombre de la funci�n del script que usamos para efectuar un ciclo cuando le llega una orden.
	     <li><em>funcion_fin</em>: El nombre de la funci�n del script que usamos para finalizar el valor de los pines.
	     <li><em>funcion_iniciar</em>: El nombre de la funci�n del script que usamos para iniciar el puerto y los pines.
	     <li><em>puerto</em>: El n�mero de puerto paralelo que se usa (1 o 2).
	   </ul> 
  
*/

#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <lualib.h>
#include <lauxlib.h>
#include "robot_sdk.h"
#include "paralelo.h"

/*! \brief El puerto de entrada, recibe un <code>char *</code> */
#define PUERTO "entrada_robot"


//! Estructura de datos de informaci�n del m�dulo.
typedef struct {
  char m_error[128];		/*!< El b�fer con los datos de salida e informaci�n. */
  lua_State *m_lua;		/*!< El entorno de Lua, para los guiones de comportamiento del robot. */
  char *m_funcion_ciclo;	/*!< El nombre de la funci�n a la que se debe llamar en el gui�n. */
  char *m_funcion_fin;
  char *m_funcion_iniciar;
  //guint m_tiempo;
} dato_robot_t;

//! El argumento que le pasamos a la funci�n de llamada del timer.
typedef struct { 
  lua_State *l; /*!< El <code>lua_State</code> del script. */
  char *f; /*!< Una cadena que representa el nombre de la funci�n Lua que se llama cuando hay que detener la orden */
} arg_timer_t;




/*! \brief Llama a una funci�n de Lua al estilo <code>printf</code>
  
\author Variaci�n de Carlos Le�n del libro "Programming in Lua" de Roberto Ierusalimschy
\param L El lua_State en el que est� la funci�n
\param func El nombre de la funci�n que llamamos
\param sig Una cadena que define los par�metros, seg�n la forma:
           <ul>
	     <li>i: entero.
	     <li>d: coma flotante.
	     <li>s: cadena (char *).
	     <li>p: puntero.
	     <li>b: booleano.
	   </ul>
	   Los par�metros de entrada est�n separados de los de salida por ">".
*/
static void robot_llamar_funcion (lua_State * L, const char *func, const char *sig, ...) {
  va_list vl;
  int narg, nres;     
  va_start(vl, sig);
  lua_getglobal(L, func);
  narg = 0;
  while (*sig) {
    switch (*sig++) {    
    case 'd': 
      lua_pushnumber(L, va_arg(vl, double));
      break;    
    case 'i': 
      lua_pushnumber(L, va_arg(vl, int));
      break;    
    case 's': 
      lua_pushstring(L, va_arg(vl, char *));
      break;
    case 'p': 
      lua_pushlightuserdata(L, va_arg(vl, void *));
      break;    
    case 'b':
      lua_pushboolean(L, va_arg(vl, int));
      break;    
    case '>':
      goto endwhile;
    }
    narg++;
    luaL_checkstack(L, 1, "too many arguments");
  } endwhile:    
    nres = strlen(sig);
    lua_pcall(L, narg, nres, 0);
    nres = -nres; 
    while (*sig) {
      switch (*sig++) {    
      case 'd':
	*va_arg(vl, double *) = lua_tonumber(L, nres);
	break;    
      case 'i':
	*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
	break;    
      case 's':
	*va_arg(vl, const char **) = lua_tostring(L, nres);
	break;
      case 'p':	
	*va_arg(vl, const void **) = lua_topointer(L, nres);
	break;
      case 'b':	
	*va_arg(vl, int*) = lua_toboolean(L, nres);
	break;
      }
      nres++;
    }
    va_end(vl);
}



//! Realiza un ciclo en el m�dulo.
/*! En el ciclo recibe la se�al del m�dulo de gesti�n de resultados del pipeline, y llama al script con los datos.
  
\param modulo El m�dulo actual.
\param puerto El puerto por el que llega la informaci�n.
\param dato La informaci�n que llega.

\return Una cadena que indica qu� tal ha ido todo.
*/
static char *robot_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  dato_robot_t *dato_robot = (dato_robot_t *)modulo->m_dato;
  robot_in_t *entrada = (robot_in_t*)dato;
  if(!strcmp(PUERTO, puerto) && entrada) {
    lua_State *L = dato_robot->m_lua;
    char *aux;
    robot_llamar_funcion(L, dato_robot->m_funcion_ciclo, "ss>s",
			 entrada->m_orden, entrada->m_parametro, &aux);
    strcpy(dato_robot->m_error, aux);
    return dato_robot->m_error;
  }
  else {
    return 0;
  }
}

//! Pone un pin a alta.
/*!
  \param L El lua_State.
  \return El n�mero de argumentos que se le devuelven a Lua.
 */
static int robot_alta(lua_State *L) {
  int index = luaL_checkint(L, 1);
  paralelo_sube_pin(index);
  return 0;
}

//! Pone un pin a baja.
/*!
  \param L El lua_State.
  \return El n�mero de argumentos que se le devuelven a Lua.
 */

static int robot_baja(lua_State *L) {
  int index = luaL_checkint(L, 1);
  paralelo_baja_pin(index);
  return 0;
}
//! La funci�n del timer del robot, para detener la orden actual.
/*!
  \param data Un arg_timer_t.
  \return FALSE.
 */

gboolean robot_on_time(gpointer data) {
  arg_timer_t * a = (arg_timer_t *)data;
  robot_llamar_funcion(a->l, a->f, "");
  free(a->f);
  free(a);
  return FALSE;
}
//! Inicia un timer que se detendr� llamando a la funci�n de salida que se le pasa en el script.
/*!
  \param L El lua_State.
  \return El n�mero de argumentos que se le devuelven a Lua.
 */

static int robot_timer(lua_State *L) {
  arg_timer_t *a = (arg_timer_t*)malloc(sizeof(arg_timer_t));
  int ms = luaL_checkint(L, 1);
  const char *funcion_parada = lua_tostring(L, 2);
  a->l = L;
  a->f = strdup(funcion_parada);
  lua_pop(L, 1);
  g_timeout_add(ms, robot_on_time, a);
  return 0;
}
//! Establece pines de salida
/*!
  \param L El lua_State.
  \return El n�mero de argumentos que se le devuelven a Lua.
 */

static int robot_salida(lua_State *L) {
  int index = luaL_checkint(L, 1);
  paralelo_pin_salida(index);
  return 0;
}

//! Inicia un m�dulo.
/*! Crea la memoria, lee los argumentos del XML, y abre el puerto paralelo.
  
\param modulo El m�dulo actual.
\param argumentos Una tabla con los argumentos.

\return Una cadena que indica qu� tal ha ido todo.
*/
static char *robot_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  char *devolver = "iniciado";
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  lua_State *l;
  static const struct luaL_reg robot [] = {
    {"alta", robot_alta},
    {"baja", robot_baja},
    {"salida",robot_salida},
    {"timer", robot_timer},
    {NULL, NULL}
  };
  const char *ruta;
  char * puerto;
  dato->m_funcion_ciclo = strdup(g_hash_table_lookup(argumentos, "funcion_ciclo"));
  dato->m_funcion_fin = strdup(g_hash_table_lookup(argumentos, "funcion_fin"));
  dato->m_funcion_iniciar = strdup(g_hash_table_lookup(argumentos, "funcion_iniciar"));
  dato->m_lua = lua_open();
  l = dato->m_lua;
  
  luaL_openlib(l, "robot", robot, 0);
  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l); 
  ruta = g_hash_table_lookup(argumentos, "guion");
  luaL_loadfile(l, ruta);
  lua_pcall(l, 0, 0, 0);
  

  puerto = g_hash_table_lookup(argumentos, "puerto");
  if(!puerto) {
    devolver = "Error, no se ha especificado puerto";
  }
  else {
    int p = atoi(puerto);

    if(paralelo_iniciar(p == 1 ? PARALELO_LPT1 : PARALELO_LPT2)) {
      devolver = "Fallo al abrir el puerto. Debe ser root (permisos)";
    }
    else {
      robot_llamar_funcion(l, dato->m_funcion_iniciar, "");
    }
  }
  return devolver;
}

//! Cierra un m�dulo de robot.
/*! Libera toda la memoria creada.
  
\param modulo El m�dulo actual.

\return Una cadena que indica qu� tal ha ido todo.
*/
static char *robot_cerrar(modulo_t *modulo)
{
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  robot_llamar_funcion(dato->m_lua, dato->m_funcion_fin, "");
  free(dato->m_funcion_ciclo);
  free(dato->m_funcion_iniciar);
  paralelo_terminar();
  free(dato->m_funcion_fin);
  lua_close(dato->m_lua);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Robot";
  modulo->m_iniciar = robot_iniciar;
  modulo->m_cerrar = robot_cerrar;
  modulo->m_ciclo = robot_ciclo;
  modulo->m_dato = (dato_robot_t *)malloc(sizeof(dato_robot_t ));
  return modulo;

}

