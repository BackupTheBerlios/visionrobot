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



static void pipeline_salida_error(const elemento_t *elemento, const char *nombre, const char *texto) {
  /*const elemento_t *aux = elemento;
    while(((dato_elemento_t*)aux->data)->m_funcion_error == 0) {
    aux = aux->parent;
    }*/
  //((dato_elemento_t*)aux->data)
  elemento->m_funcion_error(nombre, texto);
}


static int  pipeline_set_ruta(elemento_t * dato, const char *ruta) {
  if (ruta) {
    if (dato->m_handler)      {
      g_module_close(dato->m_handler);
    }
    dato->m_handler = g_module_open(ruta, G_MODULE_BIND_LAZY);
    if (dato->m_handler) {
      typedef modulo_t *(*funcion_modulo_t) ();
      funcion_modulo_t f;
      if(g_module_symbol(dato->m_handler, "get_modulo", (gpointer *)&f) != TRUE) {
	pipeline_salida_error(dato, "Bibliotecas", g_module_error());
      }
      else {
	dato->m_modulo = f ? f() : 0;
	dato->m_modulo->m_tabla = g_hash_table_new(0,0);//(g_int_hash, g_int_equal);
      }
    }
    
    else {
      pipeline_salida_error(dato, "Bibliotecas", g_module_error());
    }
  }
  return 0;
}



static pipeline_t * pipeline_annadir(pipeline_t * p, const char *nombre, const char *ruta,
			      funcion_error_t funcion_error, GHashTable *argumentos) {

  elemento_t * dato = (elemento_t *)malloc(sizeof(elemento_t));
  dato->m_nombre = g_string_new(nombre);
  dato->m_funcion_error = funcion_error;
  dato->m_modulo = 0;
  dato->m_enlaces = 0;
  dato->m_handler = 0;
  //  elemento_t *elemento = g_node_new(dato);
  pipeline_set_ruta(dato, ruta);
  dato->m_argumentos = argumentos;
  return g_slist_append(p, dato);
}

static elemento_t *pipeline_get_elemento(pipeline_t *p, const char *e) {
  /*pipeline_t *aux;
    for(aux = (pipeline_t*)p; aux != g_slist_last((pipeline_t*)p); aux = g_slist_next(aux)) {*/
  guint i;
  for(i = 0; i < g_slist_length(p); i++) {
    gpointer aux = g_slist_nth_data(p, i);
    char * c = ((elemento_t*)aux)->m_nombre->str;
    if(!strcmp(c, e)) {
      return (elemento_t*)aux;
    }
  }
  return 0;
}

static void pipeline_conectar(pipeline_t * p, const char *origen, const char *destino) {
  elemento_t *des = pipeline_get_elemento(p, destino);
  elemento_t *or = pipeline_get_elemento(p, origen);
  or->m_enlaces = g_slist_append(or->m_enlaces, &des);
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
  g_module_close(dato->m_handler);
  dato->m_modulo = 0;
  dato->m_handler = 0;
}
/*
  static void pipeline_cerrar_recursivo(elemento_t *elemento, gpointer data) {
  pipeline_cerrar_elemento(elemento);
  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_cerrar_recursivo, 0);
  }*/


int  pipeline_cerrar(pipeline_t * p) 
{
  //  pipeline_cerrar_elemento(elemento, 0);
  g_slist_foreach(p, pipeline_cerrar_elemento, 0);
  return 0;
}

static void pipeline_borrar_elemento(gpointer lista, gpointer data) {
  elemento_t *dato = (elemento_t*)((GList*)lista)->data;
  g_hash_table_destroy(dato->m_argumentos);
  g_string_free(dato->m_nombre, TRUE);
  g_slist_free(dato->m_enlaces);
  free(dato);  
  pipeline_t *p = (pipeline_t *)data;
  g_slist_remove(p, dato);
  /*  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_borrar_recursivo, 0);
      g_node_destroy(elemento);*/
}
int  pipeline_borrar(pipeline_t * p) 
{
  //pipeline_borrar_recursivo(elemento, 0);
  g_slist_foreach(p, pipeline_borrar_elemento, p);
  g_slist_free(p);
  return 0;
}

