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

#define MAX_CONEXIONES 32

#ifdef WIN32
#define pipeline_free_library(x) FreeLibrary((x))
#define pipeline_load_library(x) LoadLibrary((x))
#define pipeline_get_function(x, y) GetProcAddress((x), TEXT((y)))
#else
#define pipeline_free_library(x) dlclose((x))
#define pipeline_load_library(x) dlopen((x), RTLD_LAZY)
#define pipeline_get_function(x, y) dlsym((x),(y))
#endif

#define pipeline_enviar_error(y, x) if((x)->m_funcion_error) pipeline_error((y), (x), (x)->m_funcion_error())

pipeline_t *pipeline_crear()
{
    pipeline_t *p = (pipeline_t *) malloc(sizeof(pipeline_t));
    p->m_numero = 0;
    p->m_corriendo = 0;
    p->m_error = -1;
    return p;
}

int pipeline_poner_a_cero(elemento_t * elemento) {
  elemento->m_handler = 0;
  	elemento->m_funcion_ciclo = 0;
	elemento->m_funcion_iniciar = 0;
	elemento->m_funcion_propiedades = 0;
	elemento->m_funcion_cerrar = 0;
	elemento->m_funcion_get_datos = 0;
	elemento->m_funcion_set_datos = 0;
	elemento->m_funcion_error = 0;
    return 0;
}

elemento_t *pipeline_nuevo(pipeline_t * pipeline, const char *nombre,
			      const char * ruta)
{
    pipeline->m_elemento[pipeline->m_numero].m_id = pipeline->m_numero;
    pipeline->m_elemento[pipeline->m_numero].m_numero_conexiones = 0;
    pipeline->m_elemento[pipeline->m_numero].m_iniciado = 0;
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_ruta, ruta);
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_nombre, nombre);
    pipeline_poner_a_cero(&pipeline->m_elemento[pipeline->m_numero]);
    pipeline_cambiar_biblioteca(&pipeline->m_elemento[pipeline->m_numero]);
    pipeline->m_numero++;
    return &pipeline->m_elemento[pipeline->m_numero - 1];
}



int pipeline_cerrar_biblioteca(elemento_t * elemento)
{
    if (elemento->m_funcion_cerrar) {	
	elemento->m_funcion_cerrar();
    }
    pipeline_free_library(elemento->m_handler);
    pipeline_poner_a_cero(elemento);
    return 0;
}

int pipeline_cerrar_todas_bibliotecas(pipeline_t * pipeline)
{
    int id;
    for (id = 0; id < pipeline->m_numero; ++id) {
	if (pipeline->m_elemento[id].m_handler != NULL) {
	    pipeline_cerrar_biblioteca(&pipeline->m_elemento[id]);
	}
    }
    return 0;
}

int pipeline_vaciar(pipeline_t * pipeline)
{
    int id;
    for (id = 0; id < pipeline->m_numero; ++id) {
      if (pipeline->m_elemento[id].m_handler != 0) {
	       pipeline_cerrar_biblioteca(&pipeline->m_elemento[id]);
      }
    }
    free(pipeline);
    return 0;
}

