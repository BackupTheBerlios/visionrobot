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
    return p;
}

elemento_t *nuevo(pipeline_t * pipeline, const char *nombre, gint x,
		  gint y, gchar * ruta)
{
    pipeline->m_elemento[pipeline->m_numero].m_widget =
	gtk_toggle_button_new_with_label(nombre);
    gtk_widget_show(pipeline->m_elemento[pipeline->m_numero].m_widget);
    pipeline->m_elemento[pipeline->m_numero].m_x = x;
    pipeline->m_elemento[pipeline->m_numero].m_y = y;
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_ruta, ruta);
    strcpy(pipeline->m_elemento[pipeline->m_numero].m_nombre, nombre);
    pipeline->m_elemento[pipeline->m_numero].m_handler =
#ifdef WIN32
	LoadLibrary(pipeline->m_elemento[pipeline->m_numero].m_ruta);
    pipeline->m_elemento[pipeline->m_numero].m_funcion_ciclo =
	(funcion_ciclo) GetProcAddress(pipeline->
				       m_elemento[pipeline->m_numero].
				       m_handler, TEXT("ciclo"));
#else
	dlopen(pipeline->m_elemento[pipeline->m_numero].m_ruta, RTLD_LAZY);
    pipeline->m_elemento[pipeline->m_numero].m_funcion_ciclo =
	(funcion_ciclo) dlsym(pipeline->m_elemento[pipeline->m_numero].
			      m_handler, "ciclo");
#endif

    pipeline->m_numero++;
    return &pipeline->m_elemento[pipeline->m_numero - 1];
}

int vaciar_pipeline(pipeline_t * pipeline)
{
    int id;
    for (id = 0; id < pipeline->m_numero; ++id) {
	if (pipeline->m_elemento[id].m_handler != NULL) {
#ifdef WIN32
	    FreeLibrary(pipeline->m_elemento[id].m_handler);
#else
	    dlclose(pipeline->m_elemento[id].m_handler);
#endif
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
#ifdef WIN32
	FreeLibrary(pipeline->m_elemento[id].m_handler);
#else
	dlclose(pipeline->m_elemento[id].m_handler);
#endif
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
    xmlDocPtr doc;
    char buffer[64];

    doc = xmlNewDoc(BAD_CAST "1.0");
    pipe = xmlNewNode(NULL, BAD_CAST "pipeline");


    int i;
    for (i = 0; i < pipeline->m_numero; ++i) {
	modulo = xmlNewNode(NULL, BAD_CAST "modulo");
	nombre = xmlNewNode(NULL, BAD_CAST "nombre");
	r = xmlNewNode(NULL, BAD_CAST "ruta");
	x = xmlNewNode(NULL, BAD_CAST "x");
	y = xmlNewNode(NULL, BAD_CAST "y");

	xmlAddChild(pipe, modulo);
	xmlAddChild(modulo, nombre);
	xmlAddChild(modulo, r);
	xmlAddChild(modulo, x);
	xmlAddChild(modulo, y);

	xmlNodeSetContent(nombre,
			  BAD_CAST pipeline->m_elemento[i].m_nombre);
	xmlNodeSetContent(r, BAD_CAST pipeline->m_elemento[i].m_ruta);
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_x);
	xmlNodeSetContent(x, BAD_CAST buffer);
	sprintf(buffer, "%i", pipeline->m_elemento[i].m_y);
	xmlNodeSetContent(y, BAD_CAST buffer);
    }
    xmlDocSetRootElement(doc, pipe);
    FILE *f = fopen(ruta, "w");
    xmlDocDump(f, doc);
    fclose(f);

    return 0;
}

void parseModulo(xmlDocPtr doc, xmlNodePtr cur, pipeline_t * pipeline)
{
    xmlChar *nombre;
    xmlChar *ruta;
    xmlChar *key;
    int x, y;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "nombre"))) {
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
	    ruta = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!ruta) {
		ruta = strdup("");
	    }
	}
	cur = cur->next;
    }
    nuevo(pipeline, nombre, x, y, ruta);
    free(nombre);
    free(ruta);
    return;
}

pipeline_t *cargar(const char *ruta)
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
    pipeline_t *pipe = crear_pipeline();
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    parseModulo(doc, cur, pipe);
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    return pipe;
}

void cambiar_biblioteca(pipeline_t * pipeline, gint id)
{
#ifdef WIN32
    if (pipeline->m_elemento[id].m_handler) {
	FreeLibrary(pipeline->m_elemento[id].m_handler);
    }
    pipeline->m_elemento[id].m_handler =
	LoadLibrary(pipeline->m_elemento[id].m_ruta);
    pipeline->m_elemento[id].m_funcion_ciclo =
	(funcion_ciclo) GetProcAddress(pipeline->m_elemento[id].m_handler,
				       TEXT("ciclo"));
#else
    if (pipeline->m_elemento[id].m_handler) {
	dlclose(pipeline->m_elemento[id].m_handler);
    }
    pipeline->m_elemento[id].m_handler =
	dlopen(pipeline->m_elemento[id].m_ruta, RTLD_LAZY);
    pipeline->m_elemento[id].m_funcion_ciclo =
	(funcion_ciclo) dlsym(pipeline->m_elemento[id].m_handler, "ciclo");
#endif
}
