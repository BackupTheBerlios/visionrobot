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
  pipeline_t *m_pipeline;
  gboolean m_inicio;
  GModule * m_handler;
  modulo_t * m_modulo;
  GHashTable *m_argumentos;
  GHashTable *m_enlaces;
  const char *m_nombre;
  gboolean m_activado;
} elemento_t;

typedef struct {
  char *m_salida;
  char *m_puerto;
} conexion_t;

struct pipeline_s {
  char **m_nombres;
  GHashTable *m_modulos;
  funcion_error_t m_funcion_error;
  gpointer m_dato_funcion_error;
};

static void pipeline_salida_error(const pipeline_t *pipeline, const char *nombre, const char *nombre2, const char *texto) {
    pipeline->m_funcion_error(nombre, nombre2, texto, pipeline->m_dato_funcion_error);
}


static int  pipeline_set_ruta(pipeline_t* p, const char * elemento, const char *ruta/*, const char *dir*/) {
  elemento_t *dato = g_hash_table_lookup(p->m_modulos, elemento);
  char ruta_modulo[128];
  int dev = 0;
  if (ruta) {
    if (dato->m_handler){
      g_module_close(dato->m_handler);
    }
#ifdef G_OS_WIN32
    sprintf(ruta_modulo, "%s.dll", ruta);
#else
    sprintf(ruta_modulo, "lib%s.so", ruta);
#endif
    
    dato->m_handler = g_module_open(ruta_modulo, G_MODULE_BIND_LAZY);
    
    if (dato->m_handler) {
      typedef modulo_t *(*funcion_modulo_t) ();
      funcion_modulo_t f;
      if(g_module_symbol(dato->m_handler, "get_modulo", (gpointer *)&f) != TRUE) {
	dev = -1;
      }
      else {
	dato->m_modulo = f ? f() : 0;
	dato->m_modulo->m_tabla = g_hash_table_new(g_str_hash, g_str_equal);
      }
    }    
    else {      
      dev = -1;
    }
    pipeline_salida_error(p, "Pipeline", "Bibliotecas", g_module_error());
  }
  else {
    dev = -1;
  }
  return dev;
}

static void pipeline_borrar_1_conexion(gpointer a, gpointer u) {
  conexion_t *c = (conexion_t*)a;
  free(c);
}
static void pipeline_borrar_conexion(gpointer a) {
	GSList *l = (GSList*)a;
//	g_slist_foreach(l, pipeline_borrar_1_conexion, 0); 
	g_slist_free(l);
}

static void pipeline_borrar_cadena(gpointer a) {
  char *c = (char *)a;
  if(c) {
    g_free(c);
  }
}

char ** pipeline_nombres(pipeline_t *p) {
  return p->m_nombres;
}