int pipeline_borrar(pipeline_t * pipeline, int id)
{
    int i;
    if (pipeline->m_elemento[id].m_handler) {
	   pipeline_cerrar_biblioteca(&pipeline->m_elemento[id]);
    }

    for(i = 0; i < pipeline->m_numero; ++i) {
      int j;
      for(j = 0; j < pipeline->m_elemento[i].m_numero_conexiones; ++j) {
	if(pipeline->m_elemento[i].m_destino[j]->m_id == id) {
	  pipeline_desconectar(pipeline, i, id);
	}
      }
    }

    for (i = id; i < pipeline->m_numero; ++i) {
    	pipeline->m_elemento[i] = pipeline->m_elemento[i + 1];
	    pipeline->m_elemento[i].m_id = i;
    }

    return --pipeline->m_numero;
}
int pipeline_guardar(const pipeline_t * pipeline, const char *ruta)
{
    xmlNodePtr pipe;
    xmlNodePtr modulo;
    xmlNodePtr nombre;
    xmlNodePtr r;
    xmlNodePtr error_pipe;
    xmlNodePtr *c;
    xmlNodePtr id;
    xmlDocPtr doc;
    char buffer[64];

    doc = xmlNewDoc(BAD_CAST "1.0");
    pipe = xmlNewNode(NULL, BAD_CAST "pipeline");

    sprintf(buffer, "%i", pipeline->m_error);
    error_pipe = xmlNewNode(NULL, BAD_CAST "error_pipe");
    xmlAddChild(pipe, error_pipe);
    xmlNodeSetContent(error_pipe, BAD_CAST buffer);

    int i, j;
    for (i = 0; i < pipeline->m_numero; ++i) {
	modulo = xmlNewNode(NULL, BAD_CAST "modulo");
	nombre = xmlNewNode(NULL, BAD_CAST "nombre");
	r = xmlNewNode(NULL, BAD_CAST "ruta");
/*	x = xmlNewNode(NULL, BAD_CAST "x");
	y = xmlNewNode(NULL, BAD_CAST "y");*/
	id = xmlNewNode(NULL, BAD_CAST "id");

	xmlAddChild(pipe, modulo);
	xmlAddChild(modulo, id);
	xmlAddChild(modulo, nombre);
	xmlAddChild(modulo, r);
/*	xmlAddChild(modulo, x);
	xmlAddChild(modulo, y);*/

	sprintf(buffer, "%i", pipeline->m_elemento[i].m_id);
	xmlNodeSetContent(id, BAD_CAST buffer);
	xmlNodeSetContent(nombre,
			  BAD_CAST pipeline->m_elemento[i].m_nombre);
	xmlNodeSetContent(r, BAD_CAST pipeline->m_elemento[i].m_ruta);
/*	sprintf(buffer, "%i", pipeline->m_elemento[i].m_x);
	xmlNodeSetName(x, BAD_CAST buffer);
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_y);
	xmlNodeSetName(y, BAD_CAST buffer);*/

	c = (xmlNodePtr *) malloc(sizeof(xmlNodePtr) *
				  pipeline->m_elemento[i].
				  m_numero_conexiones);
	for (j = 0; j < pipeline->m_elemento[i].m_numero_conexiones; ++j) {
	    c[j] = xmlNewNode(NULL, BAD_CAST "conexion");
	    xmlAddChild(modulo, c[j]);
	    sprintf(buffer, "%i",
		    pipeline->m_elemento[i].m_destino[j]->m_id);
	    xmlNodeSetContent(c[j], BAD_CAST buffer);
	}
	free(c);
    }
    xmlDocSetRootElement(doc, pipe);
    FILE *f = fopen(ruta, "w");
    xmlDocDump(f, doc);
    fclose(f);
    return 0;
}

typedef int  lista_objetivo_t[MAX_CONEXIONES][MAX_CONEXIONES];

int pipeline_parse_modulo(xmlDocPtr doc, xmlNodePtr cur, pipeline_t * pipeline,
		lista_objetivo_t lista_objetivo, int i)
{
    xmlChar *nombre = 0;
    xmlChar *ruta = 0;
    xmlChar *key;
    //int x, y;
    int j = 0;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "nombre"))) {
	    if (nombre)
		free(nombre);
	    nombre = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!nombre) {
		nombre = strdup("");
	    }
	}
	/*if ((!xmlStrcmp(cur->name, (const xmlChar *) "x"))) {
	    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!key) {
		key = strdup("");
	    }
	    x = atoi(key);
	    free(key);
	}
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "y"))) {
	    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!key) {
		key = strdup("");
	    }
	    y = atoi(key);
	    free(key);
	    }*/
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "ruta"))) {
	    if (ruta)
		free(ruta);
	    ruta = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!ruta) {
		ruta = strdup("");
	    }
	}
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "conexion"))) {
	    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!key) {
		key = strdup("");
	    }
	    lista_objetivo[i][j] = atoi(key);
	    j++;
	    free(key);
	}
	cur = cur->next;
    }
    pipeline_nuevo(pipeline, nombre, ruta);
    free(nombre);
    free(ruta);
    return j;
}

