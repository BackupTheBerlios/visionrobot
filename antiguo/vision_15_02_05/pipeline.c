/*! \file pipeline.c
    \brief Implementación del pipeline
    \author Carlos León
    \version 0.1
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libxml/parser.h>
#include <gmodule.h>

typedef struct {
  GString * m_nombre;
  GModule * m_handler;
  modulo_t * m_modulo;
  funcion_error_t m_funcion_error;
  GHashTable *m_argumentos;
  GSList *m_enlaces;
} elemento_t;

typedef struct {
  char m_tipo;
  GHashTable *m_tabla;
} pipeline_dato_t; 
  

static void pipeline_salida_error(const elemento_t *elemento, const char *nombre, const char *texto) {
  elemento->m_funcion_error(nombre, texto);
}


static int  pipeline_set_ruta(elemento_t * dato, const char *ruta, const char *dir) {
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
	pipeline_salida_error(dato, "Bibliotecas", g_module_error());
      }
      else {
	dato->m_modulo = f ? f() : 0;
	dato->m_modulo->m_tabla = g_hash_table_new(0,0);
	pipeline_salida_error(dato, "Bibliotecas", g_module_error());
      }
    }
    
    else {
      pipeline_salida_error(dato, "Bibliotecas", g_module_error());
    }
  }
  return 0;
}



static pipeline_t * pipeline_annadir(pipeline_t * p, const char *nombre, const char *ruta,
			      funcion_error_t funcion_error, GHashTable *argumentos, const char *dir) {

  elemento_t * dato = (elemento_t *)malloc(sizeof(elemento_t));
  dato->m_nombre = g_string_new(nombre);
  dato->m_funcion_error = funcion_error;
  dato->m_modulo = 0;
  dato->m_enlaces = 0;
  dato->m_handler = 0;
  pipeline_set_ruta(dato, ruta, dir);
  dato->m_argumentos = argumentos;
  return g_slist_append(p, dato);
}

static elemento_t *pipeline_get_elemento(pipeline_t *p, const char *e) {
  pipeline_t *aux;
  for(aux = (pipeline_t*)p; aux; aux = g_slist_next(aux)) {
    char * c = ((elemento_t*)aux->data)->m_nombre->str;
    if(!strcmp(c, e)) {
      return (elemento_t*)aux->data;
    }
  }
  return 0;
}

static void pipeline_conectar(pipeline_t * p, const char *origen, const char *destino) {
  elemento_t *des = pipeline_get_elemento(p, destino);
  elemento_t *or = pipeline_get_elemento(p, origen);
  or->m_enlaces = g_slist_append(or->m_enlaces, des);
}


static void pipeline_borrar_cadena(gpointer a) {
  char *c = (char *)a;
  g_free(c);
}

static void pipeline_cerrar_elemento(gpointer lista, gpointer data) 
{
  elemento_t *dato = (elemento_t*)lista;
  if (dato->m_modulo && dato->m_modulo->m_cerrar)   {
    g_hash_table_destroy(dato->m_modulo->m_tabla);
    char *nombre = strdup(dato->m_modulo->m_nombre);
    char *mensaje = strdup(dato->m_modulo->m_cerrar(dato->m_modulo));
    pipeline_salida_error(dato, nombre, mensaje);			  
    free(nombre);
    free(mensaje);
  }
  if(dato->m_handler) {
    g_module_close(dato->m_handler);
  }
  dato->m_modulo = 0;
  dato->m_handler = 0;
}

int  pipeline_cerrar(pipeline_t * p) 
{
  g_slist_foreach(p, pipeline_cerrar_elemento, 0);
  return 0;
}

static void pipeline_borrar_elemento(gpointer e, gpointer data) {
  elemento_t *dato = (elemento_t*)e;
  if(dato->m_argumentos) {
    g_hash_table_destroy(dato->m_argumentos);
  }
  g_string_free(dato->m_nombre, TRUE);
  g_slist_free(dato->m_enlaces);
  free(dato);  
  pipeline_t *p = (pipeline_t *)data;
  g_slist_remove(p, dato);
}

int  pipeline_borrar(pipeline_t * p) 
{
  g_slist_foreach(p, pipeline_borrar_elemento, p);
  g_slist_free(p);
  return 0;
}

static pipeline_t * pipeline_leer_xml(pipeline_t * p, xmlDocPtr doc, xmlNodePtr cur,
				      funcion_error_t funcion_error, const char *dir) 
{
  GHashTable *argumentos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_cadena);
  p = pipeline_annadir(p, xmlGetProp(cur, "nombre"), xmlGetProp(cur, "ruta"), funcion_error, argumentos, dir);
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "argumento"))) {
      g_hash_table_insert(argumentos, xmlGetProp(cur, "tipo"), xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
    }    
    cur = cur->next;
  } 
  
  return p;
}

pipeline_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error, const char *dir) 
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
    pipeline_t * p = pipeline_annadir(0, "pipeline", 0, 0, 0, dir);
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    p = pipeline_leer_xml(p, doc, cur, funcion_error, dir);
	}
	else if((!xmlStrcmp(cur->name, (const xmlChar *) "conexion"))) {
	  pipeline_conectar(p, xmlGetProp(cur, "origen"), xmlGetProp(cur, "destino"));
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    return p;
}

static void pipeline_ciclo_recursivo(gpointer gp1, gpointer gp2) {  
  elemento_t *e = (elemento_t*)gp1;
  pipeline_dato_t *d = (pipeline_dato_t *)gp2;
  pipeline_dato_t d2;

  char tipo = d->m_tipo;
  GHashTable* tabla = d->m_tabla;
  if(e && e->m_modulo && e->m_modulo->m_ciclo) {
    pipeline_salida_error(e, e->m_modulo->m_nombre,
			  e->m_modulo->m_ciclo(e->m_modulo, tipo, tabla));
  }
			
  d2.m_tipo = e->m_modulo->m_tipo;
  d2.m_tabla = e->m_modulo->m_tabla;

 g_slist_foreach(e->m_enlaces, pipeline_ciclo_recursivo, &d2);
}

int pipeline_ciclo(const pipeline_t * p)
{  
  pipeline_dato_t d = {0, 0};
  g_slist_foreach(((elemento_t*)p->data)->m_enlaces, pipeline_ciclo_recursivo, &d);
  return 0;
}
static void pipeline_iniciar_elemento(gpointer el, gpointer data) 
{
  elemento_t *dato = (elemento_t*)el;
  if (dato->m_modulo && dato->m_modulo->m_iniciar) {
    pipeline_salida_error(dato, dato->m_modulo->m_nombre,
			  dato->m_modulo->m_iniciar(dato->m_modulo, dato->m_argumentos));
  }
}

int  pipeline_iniciar(const pipeline_t * p) 
{
  g_slist_foreach((pipeline_t*)p, pipeline_iniciar_elemento, 0);
  return 0;
}
