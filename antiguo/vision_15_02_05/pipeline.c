/*! \file pipeline.c
    \brief Implementación del pipeline
    \author Carlos León
    \version 1.0
 */  
    
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */ 
#include "pipeline.h"
#include "pipeline_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libxml/parser.h>
#include <gmodule.h>

typedef struct {
  gboolean m_inicio;
  GModule * m_handler;
  modulo_t * m_modulo;
  GHashTable *m_argumentos;
  GHashTable *m_enlaces;
} elemento_t;

typedef struct {
  char *m_destino;
  char *m_puerto;
} conexion_t;

struct pipeline_s {
  GHashTable *m_modulos;
  funcion_error_t m_funcion_error;
};

typedef struct {
  pipeline_t *m_pipeline;
  elemento_t *m_elemento;
} pipeline_dato_t; 


static void pipeline_salida_error(const pipeline_t *pipeline, const char *nombre, const char *texto) {
    pipeline->m_funcion_error(nombre, texto);
}


static int  pipeline_set_ruta(pipeline_t* p, const char * elemento, const char *ruta, const char *dir) {
  elemento_t *dato = g_hash_table_lookup(p->m_modulos, elemento);
  int dev = 0;
  if (ruta) {
    if (dato->m_handler)      {
      g_module_close(dato->m_handler);
    }
    char *ruta_modulo = g_module_build_path(dir, ruta);
    dato->m_handler = g_module_open(ruta_modulo, G_MODULE_BIND_LAZY);

    if (dato->m_handler) {
      typedef modulo_t *(*funcion_modulo_t) ();
      funcion_modulo_t f;
      if(g_module_symbol(dato->m_handler, "get_modulo", (gpointer *)&f) != TRUE) {
	pipeline_salida_error(p, "Bibliotecas", g_module_error());
	dev = -1;
      }
      else {
	dato->m_modulo = f ? f() : 0;
	dato->m_modulo->m_tabla = g_hash_table_new(g_str_hash, g_str_equal);
	pipeline_salida_error(p, "Bibliotecas", g_module_error());
      }
    }    
    else {
      pipeline_salida_error(p, "Bibliotecas", g_module_error());
      dev = -1;
    }
  }
  else {
    dev = -1;
  }
  return dev;
}

static void pipeline_borrar_conexion(gpointer a) {
  conexion_t *c = (conexion_t*)a;
  free(c->m_destino);
  free(c->m_puerto);
  free(c);
}
static void pipeline_borrar_cadena(gpointer a) {
  char *c = (char *)a;
  g_free(c);
}

static void pipeline_cerrar_elemento(elemento_t *dato)
{
  if (dato->m_modulo && dato->m_modulo->m_cerrar)   {
    g_hash_table_destroy(dato->m_modulo->m_tabla);
  }
  if(dato->m_handler) {
    g_module_close(dato->m_handler);
  }
  dato->m_modulo = 0;
  dato->m_handler = 0;
}

static void pipeline_borrar_elemento(gpointer a) {
  elemento_t*dato = (elemento_t*)a;
  pipeline_cerrar_elemento(dato);
  if(dato->m_argumentos) g_hash_table_destroy(dato->m_argumentos);
  if(dato->m_enlaces) g_hash_table_destroy(dato->m_enlaces);
  free(dato);
}

static pipeline_t * pipeline_annadir(pipeline_t * p, const char *nombre, const char *ruta,
				     GHashTable *argumentos, const char *dir, const char * inicio) {
  elemento_t * dato = (elemento_t *)malloc(sizeof(elemento_t));
  dato->m_inicio = !strcmp(inicio, "1") ? TRUE : FALSE;
  dato->m_modulo = 0;
  dato->m_handler = 0;  
  dato->m_argumentos = argumentos;
  g_hash_table_insert(p->m_modulos, (gpointer)nombre, (gpointer)dato);
  pipeline_set_ruta(p, nombre, ruta, dir);
  dato->m_enlaces = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_conexion);
  return p;
}

static void pipeline_conectar(const pipeline_t * p, const char *origen, const char *salida,
			      const char *destino, const char *puerto) {
  elemento_t *or = g_hash_table_lookup(p->m_modulos, origen);
  conexion_t *conexion = (conexion_t*)malloc(sizeof(conexion_t));
  conexion->m_destino = strdup(destino);
  conexion->m_puerto = strdup(puerto);
  g_hash_table_insert(or->m_enlaces, (gpointer)strdup(salida), (gpointer)conexion);
}