/*static void pipeline_meter_argumento(gpointer key, gpointer value, gpointer user_data) {
  xmlNodePtr modulo = (xmlNodePtr) user_data;
  xmlNodePtr arg = xmlNewNode(NULL, BAD_CAST "argumento");
  xmlAddChild(modulo, arg);
  xmlNewProp(arg, "tipo", BAD_CAST key);
  xmlNodeSetContent(arg, BAD_CAST value);
}

static xmlNodePtr pipeline_get_xml(const elemento_t * elemento);

static void pipeline_meter_modulo(GNode * node, gpointer user_data) {
  xmlNodePtr modulo = (xmlNodePtr) user_data;
  xmlAddChild(modulo, pipeline_get_xml(node));
}

static xmlNodePtr pipeline_get_xml(const elemento_t * elemento){
  dato_elemento_t *dato = elemento->data;
  xmlNodePtr modulo = xmlNewNode(NULL, BAD_CAST "modulo");
  xmlNewProp(modulo, "ruta", BAD_CAST g_module_name(dato->m_handler));
  g_hash_table_foreach(dato->m_argumentos, pipeline_meter_argumento, modulo);
  g_node_children_foreach((GNode*)elemento, G_TRAVERSE_ALL, pipeline_meter_modulo, modulo);
  return modulo;
}

int  pipeline_guardar(const elemento_t * elemento, const char *ruta) {
  xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
  xmlNodePtr pipe = xmlNewNode(NULL, BAD_CAST "pipeline");
  xmlAddChild(pipe, pipeline_get_xml(elemento));
  xmlDocSetRootElement(doc, pipe);
  FILE * f = fopen(ruta, "w");
  xmlDocDump(f, doc);
  fclose(f);
  return 0;
}
*/
static pipeline_t * pipeline_leer_xml(pipeline_t * p, xmlDocPtr doc, xmlNodePtr cur,
				      funcion_error_t funcion_error) 
{
  GHashTable *argumentos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_cadena);
  p = pipeline_annadir(p, xmlGetProp(cur, "nombre"), xmlGetProp(cur, "ruta"), funcion_error, argumentos);
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "argumento"))) {
      g_hash_table_insert(argumentos, xmlGetProp(cur, "tipo"), xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
    }    
    /*else if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
      pipeline_leer_xml(doc, cur, elemento, 0);
      }*/
    cur = cur->next;
  } 
  
  return p;
}

pipeline_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error) 
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(ruta);
    if (doc == NULL) {
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
    pipeline_t * p = 0;
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    p = pipeline_leer_xml(p, doc, cur, funcion_error);
	}
	else if((!xmlStrcmp(cur->name, (const xmlChar *) "conexion"))) {
	  pipeline_conectar(p, xmlGetProp(cur, "origen"), xmlGetProp(cur, "destino"));
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    return p;
}

/*static void pipeline_ciclo_elemento(elemento_t *elemento, gpointer data) {
  typedef struct {
    char m_tipo;
    GHashTable *m_tabla;
  } pipeline_dato_t;  
  
  pipeline_dato_t *dato_ciclo = (pipeline_dato_t *)data;
  dato_elemento_t *dato = elemento->data;
  //GHashTable *tabla = g_hash_table_new(0,0);//(g_int_hash, g_int_equal);
  //  pipeline_dato_t arg = {dato_ciclo, 0};
  if (dato->m_modulo && dato->m_modulo->m_ciclo) {
    char tipo = dato_ciclo ? dato_ciclo->m_tipo : 0;
    GHashTable* tabla = dato_ciclo ? dato_ciclo->m_tabla : 0;
    char *error = dato->m_modulo->m_ciclo(dato->m_modulo, tipo, tabla);//dato_ciclo, &arg);
    pipeline_salida_error(elemento, dato->m_modulo->m_nombre, error);
    pipeline_dato_t arg = {dato->m_modulo->m_tipo, dato->m_modulo->m_tabla};
    g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_ciclo_recursivo, &arg);
  }
  //  g_hash_table_destroy(tabla);
  }*/

int pipeline_ciclo(const pipeline_t * p)//, const pipeline_dato_t *dato) 
{
  typedef struct {
    char m_tipo;
    GHashTable *m_tabla;
  } pipeline_dato_t;  
  
  pipeline_dato_t dato_ciclo = {0, 0};//(pipeline_dato_t *)data;

  pipeline_t *aux;
  for(aux = (pipeline_t*)p; aux != g_slist_last((pipeline_t*)p); aux = g_slist_next(aux)) {
    elemento_t *dato = aux->data;
    char tipo = /*dato_ciclo ? */dato_ciclo.m_tipo;// : 0;
    GHashTable* tabla = /*dato_ciclo ? */dato_ciclo.m_tabla;// : 0;
    char *error = dato->m_modulo->m_ciclo(dato->m_modulo, tipo, tabla);
    pipeline_salida_error(dato, dato->m_modulo->m_nombre, error);
    dato_ciclo.m_tipo = dato->m_modulo->m_tipo;
    dato_ciclo.m_tabla = dato->m_modulo->m_tabla;
    //    dato_ciclo = &arg;
    //    g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_ciclo_recursivo, &arg);
  }
  // g_slist_foreach(p, pipeline_ciclo_elemento, 0);
  //pipeline_ciclo_recursivo((GNode*)elemento, /*(gpointer)dato*/0);
  return 0;
}
static void pipeline_iniciar_elemento(gpointer lista, gpointer data) 
{
  elemento_t *dato = (elemento_t*)lista;//((GList*)lista)->data;
  //  dato_elemento_t *dato = elemento->data;
  if (dato->m_modulo && dato->m_modulo->m_iniciar) {
    pipeline_salida_error(dato, dato->m_modulo->m_nombre,
			  dato->m_modulo->m_iniciar(dato->m_modulo, dato->m_argumentos));
  }
}

/*static void pipeline_iniciar_recursivo(elemento_t *elemento, gpointer data) {
  pipeline_iniciar_elemento(elemento);
  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_iniciar_recursivo, 0);
  }*/

int  pipeline_iniciar(const pipeline_t * p) 
{
  g_slist_foreach((pipeline_t*)p, pipeline_iniciar_elemento, 0);
  //pipeline_iniciar_recursivo((GNode*)elemento, 0);
  return 0;
}
