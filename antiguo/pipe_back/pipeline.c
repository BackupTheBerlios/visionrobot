/*! \file pipeline.c
    \brief Implementación del pipeline
    \author zosco
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


pipeline_t *crear_pipeline()
{
    pipeline_t *p = (pipeline_t *) malloc(sizeof(pipeline_t));
    p->m_numero = 0;
    p->m_corriendo = 0;
    return p;
}

elemento_t *nuevo(pipeline_t * pipeline, const char *nombre, gint x,
		  gint y, gchar * ruta)
{
    pipeline->m_elemento[pipeline->m_numero].m_widget =
	gtk_toggle_button_new_with_label(nombre);
    gtk_widget_show(pipeline->m_elemento[pipeline->m_numero].m_widget);
    pipeline->m_error = -1;
    pipeline->m_elemento[pipeline->m_numero].m_x = x;
    pipeline->m_elemento[pipeline->m_numero].m_id = pipeline->m_numero;
    pipeline->m_elemento[pipeline->m_numero].m_numero_conexiones = 0;
    pipeline->m_elemento[pipeline->m_numero].m_y = y;
    pipeline->m_elemento[pipeline->m_numero].m_iniciado = 0;
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_ruta, ruta);
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_nombre, nombre);
    pipeline->m_elemento[pipeline->m_numero].m_handler = 0;
    cambiar_biblioteca(&pipeline->m_elemento[pipeline->m_numero]);
    pipeline->m_numero++;
    return &pipeline->m_elemento[pipeline->m_numero - 1];
}

int cerrar_biblioteca(elemento_t * elemento) {
  if(elemento->m_funcion_cerrar) {
    // estoy hay que ponerlo, sólo si se exige que se controle en la dlls
    //elemento->m_funcion_cerrar();
  }
#ifdef WIN32
  FreeLibrary(elemento->m_handler);
#else
  dlclose(elemento->m_handler);
#endif
  
  return 0;
}

int cerrar_todas_bibliotecas(pipeline_t * pipeline) {
  int id;
  for (id = 0; id < pipeline->m_numero; ++id) {
    if (pipeline->m_elemento[id].m_handler != NULL) {
      cerrar_biblioteca(&pipeline->m_elemento[id]);
    }
  }
  return 0;
}

int vaciar_pipeline(pipeline_t * pipeline)
{
    int id;
    for (id = 0; id < pipeline->m_numero; ++id) {
	if (pipeline->m_elemento[id].m_handler != NULL) {
	  cerrar_biblioteca(&pipeline->m_elemento[id]);
	}
	gtk_widget_destroy(GTK_WIDGET(pipeline->m_elemento[id].m_widget));
    }
    free(pipeline);
    return 0;
}

int borrar(pipeline_t * pipeline, gint id)
{
    int i;
    gtk_widget_destroy(GTK_WIDGET(pipeline->m_elemento[id].m_widget));
    if (pipeline->m_elemento[id].m_handler) {
      cerrar_biblioteca(&pipeline->m_elemento[id]);
    }
    for (i = id; i < pipeline->m_numero; ++i) {
	pipeline->m_elemento[i] = pipeline->m_elemento[i + 1];
    }
    return --pipeline->m_numero;
}
int guardar(pipeline_t * pipeline, const char *ruta)
{
    xmlNodePtr pipe;
    xmlNodePtr modulo;
    xmlNodePtr nombre;
    xmlNodePtr r;
    xmlNodePtr x;
    xmlNodePtr y;
    xmlNodePtr error_pipe;
    xmlNodePtr * c;
    xmlNodePtr id;
    xmlDocPtr doc;    
    char buffer[64];

    doc = xmlNewDoc(BAD_CAST "1.0");
    pipe = xmlNewNode(NULL, BAD_CAST "pipeline");

sprintf(buffer, "%i", pipeline->m_error);
error_pipe = xmlNewNode(NULL, BAD_CAST "error_pipe");
xmlAddChild(pipe, error_pipe);
	xmlNodeSetContent(error_pipe,
			  BAD_CAST buffer);
			  
    int i, j;
    for (i = 0; i < pipeline->m_numero; ++i) {
	modulo = xmlNewNode(NULL, BAD_CAST "modulo");
	nombre = xmlNewNode(NULL, BAD_CAST "nombre");	
	r = xmlNewNode(NULL, BAD_CAST "ruta");
	x = xmlNewNode(NULL, BAD_CAST "x");
	y = xmlNewNode(NULL, BAD_CAST "y");
	id = xmlNewNode(NULL, BAD_CAST "id");

	xmlAddChild(pipe, modulo);
	xmlAddChild(modulo, id);
	xmlAddChild(modulo, nombre);
	xmlAddChild(modulo, r);
	xmlAddChild(modulo, x);
	xmlAddChild(modulo, y);
		
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_id);
	xmlNodeSetContent(id,
			  BAD_CAST buffer);
	xmlNodeSetContent(nombre,
			  BAD_CAST pipeline->m_elemento[i].m_nombre);
	xmlNodeSetContent(r, BAD_CAST pipeline->m_elemento[i].m_ruta);
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_x);
	xmlNodeSetContent(x, BAD_CAST buffer);
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_y);
	xmlNodeSetContent(y, BAD_CAST buffer);
	
	c = (xmlNodePtr*)malloc(sizeof(xmlNodePtr) * pipeline->m_elemento[i].m_numero_conexiones);
	for(j = 0; j < pipeline->m_elemento[i].m_numero_conexiones; ++j) {
	  c[j] = xmlNewNode(NULL, BAD_CAST "conexion");
	  xmlAddChild(modulo, c[j]);
	  sprintf(buffer, "%i", pipeline->m_elemento[i].m_destino[j]->m_id);
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

int parseModulo(xmlDocPtr doc, xmlNodePtr cur, pipeline_t * pipeline, guint **lista_objetivo, guint i)
{
    xmlChar *nombre = 0;
    xmlChar *ruta = 0;
    //guint c[MAX_CONEXIONES];
    xmlChar *key;
    int x, y;
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
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "x"))) {
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
	}
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
	    lista_objetivo[i][j] = atoi(key); j++;
	    free(key);
	}
	cur = cur->next;
    }
    nuevo(pipeline, nombre, x, y, ruta);
    free(nombre);
    free(ruta);
    return j;
}

pipeline_t *cargar(const char *ruta)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    int num_conexiones[MAX_CONEXIONES];
    guint **lista_objetivo;
    lista_objetivo = (guint **)malloc(sizeof(guint) * MAX_CONEXIONES);
    guint i;
    for(i = 0; i < MAX_CONEXIONES; ++i) {
      lista_objetivo[i] = (guint *)malloc(sizeof(guint) * MAX_CONEXIONES);
    }
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
    i = 0;
    pipeline_t *pipe = crear_pipeline();
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
	    num_conexiones[i] = parseModulo(doc, cur, pipe, lista_objetivo, i);
	    i++;
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    
    int k;
    for(k = 0; k < pipe->m_numero; ++k) {
      for(i = 0; i < num_conexiones[k]; ++i) {
	conectar(pipe, k, lista_objetivo[k][i]);
      }
    }

    for(i = 0; i < MAX_CONEXIONES; ++i) {
      free(lista_objetivo[i]);
    }
    free(lista_objetivo);
    return pipe;
}

int conectar(pipeline_t * pipeline, gint origen, gint destino)
{
    if (pipeline && origen < pipeline->m_numero
	&& destino < pipeline->m_numero && origen >= 0 && destino >= 0) {
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


void cambiar_biblioteca(elemento_t * elemento) //pipeline_t * pipeline, gint id)
{
#ifdef WIN32
  if (elemento->m_handler) {
    FreeLibrary(elemento->m_handler);
  }
    elemento->m_handler =
	LoadLibrary(elemento->m_ruta);
    if (elemento->m_handler) {
	elemento->m_funcion_ciclo =
	    (funcion_1) GetProcAddress(elemento->m_handler,
				       TEXT(F_CICLO));
	if(!elemento->m_funcion_ciclo)
	  elemento->m_funcion_ciclo =
	    (funcion_1) GetProcAddress(elemento->m_handler,
				       TEXT(F_CICLO_));
	
	elemento->m_funcion_iniciar =
	  (funcion_1) GetProcAddress(elemento->m_handler,
				     TEXT(F_INICIAR));
	if(!elemento->m_funcion_iniciar)
	  elemento->m_funcion_iniciar =
	    (funcion_1) GetProcAddress(elemento->m_handler,
				       TEXT(F_INICIAR_));
				       
   elemento->m_funcion_error =
	  (funcion_4) GetProcAddress(elemento->m_handler,
				     TEXT(F_ERROR));
	if(!elemento->m_funcion_error)
	  elemento->m_funcion_error =
	    (funcion_4) GetProcAddress(elemento->m_handler,
				       TEXT(F_ERROR_));               				       
	
	elemento->m_funcion_propiedades =
	  (funcion_1) GetProcAddress(elemento->m_handler,
				     TEXT(F_PROPIEDADES));
	if(!elemento->m_funcion_propiedades)
	  elemento->m_funcion_propiedades =
	    (funcion_1) GetProcAddress(elemento->m_handler,
				       TEXT(F_PROPIEDADES_));
	
	
	elemento->m_funcion_cerrar =
	  (funcion_1) GetProcAddress(elemento->m_handler,
				     TEXT(F_CERRAR));
	if(!elemento->m_funcion_cerrar)
	  elemento->m_funcion_cerrar =
	    (funcion_1) GetProcAddress(elemento->m_handler,
				       TEXT(F_CERRAR_));
	
	
	elemento->m_funcion_get_datos =
	  (funcion_2) GetProcAddress(elemento->m_handler,
				     TEXT(F_GET_DATOS));
	if(!elemento->m_funcion_get_datos)
	  elemento->m_funcion_get_datos =
	    (funcion_2) GetProcAddress(elemento->m_handler,
				       TEXT(F_GET_DATOS_));
	
	
	elemento->m_funcion_set_datos =
	  (funcion_3) GetProcAddress(elemento->m_handler,
				     TEXT(F_SET_DATOS));
	if(!elemento->m_funcion_set_datos)
	  elemento->m_funcion_set_datos =
	    (funcion_3) GetProcAddress(elemento->m_handler,
				       TEXT(F_SET_DATOS_));

    }
#else
    if (elemento->m_handler) {
	dlclose(elemento->m_handler);
    }
    elemento->m_handler =
	dlopen(elemento->m_ruta, RTLD_LAZY);
    if (elemento->m_handler) {
	elemento->m_funcion_ciclo =
	    (funcion_1) dlsym(elemento->m_handler, F_CICLO);
	elemento->m_funcion_iniciar =
	    (funcion_1) dlsym(elemento->m_handler,
			      F_INICIAR);
	elemento->m_funcion_propiedades =
	    (funcion_1) dlsym(elemento->m_handler,
			      F_PROPIEDADES);
	elemento->m_funcion_cerrar =
	    (funcion_1) dlsym(elemento->m_handler,
			      F_CERRAR);
	elemento->m_funcion_get_datos =
	    (funcion_2) dlsym(elemento->m_handler,
			      F_GET_DATOS);
	elemento->m_funcion_set_datos =
	    (funcion_3) dlsym(elemento->m_handler,
			      F_SET_DATOS);
   elemento->m_funcion_error =
	  (funcion_4) dlsym(elemento->m_handler,
				     TEXT(F_ERROR));
    }    
#endif
    else {
      elemento->m_funcion_ciclo = 0;
      elemento->m_funcion_iniciar = 0;
      elemento->m_funcion_propiedades = 0;
      elemento->m_funcion_cerrar = 0;
      elemento->m_funcion_get_datos = 0;
      elemento->m_funcion_set_datos = 0;
      elemento->m_funcion_error = 0;
    }
}

int pipeline_error(pipeline_t * pipeline, const char * error) {
    if(error && pipeline->m_error != -1) {
       if(pipeline->m_elemento[pipeline->m_error].m_funcion_set_datos) {
         pipeline->m_elemento[pipeline->m_error].m_funcion_set_datos((char *)error);
       }
    }
    
}
