/*! \file pipeline.c
    \brief Implementaci�n del pipeline
    \author Carlos Le�n
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

void pipeline_salida_error(const char*nombre, const char *aux) {
    if(aux && nombre) {
        fprintf(stderr, "%s: %s.\n", nombre, aux);
    }
}
elemento_t *
pipeline_nuevo (elemento_t * padre,const char *ruta)
{
  elemento_t * elemento = (elemento_t *) malloc (sizeof (elemento_t));
  elemento->m_numero_conexiones = 0;
  elemento->m_numero_argumentos = 0;
  elemento->m_destino = 0;
  elemento->m_handler = 0;
  elemento->m_ruta = 0;
  elemento->m_padre = padre;
  elemento->m_argumentos = 0;
  pipeline_set_ruta(elemento, ruta);
    if(padre) {
        padre->m_destino = (elemento_t **) realloc
        (padre->m_destino, sizeof (elemento_t *) * (padre->m_numero_conexiones + 1));
        padre->m_destino[padre->m_numero_conexiones] = elemento;
        padre->m_numero_conexiones++;
    }
  return elemento;
}



int
pipeline_cerrar_biblioteca (elemento_t * elemento)
{
  if (elemento->m_modulo && elemento->m_modulo->m_cerrar)
    {
      pipeline_salida_error(elemento->m_modulo->m_nombre, 
                elemento->m_modulo->m_cerrar ());
    }
  pipeline_free_library (elemento->m_handler);
    elemento->m_modulo = 0;
  return 0;
}

int
pipeline_cerrar_todo (elemento_t * elemento)
{
  int j, dev = 0;
  if (elemento->m_handler != NULL){
           pipeline_cerrar_biblioteca (elemento);    
           for(j = 0; j < elemento->m_numero_conexiones; ++j) {
            dev |= pipeline_cerrar_todo(elemento->m_destino[j]);
           }
      }

  return dev;
}

int
pipeline_borrar (elemento_t *elemento)
{
    int j;
    for(j = 0; j < elemento->m_numero_conexiones; ++j) {
        pipeline_borrar(elemento->m_destino[j]);
    }
    free(elemento->m_ruta);    
    pipeline_borrar_argumentos (elemento);
    
    if(elemento->m_padre) {
    elemento->m_padre->m_destino = 
         (elemento_t **) realloc (elemento->m_padre->m_destino,
                          sizeof (elemento_t *) *
                          (elemento->m_padre->m_numero_conexiones - 1));
    elemento->m_padre->m_numero_conexiones--;
                           
    }   
    free(elemento);
  return 0;
}

xmlNodePtr pipeline_get_xml(const elemento_t *elemento) {
    xmlNodePtr modulo = xmlNewNode (NULL, BAD_CAST "modulo");
    xmlNewProp (modulo, "ruta", BAD_CAST elemento->m_ruta);
    int j;
    for (j = 0; j < elemento->m_numero_argumentos; j += 2)
    {
      xmlNodePtr arg = xmlNewNode (NULL, BAD_CAST "argumento");
      xmlAddChild (modulo, arg);
      xmlNewProp (arg, "tipo", BAD_CAST elemento->m_argumentos[j]);
      xmlNodeSetContent (arg, BAD_CAST elemento->m_argumentos[j + 1]);
    }
    for(j = 0; j < elemento->m_numero_conexiones; ++j) {
        xmlAddChild(modulo, pipeline_get_xml(elemento->m_destino[j]));
    }
    return modulo;
}
int
pipeline_guardar (const elemento_t *elemento, const char *ruta)
{
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  xmlNodePtr pipe = xmlNewNode (NULL, BAD_CAST "pipeline");
  xmlAddChild (pipe, pipeline_get_xml(elemento));

  xmlDocSetRootElement (doc, pipe);
  FILE *f = fopen (ruta, "w");
  xmlDocDump (f, doc);
  fclose (f);
  return 0;
}

elemento_t *
pipeline_leer_xml (xmlDocPtr doc, xmlNodePtr cur, elemento_t * padre)
{
    xmlChar **argumentos = 0;
    int numero_argumentos = 0;         
    elemento_t * elemento = pipeline_nuevo(padre, xmlGetProp(cur, "ruta"));     
    cur = cur->xmlChildrenNode;  
    while (cur != NULL) {
        if ((!xmlStrcmp (cur->name, (const xmlChar *) "argumento"))) {
            char *arg = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
            if (!arg)  {
                arg = strdup ("");
            }
            char* tipo = xmlGetProp(cur, "tipo");
            if (!tipo)  {
                tipo = strdup ("");
            }
            argumentos = (xmlChar **) realloc (argumentos,
                         sizeof (xmlChar *) *
                         2 * (numero_argumentos + 1));
            argumentos[2 * numero_argumentos] = tipo;
            argumentos[(2 * numero_argumentos) + 1] = arg;
            numero_argumentos += 1;
        }
        else if ((!xmlStrcmp (cur->name, (const xmlChar *) "modulo"))) {
            pipeline_leer_xml(doc, cur, elemento);
        }
        cur = cur->next;
    }
    pipeline_set_argumentos(elemento, (const char**)argumentos, 2 * numero_argumentos);
    return elemento;

}

elemento_t *
pipeline_cargar (const char *ruta)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (ruta);
  if (doc == NULL){
      return 0;
    }
  cur = xmlDocGetRootElement (doc);
  if (cur == NULL) {
      xmlFreeDoc (doc);
      return 0;
    }
  if (xmlStrcmp (cur->name, (const xmlChar *) "pipeline")) {
      xmlFreeDoc (doc);
      return 0;
    }
  elemento_t *elemento;
  cur = cur->xmlChildrenNode;

  cur = cur->next;
  if (cur != NULL) {
      if ((!xmlStrcmp (cur->name, (const xmlChar *) "modulo"))){
        elemento = pipeline_leer_xml(doc, cur, 0);
      }
    }
  xmlFreeDoc (doc);

  return elemento;
}

int
pipeline_ciclo (const elemento_t * elemento, const void *dato)
{
  void *arg = 0;
  int i;
  
    if (elemento->m_modulo && elemento->m_modulo->m_ciclo) {
      pipeline_salida_error(elemento->m_modulo->m_nombre,
            elemento->m_modulo->m_ciclo (dato, &arg));	  
	  for (i = 0; i < elemento->m_numero_conexiones; ++i) {
        pipeline_ciclo(elemento->m_destino[i], arg);
      }    
    }
  return 0;
}

int
pipeline_iniciar (const elemento_t * elemento)
{
  if(elemento->m_modulo && elemento->m_modulo->m_iniciar) {
    pipeline_salida_error(elemento->m_modulo->m_nombre,
            elemento->m_modulo->m_iniciar (elemento->m_numero_argumentos,
                 (const char **) elemento->m_argumentos));
    return 0;                                  
  }   
  else {
    return -1;
  }
    
}

int
pipeline_iniciar_todo (const elemento_t * elemento)
{
  int i, dev = 0;
  pipeline_iniciar (elemento);
  for (i = 0; i < elemento->m_numero_conexiones; ++i) {
      dev |= pipeline_iniciar_todo(elemento->m_destino[i]);
  }
  return dev;
  
  
}



void
pipeline_borrar_argumentos (elemento_t *elemento)
{ 
  int j;
  for (j = 0; j < elemento->m_numero_argumentos; ++j)
    {
      free (elemento->m_argumentos[j]);
    }
    elemento->m_numero_argumentos = 0;
  free (elemento->m_argumentos);
  elemento->m_argumentos = 0;
}

void
pipeline_set_argumentos (elemento_t *elemento,
				const char **argumentos,
				int numero_argumentos)
{
  pipeline_borrar_argumentos(elemento);
  elemento->m_numero_argumentos = numero_argumentos;
  elemento->m_argumentos = (char **) argumentos;  
}

int
pipeline_set_ruta (elemento_t *elemento, const char *ruta)
{
    if(ruta) {
  if(elemento->m_ruta) free (elemento->m_ruta);
  elemento->m_ruta = strdup (ruta);
  
  if (elemento->m_handler)
    {
      pipeline_free_library (elemento->m_handler);
    }
  elemento->m_handler = pipeline_load_library (ruta);

    if(elemento->m_handler) {
       funcion_get_modulo f = 
                 (funcion_get_modulo)pipeline_get_symbol(elemento->m_handler,
                        "get_modulo");
        elemento->m_modulo = f ? f() : 0;
    }
    else {
        pipeline_salida_error("Bibliotecas", pipeline_error());
        elemento->m_ruta = 0;
    }
    }
  return 0;
}
