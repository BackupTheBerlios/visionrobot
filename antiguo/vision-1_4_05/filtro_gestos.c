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
  const char *m_iniciar;
  const char *m_filtrar;
  const char *m_parametros;
  const char *m_cerrar;
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
  if (value && dato) {
    if(!strcmp(puerto, PUERTO_IMAGEN)) {
      if(1 != dato->m_iniciado) {
	dato->m_iniciado = 1;
	filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t *) value;
	color_t *imagen_salida = (color_t *)malloc(sizeof(color_t) *
						    imagen->m_ancho *
						    imagen->m_alto *
						    imagen->m_bytes) ;
	// Esto de piiipiii son los parametros al estilo printf:
	// p = pointer, i = int
	filtro_llamar_funcion(dato->m_lua, dato->m_iniciar, "piiipiii>s", 
			      imagen->m_imagen, imagen->m_ancho, imagen->m_alto, imagen->m_bytes,
			      imagen_salida, imagen->m_ancho, imagen->m_alto, imagen->m_bytes,
			      &aux);
      }
      else {
	// El filtro espera que la funcion de filtro le devuelva
	// 0 si todo es negro
	// 1 si no todo es negro
	int color;
	filtro_gestos_in_imagen_t *imagen;
	filtro_llamar_funcion(dato->m_lua, dato->m_filtrar, ">bsp", &color, &aux, &imagen);
	GHashTable *tabla = modulo->m_tabla;
	if(!color) {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, 0);
	}
	else {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, imagen);
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
      filtro_llamar_funcion(dato->m_lua, dato->m_parametros, "iiiiii>s",
			    parametros->m_rojo_sup,
			    parametros->m_rojo_inf,
			    parametros->m_verde_sup,
			    parametros->m_verde_inf,
			    parametros->m_azul_sup,
			    parametros->m_azul_inf, &aux);
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
    return 0;
  }
}

static int filtro_gestos_delete(lua_State *L) {  
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  free(a->m_imagen);
  ilDeleteImages(1, &a->m_id);
  return 0;
}
static int filtro_gestos_newarray(lua_State *l) {  
  int ancho = lua_tonumber(l, 2);
  int alto = lua_tonumber(l, 3);
  int bytes = lua_tonumber(l, 4);
  color_t* imagen = (color_t*)lua_topointer(l, 1);
  int tam = sizeof(filtro_gestos_in_imagen_t);
  filtro_gestos_in_imagen_t *origen = (filtro_gestos_in_imagen_t*)lua_newuserdata(l, tam);
  luaL_getmetatable(l, "vision.imagen");
  lua_setmetatable(l, -2);
  ilGenImages(1, &origen->m_id);  
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

static int filtro_gestos_centrar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int cont = luaL_checkint(L, 2);
  int ac_x = luaL_checkint(L, 3);
  int ac_y = luaL_checkint(L, 4);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;
  color_t *dibujo = a->m_imagen;
  int tam = bytes * ancho;
  int sum, sum2, sum3, sum4;
  if (cont != 0) { 
    int difY = ((int) floor(alto / 2 - floor(ac_y / cont)));
    int difX = ((int) floor(ancho / 2 - ((int) floor(ac_x / (bytes * cont)))));
    if (difY < 0) {
      int y, x;
      for (y = 0; y - difY < alto; y++) {
	sum = y * tam;
	sum2 = (y - difY) * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = dibujo[sum2 + x];
	}
      }
      while (y < alto) {
	sum = y * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = 0;
	}
	y++;
      }
    }
    if (difY > 0) {
      int y, x;
      for (y = alto - 1; y - difY >= 0; y--) {
	sum = y * tam;
	sum2 = (y - difY) * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = dibujo[sum2 + x];
	}
      }
      while (y >= 0) {
	sum = y * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = 0;
	}
	y--;
      }
    }
    if (difX < 0) {
      int y;
      for (y = 0; y < alto; y++) {
	int x;	
	sum3 = y * tam;
	x = 0;
	sum2 = x - (difX * bytes);
	for (x = 0; sum2 + bytes - 1 < tam; x+=bytes) {
	  sum = sum3 + x;
	  sum2 = x - (difX * bytes);
	  sum4 = sum3 + sum2;
	  dibujo[sum] = dibujo[sum4];
	  dibujo[sum + 1] = dibujo[sum4 + 1];
	  dibujo[sum + 2] = dibujo[sum4 + 2];
	}
	while (x + 2 < tam) {
	  sum = sum3 + x;
	  dibujo[sum] = 0;
	  dibujo[sum + 1] = 0;
	  dibujo[sum + 2] = 0;
	  x += bytes - 1;
	}
      }
    }
    /* He cambiado esto porque si no viola el segmento.
       Antes:
       if(difX > 0) {
       Carlos
    */
    if (difX > 1) {
      int y;
      for (y = 0; y < alto - 1; y++) {
	int x;
	sum3 = y * tam;
	x = tam - 2;
	sum2 = x - (difX * bytes);
	for (x = tam - 2; sum2 + bytes - 1 >= 0;
	     x-=bytes) {
	  sum2 = x - (difX * bytes);
	  sum = sum2 + sum3;
	  sum4 = sum3 + x;	  
	  dibujo[sum4] = dibujo[sum];
	  dibujo[sum4 - 1] = dibujo[sum - 1];
	  dibujo[sum4 - 2] = dibujo[sum - 2];
	}
	sum4 = y * tam;
	while (x >= 0) {
	  sum = sum4 + x;
	  dibujo[sum] = 0;
	  dibujo[sum - 1] = 0;
	  dibujo[sum - 2] = 0;
	  x -= bytes - 1;
	}
      }
    }
  }
  return 0;
}

