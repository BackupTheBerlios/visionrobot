#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>


#define PUERTO_IMAGEN "entrada_imagen"
#define PUERTO_PARAMETROS "entrada_parametros"
#define PUERTO_SALIDA "salida_imagen"

typedef struct {
  char m_buffer_error[128];  
  char *m_error;
  char m_iniciado;
  filtro_gestos_in_imagen_t m_buffer;
  filtro_gestos_in_imagen_t m_salida;
  filtro_gestos_in_parametros_t m_buffer_parametros;
  const char *m_iniciar;
  const char *m_filtrar;
  const char *m_parametros;
  lua_State *m_lua;
} dato_filtro_t;


static void filtro_llamar_funcion (lua_State * L, const char *func, const char *sig, ...) {
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
	lua_isnumber(L, nres);
	*va_arg(vl, double *) = lua_tonumber(L, nres);
	break;    
      case 'i':
	lua_isnumber(L, nres);
	*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
	break;    
      case 's':
	lua_isstring(L, nres);
	*va_arg(vl, const char **) = lua_tostring(L, nres);
	break;
      case 'p':	lua_islightuserdata(L, nres);
	*va_arg(vl, const void **) = lua_topointer(L, nres);
	break;
      }
      nres++;
    }
    va_end(vl);
}


static char *filtro_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;

  if (dato) {
    if(!strcmp(puerto, PUERTO_IMAGEN)) {
      if(1 != dato->m_iniciado) {
	dato->m_iniciado = 1;
	filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t *) value;
	dato->m_buffer.m_alto = dato->m_salida.m_alto = imagen->m_alto;
	dato->m_buffer.m_ancho = dato->m_salida.m_ancho = imagen->m_ancho;
	dato->m_buffer.m_imagen = imagen->m_imagen;
	dato->m_buffer.m_bytes = dato->m_salida.m_bytes = imagen->m_bytes;
	dato->m_salida.m_imagen = (color_t *)malloc(sizeof(color_t) *
						    dato->m_salida.m_ancho *
						    dato->m_salida.m_alto *
						    dato->m_salida.m_bytes) ;
	// Esto de piiipiii son los parametros al estilo printf:
	// p = pointer, i = int
	filtro_llamar_funcion(dato->m_lua, dato->m_iniciar, "piiipiii", 
			      dato->m_buffer.m_imagen, dato->m_buffer.m_ancho, dato->m_buffer.m_alto, dato->m_buffer.m_bytes,
			      dato->m_salida.m_imagen, dato->m_salida.m_ancho, dato->m_salida.m_alto, dato->m_salida.m_bytes);
      }
      if(dato->m_buffer.m_imagen) {
	filtro_llamar_funcion(dato->m_lua, dato->m_filtrar, "");
	dato->m_error = 0;
      }
    }
    else if(!strcmp(puerto, PUERTO_PARAMETROS)) {
      filtro_gestos_in_parametros_t* parametros = (filtro_gestos_in_parametros_t*)value;
      if(parametros) {
	dato->m_buffer_parametros.m_rojo_sup = parametros->m_rojo_sup;
	dato->m_buffer_parametros.m_rojo_inf = parametros->m_rojo_inf;
	dato->m_buffer_parametros.m_verde_sup = parametros->m_verde_sup;
	dato->m_buffer_parametros.m_verde_inf = parametros->m_verde_inf;
	dato->m_buffer_parametros.m_azul_sup = parametros->m_azul_sup;
	dato->m_buffer_parametros.m_azul_inf = parametros->m_azul_inf;

	filtro_llamar_funcion(dato->m_lua, dato->m_parametros, "iiiiii",
			      parametros->m_rojo_sup,
			      parametros->m_rojo_inf,
			      parametros->m_verde_sup,
			      parametros->m_verde_inf,
			      parametros->m_azul_sup,
			      parametros->m_azul_inf);

	sprintf(dato->m_buffer_error,
		"Colores: PARAMETRO [%i, %i, %i, %i, %i, %i]",
		parametros->m_rojo_sup,
		parametros->m_rojo_inf,
		parametros->m_verde_sup,
		parametros->m_verde_inf,
		parametros->m_azul_sup,
		parametros->m_azul_inf);
	
	dato->m_error = dato->m_buffer_error;
      }
      else {
	dato->m_error = 0;
      }
    }
    return dato->m_error;
  }
  else {
    return "fallo en el filtro";
  }
}