int  pipeline_borrar(pipeline_t * p) 
{
  if(p) {
    g_hash_table_destroy(p->m_modulos);
    free(p);
    return 0;
  }
  else {
    return -1;
  }
}

static pipeline_t * pipeline_leer_xml(pipeline_t * p, xmlDocPtr doc, xmlNodePtr cur,
				      const char *dir) 
{
  GHashTable *argumentos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_cadena);
  char *nombre = xmlGetProp(cur, "nombre");
  p = pipeline_annadir(p, nombre, xmlGetProp(cur, "ruta"), argumentos, dir, xmlGetProp(cur, "inicio"));
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "argumento"))) {
      g_hash_table_insert(argumentos, xmlGetProp(cur, "nombre"), xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
    }    
    else if ((!xmlStrcmp(cur->name, (const xmlChar *) "conexion"))) {
      char *puerto_origen = xmlGetProp(cur, "puerto");
      xmlNodePtr cur2 = cur->xmlChildrenNode;
      while(cur2 != NULL){
	if((!xmlStrcmp(cur2->name, (const xmlChar *) "destino"))) {
	  char *destino = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
	  pipeline_conectar(p, nombre, puerto_origen, destino,
			    xmlGetProp(cur2, "puerto"));
	}
	cur2 = cur2->next;
      }      
    }  
    cur = cur->next;
  } 
  
  return p;
}

pipeline_t * pipeline_cargar(const char *ruta, const char *dir, funcion_error_t funcion_error) 
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(ruta);
    if (doc == NULL) {
	return 0;
    }

    xmlValidCtxt cvp;
    
    cvp.userData = (void *) stderr;
    cvp.error    = (xmlValidityErrorFunc) fprintf;
    cvp.warning  = (xmlValidityWarningFunc) fprintf;
    if (!xmlValidateDocument(&cvp, doc)) {
      return 0;
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	xmlFreeDoc(doc);
	return 0;
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "pipeline")) {
	xmlFreeDoc(doc);
	return 0;
    }
    pipeline_t *p = (pipeline_t*)malloc(sizeof(pipeline_t));
    p->m_modulos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_elemento);
    p->m_funcion_error = funcion_error;

    cur = cur->xmlChildrenNode;
    cur = cur->next;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    p = pipeline_leer_xml(p, doc, cur, dir);
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    return p;
}
static void pipeline_ciclo_recursivo(pipeline_t *p, elemento_t *e, const char *puerto_entrada, const void *dato_entrada);

static void pipeline_enviar(gpointer key, gpointer value, gpointer user_data) {
  char *salida = (char*)key;
  conexion_t *conexion = (conexion_t*)value;
  pipeline_dato_t *d = (pipeline_dato_t*)user_data;
  void *dato = g_hash_table_lookup(d->m_elemento->m_modulo->m_tabla, salida);
  elemento_t *destino = g_hash_table_lookup(d->m_pipeline->m_modulos, conexion->m_destino);
  pipeline_ciclo_recursivo(d->m_pipeline, destino, conexion->m_puerto, dato);
}
static void pipeline_ciclo_recursivo(pipeline_t *p, elemento_t *e, const char *puerto_entrada, const void *dato_entrada)
{
  if(e && e->m_modulo && e->m_modulo->m_ciclo) {
    pipeline_salida_error(p, e->m_modulo->m_nombre,
			  e->m_modulo->m_ciclo(e->m_modulo, puerto_entrada, dato_entrada));
  }
  pipeline_dato_t d = {p, e};
  g_hash_table_foreach(e->m_enlaces, pipeline_enviar, &d);
}
static void pipeline_ciclo_pre(gpointer key, gpointer value, gpointer user_data) {
  elemento_t*e = (elemento_t*)value;
  if(e->m_inicio) {
    pipeline_ciclo_recursivo((pipeline_t*)user_data, e, 0, 0);
  }
}

int pipeline_ciclo(const pipeline_t * p)
{  
  g_hash_table_foreach(p->m_modulos, pipeline_ciclo_pre, (gpointer)p);
  return 0;
}
static void pipeline_iniciar_elemento(gpointer key, gpointer value, gpointer user_data) 
{
  elemento_t *dato = (elemento_t*)value;
  if (dato->m_modulo && dato->m_modulo->m_iniciar) {
    pipeline_salida_error((pipeline_t*)user_data, dato->m_modulo->m_nombre,
			  dato->m_modulo->m_iniciar(dato->m_modulo, dato->m_argumentos));
  }
}

int  pipeline_iniciar(const pipeline_t * p) 
{
  g_hash_table_foreach(p->m_modulos, pipeline_iniciar_elemento, (gpointer)p);
  return 0;
}
