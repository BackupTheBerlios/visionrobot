#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <lualib.h>
#include <lauxlib.h>
#include <parapin.h>
#include "robot_sdk.h"

/* Pines del puerto paralelo */
/* TODO: poner los que sean */
#define AVANZAR LP_PIN05
#define RETROCEDER LP_PIN06
#define GIRAR_IZQUIERDA LP_PIN07
#define GIRAR_DERECHA LP_PIN08

/*! \brief El puerto de entrada, recibe un <code>char *</code> */
#define PUERTO "entrada_robot"


//! Estructura de datos de información del módulo.
typedef struct {
  char m_error[128];		/*!< El búfer con los datos de salida e información. */
  lua_State *m_lua;		/*!< El entorno de Lua, para los guiones de comportamiento del robot. */
  char *m_funcion;		/*!< El nombre de la función a la que se debe llamar en el guión. */
} dato_robot_t;

//! Realiza un ciclo en el módulo.
/*! En el ciclo recibe la señal del módulo de gestión de resultados del pipeline, y llama al script con los datos.
  
\param modulo El módulo actual.
\param puerto El puerto por el que llega la información.
\param dato La información que llega.

\return Una cadena que indica qué tal ha ido todo.
*/
static char *robot_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  dato_robot_t *dato_robot = (dato_robot_t *)modulo->m_dato;
  robot_in_t *entrada = (robot_in_t*)dato;
  if(!strcmp(PUERTO, puerto) && entrada) {
    lua_State *L = dato_robot->m_lua;
    lua_getglobal(L, dato_robot->m_funcion);
    lua_pushstring(L, (const char *)entrada->m_orden);
    lua_pushstring(L, (const char *)entrada->m_parametro);
    lua_pcall(L, 2, 1, 0);
    strcpy(dato_robot->m_error, (char *)lua_tostring(L, -1));
    return dato_robot->m_error;
  }
  else {
    return 0;
  }
}

//! Función Lua para el avance.
/*! Establece correctamente los pines del puerto paralelo.
  
\param L El entorno de Lua.

\return El número de argumentos que se devuelven.
*/
static int robot_avanzar (lua_State *L) {
  char param = (char)luaL_checkint(L, 1);
  clear_pin(RETROCEDER | GIRAR_DERECHA | GIRAR_IZQUIERDA);
  set_pin(AVANZAR);
  return 0;
}

//! Función Lua para el retroceso.
/*! Establece correctamente los pines del puerto paralelo.
  
\param L El entorno de Lua.

\return El número de argumentos que se devuelven.
*/

static int robot_retroceder (lua_State *L) {
  char param = (char)luaL_checkint(L, 1);
  clear_pin(AVANZAR | GIRAR_DERECHA | GIRAR_IZQUIERDA);
  set_pin(RETROCEDER);
  return 0;
}

//! Función Lua para el giro a la izquierda.
/*! Establece correctamente los pines del puerto paralelo.
  
\param L El entorno de Lua.

\return El número de argumentos que se devuelven.
*/

static int robot_girar_izquierda (lua_State *L) {
  char param = (char)luaL_checkint(L, 1);
  clear_pin(RETROCEDER | GIRAR_DERECHA | AVANZAR);
  set_pin(GIRAR_IZQUIERDA);
  return 0;
}

//! Función Lua para el giro a la derecha.
/*! Establece correctamente los pines del puerto paralelo.
  
\param L El entorno de Lua.

\return El número de argumentos que se devuelven.
*/

static int robot_girar_derecha (lua_State *L) {
  char param = (char)luaL_checkint(L, 1);
  clear_pin(RETROCEDER | AVANZAR | GIRAR_IZQUIERDA);
  set_pin(GIRAR_DERECHA);
  return 0;
}


//! Función Lua para la parada.
/*! Establece correctamente los pines del puerto paralelo.
  
\param L El entorno de Lua.

\return El número de argumentos que se devuelven.
*/

static int robot_parar (lua_State *L) {
  clear_pin(RETROCEDER | AVANZAR | GIRAR_IZQUIERDA | GIRAR_DERECHA);
  return 0;
}

//! Inicia un módulo.
/*! Crea la memoria, lee los argumentos del XML, y abre el puerto paralelo.
  
\param modulo El módulo actual.
\param argumentos Una tabla con los argumentos.

\return Una cadena que indica qué tal ha ido todo.
*/
static char *robot_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  char *devolver = "iniciado";
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  dato->m_funcion = strdup(g_hash_table_lookup(argumentos, "funcion"));
  dato->m_lua = lua_open();
  lua_State *l = dato->m_lua;
  static const struct luaL_reg robot [] = {
    {"avanzar", robot_avanzar},
    {"retroceder", robot_retroceder},
    {"girar_derecha", robot_girar_derecha},
    {"girar_izquierda", robot_girar_izquierda},
    {"parar", robot_parar},
    {NULL, NULL}
  };
  luaL_openlib(l, "robot", robot, 0);
  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l); 
  const char *ruta = g_hash_table_lookup(argumentos, "guion");
  luaL_loadfile(l, ruta);
  lua_pcall(l, 0, 0, 0);
  

  char * puerto = g_hash_table_lookup(argumentos, "puerto");
  if(!puerto) {
    devolver = "Error, no se ha especificado puerto";
  }
  else {
    int p = atoi(puerto);
    if(pin_init_user(p == 1 ? LPT1 : LPT2)) {
      devolver = "Fallo al abrir el puerto. Debe ser root (permisos)";
    }
    else {
      pin_output_mode(AVANZAR | GIRAR_DERECHA | GIRAR_IZQUIERDA | RETROCEDER);
    }
  }

  return devolver;
}

//! Cierra un módulo de robot.
/*! Libera toda la memoria creada.
  
\param modulo El módulo actual.

\return Una cadena que indica qué tal ha ido todo.
*/
static char *robot_cerrar(modulo_t *modulo)
{
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  clear_pin(AVANZAR | GIRAR_DERECHA | GIRAR_IZQUIERDA | RETROCEDER);
  free(dato->m_funcion);
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