static int filtro_gestos_new(lua_State *l) {  
  int alto = lua_tonumber(l, 2);
  int ancho = lua_tonumber(l, 3);
  int bytes = lua_tonumber(l, 4);
  color_t* imagen = (color_t*)lua_topointer(l, 1);
  int tam = sizeof(filtro_gestos_in_imagen_t);
  filtro_gestos_in_imagen_t *origen = (filtro_gestos_in_imagen_t*)lua_newuserdata(l, tam);
  origen->m_alto = alto;
  origen->m_ancho = ancho;
  origen->m_bytes = bytes;
  origen->m_imagen = imagen;
  return 1;
}

static int filtro_gestos_setarray (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  double value = luaL_checknumber(L, 3);
  a->m_imagen[index - 1] = value;
  return 0;
}

static int filtro_gestos_getarray (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  lua_pushnumber(L, a->m_imagen[index - 1]);
  return 1;
}

static int filtro_gestos_getsize (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  lua_pushnumber(L, a->m_ancho * a->m_alto * a->m_bytes);
  return 1;
}

static lua_State *filtro_abrir_lua(modulo_t *modulo, const char *ruta) {
  dato_filtro_t * dato = (dato_filtro_t*)modulo->m_dato;
  dato->m_lua = lua_open();
  lua_State *l = dato->m_lua;
  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l);    

  static const struct luaL_reg arraylib [] = {
    {"new", filtro_gestos_new},
    {"set", filtro_gestos_setarray},
    {"get", filtro_gestos_getarray},
    {"size", filtro_gestos_getsize},
    {NULL, NULL}
  };

  luaL_newmetatable(l, "vision.imagen");
  luaL_openlib(l, "imagen", arraylib, 0);

  lua_pushstring(l, "__index");
  lua_pushstring(l, "get");
  lua_pushvalue(l, 2);
  lua_settable(l, 1);

  lua_pushstring(l, "__newindex");
  lua_pushstring(l, "set");
  lua_gettable(l, 2);
  lua_settable(l, 1);
   
  luaL_loadfile(l, ruta);
  lua_pcall(l, 0, 0, 0);

  return l;
}

static char *filtro_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  dato->m_iniciado = -1;
  dato->m_error = 0;
  dato->m_lua = filtro_abrir_lua(modulo, g_hash_table_lookup(argumentos, "guion"));
  dato->m_iniciar = g_hash_table_lookup(argumentos, "iniciar");
  dato->m_filtrar = g_hash_table_lookup(argumentos, "filtrar");
  dato->m_parametros = g_hash_table_lookup(argumentos, "parametros");
  GHashTable *tabla = modulo->m_tabla;
  dato->m_salida.m_imagen = 0;
  dato->m_buffer.m_imagen = 0;
  g_hash_table_insert(tabla, PUERTO_SALIDA, &dato->m_salida);
  
  return "filtro iniciado";
}
static char *filtro_cerrar(modulo_t *modulo)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  free(dato->m_salida.m_imagen);
  lua_close(dato->m_lua);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));  
  modulo->m_nombre = "Filtro";
  modulo->m_iniciar = filtro_iniciar;
  modulo->m_cerrar = filtro_cerrar;
  modulo->m_ciclo = filtro_ciclo;
  modulo->m_dato = (dato_filtro_t *)malloc(sizeof(dato_filtro_t));
  return modulo;

}
