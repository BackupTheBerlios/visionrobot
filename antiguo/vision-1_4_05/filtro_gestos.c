#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>
#include <math.h>

#define PUERTO_IMAGEN "entrada_imagen"
#define PUERTO_PARAMETROS "entrada_parametros"
#define PUERTO_SALIDA "salida_imagen"

typedef struct {
  char m_buffer_error[128];  
  char *m_error;
  char m_iniciado;
  filtro_gestos_in_imagen_t m_buffer;
  filtro_gestos_in_imagen_t m_salida;
  //  filtro_gestos_in_parametros_t m_buffer_parametros;
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


static char *filtro_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  char *aux = 0;
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
	filtro_llamar_funcion(dato->m_lua, dato->m_iniciar, "piiipiii>s", 
			      dato->m_buffer.m_imagen, dato->m_buffer.m_ancho, dato->m_buffer.m_alto, dato->m_buffer.m_bytes,
			      dato->m_salida.m_imagen, dato->m_salida.m_ancho, dato->m_salida.m_alto, dato->m_salida.m_bytes,
			      &aux);
      }
      else if(dato->m_buffer.m_imagen) {
	// El filtro espera que la funcion de filtro le devuelva
	// 0 si todo es negro
	// 1 si no todo es negro
	int color;
	filtro_llamar_funcion(dato->m_lua, dato->m_filtrar, ">bs", &color, &aux);
	GHashTable *tabla = modulo->m_tabla;
	if(!color) {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, 0);
	}
	else {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, &dato->m_salida);
	}
	dato->m_error = dato->m_buffer_error;
      }
      if(aux) {
	strcpy(dato->m_buffer_error, aux);
	dato->m_error = dato->m_buffer_error;
      }
      else {
	dato->m_error = 0;
      }
    }
    else if(!strcmp(puerto, PUERTO_PARAMETROS)) {
      filtro_gestos_in_parametros_t* parametros = (filtro_gestos_in_parametros_t*)value;
      if(parametros) {
	/*	dato->m_buffer_parametros.m_rojo_sup = parametros->m_rojo_sup;
	dato->m_buffer_parametros.m_rojo_inf = parametros->m_rojo_inf;
	dato->m_buffer_parametros.m_verde_sup = parametros->m_verde_sup;
	dato->m_buffer_parametros.m_verde_inf = parametros->m_verde_inf;
	dato->m_buffer_parametros.m_azul_sup = parametros->m_azul_sup;
	dato->m_buffer_parametros.m_azul_inf = parametros->m_azul_inf;*/

	filtro_llamar_funcion(dato->m_lua, dato->m_parametros, "iiiiii>s",
			      parametros->m_rojo_sup,
			      parametros->m_rojo_inf,
			      parametros->m_verde_sup,
			      parametros->m_verde_inf,
			      parametros->m_azul_sup,
			      parametros->m_azul_inf, &aux);
      }
    }
    if(aux) {
      strcpy(dato->m_buffer_error, aux);
      dato->m_error = dato->m_buffer_error;
    }
    else {
      dato->m_error = 0;
    }
    return dato->m_error;
  }
  else {
    return "fallo en el filtro";
  }
}

static int filtro_gestos_newarray(lua_State *l) {  
  int alto = lua_tonumber(l, 2);
  int ancho = lua_tonumber(l, 3);
  int bytes = lua_tonumber(l, 4);
  color_t* imagen = (color_t*)lua_topointer(l, 1);
  int tam = sizeof(filtro_gestos_in_imagen_t);
  filtro_gestos_in_imagen_t *origen = (filtro_gestos_in_imagen_t*)lua_newuserdata(l, tam);
  luaL_getmetatable(l, "vision.imagen");
  lua_setmetatable(l, -2);
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
  if(index >= 1) {
    a->m_imagen[index - 1] = value;
  }
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

static int filtro_gestos_copia (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  filtro_gestos_in_imagen_t *b = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 2);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;
  int p, i, j, tam = ancho * bytes;
  for(i = 0; i < alto; ++i) {
    for(j = 0; j < tam; ++j) {
      p = i * tam + j;
      b->m_imagen[p] = a->m_imagen[p];
    }
  }
  lua_pushboolean(L, 1);
  return 1;
}


