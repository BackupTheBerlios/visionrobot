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
#include <lualib.h>
#include <lauxlib.h>
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
  lua_State *m_lua; /*!< El contexto de Lua */
  GSList * m_ordenes; /*!< La lista de órdenes del script. */
} post_gestion_dato_t;

//! El tipo de datos para definir la orden.
typedef enum {
    MENSAJE /*!< Si es un mensaje, es como una orden del pipeline. */
    , ESPERAR /*!< Si es un temporizador entre mensajes. */
} tipo_accion_t;

//! Union para definir los diferentes tipos.
typedef union {
    int m_milisegundos; /*!< Milisegundos de espera. */
    robot_in_t m_orden; /*!< Orden que se le pasa al módulo. */
} argumento_t;

//! Una orden para la lista.
typedef struct {
    tipo_accion_t m_tipo; /*!< El tipo de orden. */
    argumento_t m_argumento; /*!< El argumento de la orden. */
} elemento_lista_t;

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

/*! \brief Llama a una función de Lua al estilo <code>printf</code>
  
\author Variación de Carlos León del libro "Programming in Lua" de Roberto Ierusalimschy
\param L El lua_State en el que está la función
\param func El nombre de la función que llamamos
\param sig Una cadena que define los parámetros, según la forma:
           <ul>
         <li>i: entero.
         <li>d: coma flotante.
         <li>s: cadena (char *).
         <li>p: puntero.
         <li>b: booleano.
       </ul>
       Los parámetros de entrada están separados de los de salida por ">".
*/
static void post_gestion_llamar_funcion (lua_State * L, const char *func, const char *sig, ...) {
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

static void post_gestion_insertar(post_gestion_dato_t *pgd, tipo_accion_t t, argumento_t a) {
	elemento_lista_t *e = (elemento_lista_t*)malloc(sizeof(elemento_lista_t));
	e->m_argumento = a;
	e->m_tipo = t;
	pgd->m_ordenes = g_slist_append(pgd->m_ordenes, e);
}

//! Envía un mensaje.
/*!
  \param L El lua_State.
  \return El número de argumentos que se le devuelven a Lua.
 */
static int post_gestion_mensaje(lua_State *L) {
	post_gestion_dato_t *in = (post_gestion_dato_t *)lua_touserdata(L, 1);
	argumento_t a;
	a.m_orden.m_orden = (char*)lua_tostring(L, 2);
	a.m_orden.m_parametro = (char*)lua_tostring(L, 3);
	post_gestion_insertar(in, MENSAJE, a); 
    return 0;
}

//! Programa una espera.
/*!
  \param L El lua_State.
  \return El número de argumentos que se le devuelven a Lua.
 */
static int post_gestion_esperar(lua_State *L) {
	post_gestion_dato_t *in = (post_gestion_dato_t *)lua_touserdata(L, 1);
	int numero = (int)lua_tonumber(L, 2);
	argumento_t a;
	a.m_milisegundos = numero;
	post_gestion_insertar(in, ESPERAR, a); 
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
  lua_State *l;
  char *script;
  post_gestion_dato_t *pgd = (post_gestion_dato_t*)modulo->m_dato;
  pgd->m_lua = 0;
  
  robot = (robot_in_t*)&pgd->m_robot;
  robot->m_orden = (char*)g_hash_table_lookup(argumentos,"orden_defecto");
  robot->m_parametro = (char*)g_hash_table_lookup(argumentos,"param_defecto");
  
  script = (char*)g_hash_table_lookup(argumentos,"script");
  if(script) {
    char *funcion = (char*)g_hash_table_lookup(argumentos,"funcion");
    static const struct luaL_reg pg_lua [] = {
    {"mensaje", post_gestion_mensaje},
    {"esperar", post_gestion_esperar},
    {NULL, NULL}
  };
  pgd->m_lua = lua_open();
  pgd->m_ordenes = 0;
  l = pgd->m_lua;
  
  luaL_openlib(l, "post_gestion", pg_lua, 0);
  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l); 
  luaL_loadfile(l, script);
  lua_pcall(l, 0, 0, 0);
  post_gestion_llamar_funcion(l, funcion, "p", pgd);
  }
  
  return "iniciado";
}

//! Borra los datos de la lista.
/*! Borra los datos de la lista. Función de retrollamada.
\param dato El dato que borramos.
\param unused parámetro sin usar.
*/
static void post_gestion_borrar_ordenes(gpointer dato, gpointer unused) {
	elemento_lista_t *e = (elemento_lista_t*)dato;
	free(e);
}

//! Cierra un módulo.
/*! Libera toda la memoria creada.
  
\param modulo El módulo actual.

\return Una cadena que indica qué tal ha ido todo.
*/

static char *post_gestion_cerrar(modulo_t *modulo)
{
  post_gestion_dato_t *pgd = (post_gestion_dato_t*)modulo->m_dato;
  if(pgd->m_lua) {
    lua_close(pgd->m_lua);
	g_slist_foreach (pgd->m_ordenes, post_gestion_borrar_ordenes,0);
	g_slist_free(pgd->m_ordenes);
  }
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

