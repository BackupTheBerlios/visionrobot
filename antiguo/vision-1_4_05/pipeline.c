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
  const elemento_t *aux = elemento;
  while(((dato_elemento_t*)aux->data)->m_funcion_error == 0) {
    aux = aux->parent;
  }
  ((dato_elemento_t*)aux->data)->m_funcion_error(nombre, texto);
}

static void pipeline_borrar_cadena(gpointer a) {
  char *c = (char *)a;
  g_free(c);
}

static int  pipeline_cerrar_elemento(elemento_t * elemento) 
{
  dato_elemento_t *dato = elemento->data;
  if (dato->m_modulo && dato->m_modulo->m_cerrar)   {
    char *nombre = strdup(dato->m_modulo->m_nombre);
    char *mensaje = strdup(dato->m_modulo->m_cerrar(dato->m_modulo));
    pipeline_salida_error(elemento, nombre, mensaje);			  
    free(nombre);
    free(mensaje);
  }
  g_module_close(dato->m_handler);
  dato->m_modulo = 0;
  dato->m_handler = 0;
  return 0;
}

static void pipeline_cerrar_recursivo(elemento_t *elemento, gpointer data) {
  pipeline_cerrar_elemento(elemento);
  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_cerrar_recursivo, 0);
}


int  pipeline_cerrar(elemento_t * elemento) 
{
  pipeline_cerrar_recursivo(elemento, 0);
  return 0;
}

static void pipeline_borrar_recursivo(elemento_t* elemento, gpointer data) {
  dato_elemento_t *dato = elemento->data;
  g_hash_table_destroy(dato->m_argumentos);
  free(dato);  
  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_borrar_recursivo, 0);
  g_node_destroy(elemento);
}
int  pipeline_borrar(elemento_t * elemento) 
{
  pipeline_borrar_recursivo(elemento, 0);
  return 0;
}

static void pipeline_meter_argumento(gpointer key, gpointer value, gpointer user_data) {
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

static elemento_t * pipeline_leer_xml(xmlDocPtr doc, xmlNodePtr cur,
				  elemento_t * padre, funcion_error_t funcion_error) 
{
  GHashTable *argumentos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_cadena);
  elemento_t * elemento =  pipeline_annadir(padre, xmlGetProp(cur, "ruta"), funcion_error, argumentos);
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "argumento"))) {
      g_hash_table_insert(argumentos, xmlGetProp(cur, "tipo"), xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
    }    
    else if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
      pipeline_leer_xml(doc, cur, elemento, 0);
    }
    cur = cur->next;
  } 
  
  return elemento;
}

elemento_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error) 
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
    elemento_t * elemento;
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    if (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    elemento = pipeline_leer_xml(doc, cur, 0, funcion_error);
	}
    }
    xmlFreeDoc(doc);
    return elemento;
}

static void pipeline_ciclo_recursivo(elemento_t *elemento, gpointer data) {
  pipeline_dato_t *dato_ciclo = (pipeline_dato_t *)data;
  dato_elemento_t *dato = elemento->data;
  pipeline_dato_t arg = {0, 0};
  int i;
  if (dato->m_modulo && dato->m_modulo->m_ciclo) {
    char *error = dato->m_modulo->m_ciclo(dato->m_modulo, dato_ciclo, &arg);
    pipeline_salida_error(elemento, dato->m_modulo->m_nombre, error);
    g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_ciclo_recursivo, &arg);
  }
}

int pipeline_ciclo(const elemento_t * elemento, const pipeline_dato_t *dato) 
{
  pipeline_ciclo_recursivo((GNode*)elemento, (gpointer)dato);
  return 0;
}
static int pipeline_iniciar_elemento(const elemento_t * elemento) 
{
  dato_elemento_t *dato = elemento->data;
  if (dato->m_modulo && dato->m_modulo->m_iniciar) {
    pipeline_salida_error(elemento, dato->m_modulo->m_nombre,
			  dato->m_modulo->m_iniciar(dato->m_modulo, dato->m_argumentos));
    return 0;
  }
  
  else {
    return -1;
  }
}

static void pipeline_iniciar_recursivo(elemento_t *elemento, gpointer data) {
  pipeline_iniciar_elemento(elemento);
  g_node_children_foreach(elemento, G_TRAVERSE_ALL, pipeline_iniciar_recursivo, 0);
}

int  pipeline_iniciar(const elemento_t * elemento) 
{
  pipeline_iniciar_recursivo((GNode*)elemento, 0);
  return 0;
}

static int  pipeline_set_ruta(elemento_t * elemento, const char *ruta) {
  dato_elemento_t *dato = elemento->data;
  if (ruta) {
    if (dato->m_handler)      {
      g_module_close(dato->m_handler);
    }
    dato->m_handler = g_module_open(ruta, G_MODULE_BIND_LAZY);
    if (dato->m_handler) {
      typedef modulo_t *(*funcion_modulo_t) ();
      funcion_modulo_t f;
      if(g_module_symbol(dato->m_handler, "get_modulo", (gpointer *)&f) != TRUE) {
	pipeline_salida_error(elemento, "Bibliotecas", g_module_error());
      }
      else {
	dato->m_modulo = f ? f() : 0;
      }
    }
    
    else {
      pipeline_salida_error(elemento, "Bibliotecas", g_module_error());
    }
  }
  return 0;
}


elemento_t * pipeline_annadir(elemento_t * padre, const char *ruta, funcion_error_t funcion_error, GHashTable *argumentos) {
  dato_elemento_t * dato = (dato_elemento_t *)malloc(sizeof(dato_elemento_t));
  dato->m_funcion_error = funcion_error;
  dato->m_modulo = 0;
  dato->m_handler = 0;
  elemento_t *elemento = g_node_new(dato);
  pipeline_set_ruta(elemento, ruta);
  dato->m_argumentos = argumentos;//g_hash_table_new_full(g_str_hash, pipeline_iguales, pipeline_borrar_cadena, pipeline_borrar_cadena);
  return padre ? g_node_insert(padre, -1, elemento) : elemento;
}