static int filtro_gestos_difuminar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  filtro_gestos_in_imagen_t *b = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 2);
  int factor = luaL_checkint(L, 3);
  int reduccion = luaL_checkint(L, 4);
  int ir = luaL_checkint(L, 5);
  int sr = luaL_checkint(L, 6);
  int iv = luaL_checkint(L, 7);
  int sv = luaL_checkint(L, 8);
  int ia = luaL_checkint(L, 9);
  int sa = luaL_checkint(L, 10);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;  
  color_t *buffer = a->m_imagen;
  color_t *orden = b->m_imagen;
  int p, i, j, tam = ancho * bytes, y, x;
  int rojo, verde, azul, acX = 0, acY = 0, cont = 0;
  int param_bytes = factor * bytes;
  for(y = 0; y < alto; ++y) {
    for(x = 0; x < tam; x += bytes) {
      rojo = 0;
      verde = 0;
      azul = 0;    
      if (y - factor >= 0 && y + factor < alto) {	
	if (x - param_bytes >= 0 && x + param_bytes < tam) {  
	  for (i = y - factor; i <= y + factor; i++) {	
	    for (j = x - param_bytes; j <= x + param_bytes; j += bytes) {
	      p = (i * tam) + j;
	      rojo += buffer[p];
	      verde += buffer[p + 1];
	      azul += buffer[p + 2];
	    }
	  }
	}
      }
      
      int numero = (2 * factor) + 1;
      numero *= numero;
      rojo = ((int)floor(rojo / numero));
      verde = ((int) floor(verde / numero));
      azul = ((int) floor(azul / numero));

      rojo = buffer[y * tam + x];
      verde = buffer[y * tam + x + 1];
      azul = buffer[y * tam + x + 2];
	    
      while (rojo % reduccion && rojo > 1)
	rojo--;
      while (verde % reduccion && verde > 1)
	verde--;
      while (azul % reduccion && azul > 1)
      azul--;

      /*rojo = buffer[y * tam + x];
      verde = buffer[y * tam + x + 1];
      azul = buffer[y * tam + x + 2];*/
      cont = 1;
      int pos = y * ancho * bytes + x;
      int valor = 0;
      color_t azul_comparar = (color_t)rojo;
      color_t rojo_comparar = (color_t)azul;
      color_t verde_comparar = (color_t)verde;
      if ((rojo_comparar >= ir && rojo_comparar <= sr)
	  && (verde_comparar >= iv && verde_comparar <= sv)
	  && (azul_comparar >= ia && azul_comparar <= sa)) {
	valor = 255;
	cont++;
	acX += x;
	acY += y;
      }
      orden[pos] = rojo;
      orden[pos + 1] = verde;
      orden[pos + 2] = azul;
    }
  }
  lua_pushnumber(L, cont);
  lua_pushnumber(L, acX);
  lua_pushnumber(L, acY);
  return 3;
}


static lua_State *filtro_abrir_lua(modulo_t *modulo, const char *ruta) {
  dato_filtro_t * dato = (dato_filtro_t*)modulo->m_dato;

  static const struct luaL_reg arraylib_f [] = {
    {"new", filtro_gestos_newarray},
    {NULL, NULL}
  };

  static const struct luaL_reg arraylib_m [] = {
    {"set", filtro_gestos_setarray},
    {"get", filtro_gestos_getarray},
    {"size", filtro_gestos_getsize},
    {"copia", filtro_gestos_copia},
    {"difuminar", filtro_gestos_difuminar},
    {NULL, NULL}
  };

  dato->m_lua = lua_open();
  lua_State *l = dato->m_lua;
  luaL_newmetatable(l, "vision.imagen");
  lua_pushstring(l, "__index");
  lua_pushvalue(l, -2);
  lua_settable(l, -3);
  luaL_openlib(l, NULL, arraylib_m, 0);  
  luaL_openlib(l, "imagen", arraylib_f, 0);

  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l);    

  luaL_loadfile(l, ruta);
  lua_pcall(l, 0, 0, 0);

  return l;
}

static char *filtro_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  dato->m_iniciado = -1;
  dato->m_error = 0;
  const char *archivo = g_hash_table_lookup(argumentos, "guion");
  sprintf(dato->m_buffer_error, "iniciado, cargado \"%s\"", archivo);
  dato->m_lua = filtro_abrir_lua(modulo, archivo);
  dato->m_iniciar = g_hash_table_lookup(argumentos, "iniciar");
  dato->m_filtrar = g_hash_table_lookup(argumentos, "filtrar");
  dato->m_parametros = g_hash_table_lookup(argumentos, "parametros");
  GHashTable *tabla = modulo->m_tabla;
  dato->m_salida.m_imagen = 0;
  dato->m_buffer.m_imagen = 0;
  g_hash_table_insert(tabla, PUERTO_SALIDA, &dato->m_salida);
  
  return dato->m_buffer_error;;
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