pipeline_t *pipeline_cargar(const char *ruta)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    int num_conexiones[MAX_CONEXIONES];
    //int **lista_objetivo;
    lista_objetivo_t lista_objetivo;
    /*    lista_objetivo = (int **) malloc(sizeof(int) *
						 MAX_CONEXIONES);
    int i;
    for (i = 0; i < MAX_CONEXIONES; ++i) {
	lista_objetivo[i] =
	    (int *) malloc(sizeof(int) *
				       MAX_CONEXIONES);
				       }*/
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
    int i = 0;
    pipeline_t *pipe = pipeline_crear();
    cur = cur->xmlChildrenNode;

    char *key;
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "error_pipe"))) {
	key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	if (!key) {
	    key = strdup("");
	}
	pipe->m_error = atoi(key);
	free(key);
    }

    cur = cur->next;


    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    num_conexiones[i] =
		pipeline_parse_modulo(doc, cur, pipe, lista_objetivo, i);
	    i++;
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);

    int k;
    for (k = 0; k < pipe->m_numero; ++k) {
	for (i = 0; i < num_conexiones[k]; ++i) {
	    pipeline_conectar(pipe, k, lista_objetivo[k][i]);
	}
    }

    /*for (i = 0; i < MAX_CONEXIONES; ++i) {
	free(lista_objetivo[i]);
    }
    free(lista_objetivo);*/
    return pipe;
}

int pipeline_conectar(pipeline_t * pipeline, int origen,
		      int destino)
{
    if (pipeline && origen < pipeline->m_numero
	&& destino < pipeline->m_numero && origen >= 0 && destino >= 0) {

      int i;
      for(i = 0; i < pipeline->m_elemento[origen].m_numero_conexiones; ++i) {
	if(pipeline->m_elemento[origen].m_destino[i]->m_id == destino) {
	  return -1;
	}
      }

      pipeline->m_elemento[origen].m_destino[pipeline->
					     m_elemento[origen].
					     m_numero_conexiones] =
	    &pipeline->m_elemento[destino];
	pipeline->m_elemento[origen].m_numero_conexiones++;
	return 0;
    } else {
	return -1;
    }
}


void pipeline_cambiar_biblioteca(elemento_t * elemento)
{
    if (elemento->m_handler) {
	pipeline_free_library(elemento->m_handler);
    }
    elemento->m_handler = pipeline_load_library(elemento->m_ruta);

    if (elemento->m_handler) {
	elemento->m_funcion_ciclo =
	    (funcion_1) pipeline_get_function(elemento->m_handler,
					      F_CICLO);
	elemento->m_funcion_iniciar =
	    (funcion_1) pipeline_get_function(elemento->m_handler,
					      F_INICIAR);
	elemento->m_funcion_propiedades =
	    (funcion_1) pipeline_get_function(elemento->m_handler,
					      F_PROPIEDADES);
	elemento->m_funcion_cerrar =
	    (funcion_1) pipeline_get_function(elemento->m_handler,
					      F_CERRAR);
	elemento->m_funcion_get_datos =
	    (funcion_2) pipeline_get_function(elemento->m_handler,
					      F_GET_DATOS);
	elemento->m_funcion_set_datos =
	    (funcion_3) pipeline_get_function(elemento->m_handler,
					      F_SET_DATOS);
	elemento->m_funcion_error =
	    (funcion_4) pipeline_get_function(elemento->m_handler,
					      F_ERROR);
    } else {
      pipeline_poner_a_cero(elemento);
    }
}