static void pipeline_cerrar_elemento(elemento_t *dato)
{
  if (dato->m_modulo && dato->m_modulo->m_cerrar)   {
    char *nombre = strdup(dato->m_modulo->m_nombre);
    char *cadena;
    g_hash_table_destroy(dato->m_modulo->m_tabla);
    cadena = strdup(dato->m_modulo->m_cerrar(dato->m_modulo));    
    pipeline_salida_error(dato->m_pipeline, dato->m_nombre, nombre,
			  cadena);
    free(cadena);
    free(nombre);
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

void pipeline_set_activo(const pipeline_t *p, const char *e, char activo) {
  elemento_t *elemento = g_hash_table_lookup(p->m_modulos, e);
  elemento->m_activado = activo;
}
char pipeline_get_activo(const pipeline_t *p, const char *e) {
  elemento_t *elemento = g_hash_table_lookup(p->m_modulos, e);
  return elemento->m_activado;
}

static pipeline_t * pipeline_annadir(pipeline_t * p, const char *nombre, const char *ruta,
				     GHashTable *argumentos, const char * inicio, gboolean activado) {
  elemento_t * dato = (elemento_t *)malloc(sizeof(elemento_t));
  dato->m_inicio = !strcmp(inicio, "1") ? TRUE : FALSE;
  dato->m_modulo = 0;
  dato->m_handler = 0;  
  dato->m_activado = activado;
  dato->m_nombre = strdup(nombre);
  p->m_nombres = (char **)realloc(p->m_nombres, sizeof(char **) * (g_hash_table_size(p->m_modulos) + 1));
  p->m_nombres[g_hash_table_size(p->m_modulos)] = (char *)nombre;
  dato->m_pipeline = p;
  dato->m_argumentos = argumentos;
  g_hash_table_insert(p->m_modulos, (gpointer)dato->m_nombre, (gpointer)dato);
  if(-1 == pipeline_set_ruta(p, nombre, ruta)) {
    return 0;
  }
  dato->m_enlaces = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_conexion);
  return p;
}

static void pipeline_conectar(const pipeline_t * p, const char *origen, const char *salida,
			      const char *destino, const char *puerto) {
  GSList* lista_destinos;
  elemento_t *or = g_hash_table_lookup(p->m_modulos, origen);
  conexion_t *conexion = (conexion_t*)malloc(sizeof(conexion_t));
  conexion->m_salida = (char*)salida;//strdup(salida);//
  conexion->m_puerto = (char *)puerto;//strdup(puerto);//
  lista_destinos = g_hash_table_lookup(or->m_enlaces, destino);  
  lista_destinos = g_slist_append(lista_destinos, conexion);  
  g_hash_table_insert(or->m_enlaces, (gpointer)destino, (gpointer)lista_destinos);
}

int pipeline_borrar(pipeline_t * p) 
{
  if(p) {
    g_hash_table_destroy(p->m_modulos);
    free(p->m_nombres);
    free(p);
    return 0;
  }
  else {
    return -1;
  }
}

static pipeline_t * pipeline_leer_xml(pipeline_t * p, xmlDocPtr doc, xmlNodePtr cur) 
{
  GHashTable *argumentos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_cadena);
  char *nombre = xmlGetProp(cur, "nombre");
  p = pipeline_annadir(p, nombre, xmlGetProp(cur, "ruta"), argumentos, xmlGetProp(cur, "inicio"), (gboolean)xmlGetProp(cur, "activado"));
  if(p == 0) {
    return 0;
  }
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

pipeline_t * pipeline_cargar(const char *ruta, funcion_error_t funcion_error, const void *dato) 
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlValidCtxt cvp;
    pipeline_t *p;

    if(!g_module_supported()) {
      return 0;
    }
    
    doc = xmlParseFile(ruta);
    if (doc == NULL) {
	return 0;
    }
    
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

    chdir(g_path_get_dirname(ruta));
    p = (pipeline_t*)malloc(sizeof(pipeline_t));
    p->m_modulos = g_hash_table_new_full(g_str_hash, g_str_equal, pipeline_borrar_cadena, pipeline_borrar_elemento);
    p->m_funcion_error = funcion_error;
    p->m_dato_funcion_error = (gpointer)dato;
    p->m_nombres = 0;
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	  p = pipeline_leer_xml(p, doc, cur/*, dir*/);
	    if(p == 0) {
	      xmlFreeDoc(doc);
	      pipeline_borrar(p);
	      return 0;
	    }
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    p->m_nombres = (char **)realloc(p->m_nombres, sizeof(char **) * (g_hash_table_size(p->m_modulos) + 1));
    p->m_nombres[g_hash_table_size(p->m_modulos)] = 0;
    return p;
}
static void pipeline_ciclo_recursivo(pipeline_t *p, elemento_t *e, const char *puerto_entrada, const void *dato_entrada);

typedef struct {
	elemento_t *e;
	char *k;
} enviar_t;

static void pipeline_enviar_1(gpointer value, gpointer user_data) {
  conexion_t *conexion = (conexion_t*)value;
  char *puerto_salida = conexion->m_salida;
  enviar_t *enviar = (enviar_t *)user_data;
  elemento_t *d = enviar->e;
  elemento_t *destino = g_hash_table_lookup(d->m_pipeline->m_modulos, enviar->k);
  void *dato = g_hash_table_lookup(d->m_modulo->m_tabla, puerto_salida);
  pipeline_ciclo_recursivo(d->m_pipeline, destino, conexion->m_puerto, dato);
}

static void pipeline_enviar(gpointer key, gpointer value, gpointer user_data) {
  GSList *l = (GSList *)value;
  elemento_t *d = (elemento_t *)user_data;
  enviar_t e = {d, (char*)key};
  g_slist_foreach(l, pipeline_enviar_1, &e);
}
static void pipeline_ciclo_recursivo(pipeline_t *p, elemento_t *e, const char *puerto_entrada, const void *dato_entrada)
{
  if(e->m_activado) {
    if(e && e->m_modulo && e->m_modulo->m_ciclo) {
      pipeline_salida_error(p, e->m_nombre, e->m_modulo->m_nombre,
			    e->m_modulo->m_ciclo(e->m_modulo, puerto_entrada, dato_entrada));
    }
    g_hash_table_foreach(e->m_enlaces, pipeline_enviar, e);
  }
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
    pipeline_salida_error((pipeline_t*)user_data, dato->m_nombre, dato->m_modulo->m_nombre,
			  dato->m_modulo->m_iniciar(dato->m_modulo, dato->m_argumentos));
  }
}

int  pipeline_iniciar(const pipeline_t * p) 
{
  g_hash_table_foreach(p->m_modulos, pipeline_iniciar_elemento, (gpointer)p);
  return 0;
}
