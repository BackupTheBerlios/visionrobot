#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <lualib.h>
#include <lauxlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*! \brief El puerto de entrada, recibe un <code>char *</code> */
#define PUERTO "entrada_texto"

typedef struct {
  char m_error[128];
  lua_State *m_lua;
  char *m_funcion;
  int m_fd;
  guint m_timer;
  guint m_timer_id;
  char m_param;
} dato_robot_t;

static char *robot_ciclo(modulo_t *modulo, const char *puerto, const void *dato)
{
  dato_robot_t *dato_robot = (dato_robot_t *)modulo->m_dato;
  if(!strcmp(PUERTO, puerto) && dato) {
    lua_State *L = dato_robot->m_lua;
    lua_getglobal(L, dato_robot->m_funcion);
    lua_pushlightuserdata(L, dato_robot);
    lua_pushstring(L, (const char *)dato);    
    lua_pcall(L, 2, 1, 0);
    strcpy(dato_robot->m_error, (char *)lua_tostring(L, -1));
    return dato_robot->m_error;
  }
  else {
    return 0;
  }
}

gboolean robot_timer_avanzar(gpointer data) {
  dato_robot_t *dato = (dato_robot_t*)data;
  write(dato->m_fd, &dato->m_param, sizeof(char) );
  //  printf("JAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  return TRUE;
}

static void robot_cambiar_funcion(dato_robot_t *dato, GSourceFunc funcion) {
  g_source_remove(dato->m_timer_id);
  dato->m_timer_id = g_timeout_add(dato->m_timer, funcion, dato);  
}

static int robot_avanzar (lua_State *L) {
  dato_robot_t *dato = (dato_robot_t*)lua_touserdata(L, 1);
  dato->m_param = (char)luaL_checkint(L, 2);
  robot_cambiar_funcion(dato, robot_timer_avanzar);
  return 0;
}

static char *robot_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  dato->m_funcion = strdup(g_hash_table_lookup(argumentos, "funcion"));
  dato->m_lua = lua_open();
  lua_State *l = dato->m_lua;
  static const struct luaL_reg robot [] = {
    {"avanzar", robot_avanzar},
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

  dato->m_fd = open(g_hash_table_lookup(argumentos, "dispositivo"), O_RDWR | O_NOCTTY | O_NDELAY );
  dato->m_timer = atoi(g_hash_table_lookup(argumentos, "timer"));
  return "iniciado";
}
static char *robot_cerrar(modulo_t *modulo)
{
  dato_robot_t *dato = (dato_robot_t *)modulo->m_dato;
  close(dato->m_fd);
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