int pipeline_error(const pipeline_t * pipeline, const elemento_t * elemento, const char *error)
{
    if (error && pipeline->m_error != -1) {
	if (pipeline->m_elemento[pipeline->m_error].m_funcion_set_datos) {
    char buffer[128];
    sprintf(buffer, "%s: %s", elemento->m_nombre, (const char *) error);
	    pipeline->m_elemento[pipeline->m_error].
		m_funcion_set_datos(buffer);
	}
    }
    return 0;
}

int pipeline_ciclo(const pipeline_t * pipeline)
{
    int i;
    int j;
    for (i = 0; i < pipeline->m_numero; ++i) {
	if (pipeline->m_elemento[i].m_iniciado) {
	    if (pipeline->m_elemento[i].m_funcion_get_datos) {
		void *datos =
		    pipeline->m_elemento[i].m_funcion_get_datos();
		pipeline_enviar_error(pipeline, &pipeline->m_elemento[i]);
		for (j = 0;
		     j < pipeline->m_elemento[i].m_numero_conexiones;
		     ++j) {
		    if (pipeline->m_elemento[i].m_destino[j]->
			m_funcion_set_datos) {
			pipeline->m_elemento[i].m_destino[j]->
			    m_funcion_set_datos(datos);
			pipeline_enviar_error(pipeline,
					      pipeline->m_elemento[i].
					      m_destino[j]);
		    }
		}
	    }
	}
    }
    for (i = 0; i < pipeline->m_numero; ++i) {
	if (pipeline->m_elemento[i].m_iniciado) {
	    if (pipeline->m_elemento[i].m_funcion_ciclo) {
		pipeline->m_elemento[i].m_funcion_ciclo();
		pipeline_enviar_error(pipeline, &pipeline->m_elemento[i]);
	    }
	}
    }
    return 0;
}

int pipeline_iniciar(const pipeline_t * pipeline, elemento_t * elemento)
{
  if(!elemento->m_iniciado) {    
    	elemento->m_iniciado = 1;
    if (elemento->m_funcion_iniciar) {
	elemento->m_funcion_iniciar();
	pipeline_enviar_error(pipeline, elemento);

	return 0;
    }
  }
  return -1;
}

int pipeline_parar(const pipeline_t * pipeline, elemento_t * elemento)
{
    elemento->m_iniciado = 0;
    if (elemento->m_funcion_cerrar) {
	elemento->m_funcion_cerrar();
	pipeline_enviar_error(pipeline, elemento);
	return 0;
    }
    return -1;
}

int pipeline_abrir_propiedades(const pipeline_t * pipeline, const elemento_t * elemento) {
     if (elemento->m_funcion_propiedades) {
		    elemento->m_funcion_propiedades();
			  pipeline_enviar_error(pipeline, elemento);
			  return 0;
		}
		else {
      return -1;
    }
}


int pipeline_desconectar(pipeline_t * pipeline, int origen,int destino) {
  int i;
  for(i = 0; i < pipeline->m_elemento[origen].m_numero_conexiones; ++i) {
    if(pipeline->m_elemento[origen].m_destino[i]->m_id == destino) {
      int j;
      for(j = i; j < pipeline->m_elemento[origen].m_numero_conexiones; ++j) {
	      pipeline->m_elemento[origen].m_destino[j] = pipeline->m_elemento[origen].m_destino[j + 1];
      }
      pipeline->m_elemento[origen].m_numero_conexiones--;
    }
  }
  return 0;
}

int pipeline_iniciar_todas(pipeline_t * pipeline) {
    int i;
    pipeline_iniciar(pipeline, &pipeline->m_elemento[pipeline->m_error]);
    
    for (i = 0; i < pipeline->m_numero; ++i) {
      if (i != pipeline->m_error) {
	pipeline_iniciar(pipeline, &pipeline->m_elemento[i]);
      }    
    }
    return 0;
}