static int filtro_gestos_rotar(lua_State * L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  ILfloat f =  (ILfloat)luaL_checkint(L, 2);
  ilBindImage(a->m_id);
  ILenum e;
  ilLoadL(IL_RAW, a->m_imagen, a->m_ancho * a->m_alto * a->m_bytes);
  while((e = ilGetError()) != IL_NO_ERROR) {
    printf("normal, no te jode: id del error es %i.\n", e);
  }
  iluRotate(f);
  return 0;
}

static int filtro_gestos_difuminar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  filtro_gestos_in_imagen_t *b = (filtro_gestos_in_imagen_t*)lua_touserdata(L, 2);
  int parametro_difuminado = luaL_checkint(L, 3);
  int reduccion = luaL_checkint(L, 4);
  color_t m_rojoInf = (color_t)luaL_checkint(L, 5);
  color_t m_rojoSup = (color_t)luaL_checkint(L, 6);
  color_t m_verdeInf = (color_t)luaL_checkint(L, 7);
  color_t m_verdeSup = (color_t)luaL_checkint(L, 8);
  color_t m_azulInf = (color_t)luaL_checkint(L, 9);
  color_t m_azulSup = (color_t)luaL_checkint(L, 10);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;  
  color_t *buffer = a->m_imagen;

  color_t *tipo_orden = b->m_imagen;

  int cont, cont2, acX, acX2, acY, acY2, pos;
  int tam = ancho * bytes;
  cont = cont2 = acX = acX2 = acY = acY2 = 0;
  int y, x, j, i;
  for (y = 0; y < alto; y++) {
    for (x = 0; x < tam; x+=bytes) {
      int rojo = 0;
      int verde = 0;
      int azul = 0;

      int param_bytes = parametro_difuminado * bytes;
      int param_bytes2 = (parametro_difuminado * bytes) + (bytes - 1);
      if (y - parametro_difuminado >= 0 && y + parametro_difuminado <= alto) {
	if (x - param_bytes >= 0 && x + param_bytes2 <= tam) {
	  for (i = y - parametro_difuminado; i <= y + parametro_difuminado; i++) {	  
	    for (j = x - param_bytes; j <= x + param_bytes2; j+=bytes) {
	      pos = i * tam + j;
	      rojo += buffer[pos];
	      verde += buffer[pos + 1];
	      azul += buffer[pos + 2];
	    }
	  }
	}
      }
		
      int numero = (2 * parametro_difuminado) + 1;
      numero *= numero;
      rojo = ((int)floor(rojo / numero));
      verde = ((int) floor(verde / numero));
      azul = ((int) floor(azul / numero));

      rojo -= rojo % reduccion;
      verde -= verde % reduccion;
      azul -= azul % reduccion;
      
      pos = y * tam + x;

      /* Atención: los colores rojo y azul están cambiados,
	 nosotro usamos el orden RGB */
      color_t azul_comparar = (color_t)rojo;
      color_t rojo_comparar = (color_t)azul;
      color_t verde_comparar = (color_t)verde;
      color_t valor = 0;
      if ((rojo_comparar >= m_rojoInf && rojo_comparar <= m_rojoSup)
	   && (verde_comparar >= m_verdeInf
	       && verde_comparar <= m_verdeSup)
	   && (azul_comparar >= m_azulInf
	       && azul_comparar <= m_azulSup)) {
	valor = 255;
	cont++;
	acX += x;
	acY += y;
      }
      tipo_orden[pos] = valor;
      tipo_orden[pos + 1] = valor;
      tipo_orden[pos + 2] = valor;
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
    {"delete", filtro_gestos_delete},
    {NULL, NULL}
  };

  static const struct luaL_reg arraylib_m [] = {
    {"set", filtro_gestos_setarray},
    {"get", filtro_gestos_getarray},
    {"size", filtro_gestos_getsize},
    {"copia", filtro_gestos_copia},
    {"difuminar", filtro_gestos_difuminar},
    {"centrar", filtro_gestos_centrar},
    {"rotar", filtro_gestos_rotar},
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
  dato->m_cerrar = g_hash_table_lookup(argumentos, "cerrar");
  dato->m_parametros = g_hash_table_lookup(argumentos, "parametros");
  ilInit();
  iluInit();
  return dato->m_buffer_error;;
}
static char *filtro_cerrar(modulo_t *modulo)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  filtro_llamar_funcion(dato->m_lua, dato->m_cerrar, "");
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
