/*!
  \file   gestion.c
  \author Carlos León
  \date   Sat Mar 12 19:54:40 2005
  
  \brief  Módulo que se encarga de filtrar la salida de la red, y generar, a partir de ella, una salida coherente y útil.
  
  
*/

#include "pipeline_sdk.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>

#define PUERTO_ENTRADA "entrada_red"
#define PUERTO_SALIDA  "salida_texto"

typedef struct {
  char m_buffer_error[128];
  char *m_anterior;
  GHashTable* m_historial;
  int m_tolerancia;
  int m_maximo_valor;
  char *m_neutro;
} gestion_dato_t;

typedef struct {
  int m_valor;
} estado_t;

typedef struct {
  estado_t *m_estado;
  char *m_maximo;
  int m_valor;
  int m_maximo_valor;
  char *m_error;
} argumento_t;

static void gestion_restar(gpointer key, gpointer value, gpointer user_data) {
  estado_t* estado = (estado_t*)value;
  if(estado->m_valor > 0) {
    estado->m_valor--;
  }  
}

static void gestion_sumar(gpointer key, gpointer value, gpointer user_data) {
  estado_t* estado = (estado_t*)value;
  argumento_t *argumento = (argumento_t *)user_data;
  if(argumento->m_estado == estado) {
    if(estado->m_valor < argumento->m_maximo_valor) {
      estado->m_valor++;
    }    
  }
  else if(estado->m_valor > 0) {
    estado->m_valor--;
  }  
  if(argumento->m_maximo) {
    if(estado->m_valor > argumento->m_valor) {
      argumento->m_valor = estado->m_valor;
      argumento->m_maximo = key;
    }
  }
  else {
    argumento->m_maximo = key;
    argumento->m_valor = estado->m_valor;
  }
  sprintf(argumento->m_error, "%s[%s=%i] ", argumento->m_error, (char *)key, estado->m_valor);
}

static char *gestion_ciclo(modulo_t *modulo, const char *puerto, const void *entrada)
{
  char *devolver = 0;
  const char *cadena = (const char *)entrada;
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  if(cadena && !strcmp(PUERTO_ENTRADA, puerto)) {
    if(strcmp(cadena, dato->m_neutro) == 0) {
      // hay que poner tolerancia a "nogestos"
      g_hash_table_foreach(dato->m_historial, gestion_restar, 0);
    }
    else {
      int maximo_valor = dato->m_maximo_valor;
      estado_t * estado = g_hash_table_lookup(dato->m_historial, entrada);
      // Si esa orden ya existe, actualizamos su estado, si no, la metemos en la tabla;
      // así es más genérico, y vale para cualquier orden  
      if(!estado){      
	estado = (estado_t *)malloc(sizeof(estado_t));
	estado->m_valor = 1;
	g_hash_table_insert(dato->m_historial, strdup(cadena), estado);
      }
      dato->m_buffer_error [0] = '\0';
      argumento_t argumento = {estado, 0, 0, maximo_valor, dato->m_buffer_error};
      g_hash_table_foreach(dato->m_historial, gestion_sumar, &argumento);
      
      estado_t *buscar = (estado_t *)g_hash_table_lookup(dato->m_historial, argumento.m_maximo);
      char *resultado = 0;    
      if(buscar->m_valor > dato->m_tolerancia &&
	 argumento.m_maximo != dato->m_anterior) {      
	dato->m_anterior = argumento.m_maximo;            
	resultado = dato->m_anterior;
      }
      
      devolver = dato->m_buffer_error;
      g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, resultado);
    }
  }
  else {
     /* Si la red neuronal no esta enviando datos, el tampoco envia datos al modulo de salida. Para que este
     no escriba nada */
    g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);
  }
  return devolver;
}

static void gestion_borrar_cadena(gpointer a) {  
  char *c = (char *)a;
  if(c) {
    g_free(c);
  }
}


static void gestion_borrar_estado(gpointer a) {  
  estado_t *c = (estado_t *)a;
  if(c) {
    g_free(c);
  }
}


static char *gestion_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_insert(modulo->m_tabla, PUERTO_SALIDA, 0);  
  dato->m_anterior = 0;
  dato->m_neutro = strdup(g_hash_table_lookup(argumentos,"neutro"));
  dato->m_tolerancia = atoi(g_hash_table_lookup(argumentos,"tolerancia"));
  dato->m_maximo_valor = atoi(g_hash_table_lookup(argumentos,"maximo"));
  dato->m_historial = g_hash_table_new_full(g_str_hash,
					    g_str_equal,
					    gestion_borrar_cadena,
					    gestion_borrar_estado);
  sprintf(dato->m_buffer_error, "iniciado con neutro = \"%s\", tolerancia  = %i, maximo = %i", 
	  dato->m_neutro,
	  dato->m_tolerancia,
	  dato->m_maximo_valor);
  return dato->m_buffer_error;
}

static char *gestion_cerrar(modulo_t *modulo)
{
  gestion_dato_t *dato = (gestion_dato_t *)modulo->m_dato;
  g_hash_table_destroy(dato->m_historial);
  free(dato->m_neutro);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Gestion";
  modulo->m_iniciar = gestion_iniciar;
  modulo->m_cerrar = gestion_cerrar;
  modulo->m_ciclo = gestion_ciclo;
  modulo->m_dato = (gestion_dato_t *)malloc(sizeof(gestion_dato_t));
  return modulo;

}

