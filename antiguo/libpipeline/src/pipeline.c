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

pipeline_t *pipeline_crear()
{
    pipeline_t *p = (pipeline_t *) malloc(sizeof(pipeline_t));
    p->m_numero = 0;
    p->m_elemento = 0;		//(elemento_t**)malloc(sizeof(elemento_t*));
    //p->m_elemento[0] = 0;
  //  p->m_corriendo = 0;
   // p->m_error = -1;
    return p;
}

int pipeline_poner_a_cero(pipeline_t *p, int i)
{
    elemento_t *elemento = p->m_elemento[i];
    elemento->m_handler = 0;
    
    elemento->m_funcion_iniciar = 0;
    elemento->m_funcion_cerrar = 0;
    elemento->m_funcion_ciclo = 0;
    //elemento->m_funcion_propiedades = 0;
    
    /*elemento->m_funcion_ciclo = 0;
    elemento->m_funcion_iniciar = 0;
    elemento->m_funcion_propiedades = 0;
    elemento->m_funcion_cerrar = 0;
    elemento->m_funcion_get_datos = 0;
    elemento->m_funcion_set_datos = 0;
    elemento->m_funcion_error = 0;*/
    return 0;
}

elemento_t *pipeline_nuevo(pipeline_t * pipeline, const char *nombre, const char *ruta,	/*const char **conexiones, int numero_conexiones, */
			   const char **argumentos, int numero_argumentos/*, int error*/)
{
    int id = pipeline->m_numero;
    pipeline->m_elemento = (elemento_t **) realloc
    	(pipeline->m_elemento, sizeof(elemento_t*) * (id + 1));
    pipeline->m_elemento[id] = (elemento_t*)malloc(sizeof(elemento_t));
    pipeline->m_elemento[id]->m_id = id;
    pipeline->m_elemento[id]->m_numero_conexiones = 0;
    pipeline->m_elemento[id]->m_numero_argumentos = 0;
    pipeline->m_elemento[id]->m_destino = 0;
//    pipeline->m_elemento[id]->m_error = error;
    pipeline->m_elemento[id]->m_destino = 0;
    //pipeline->m_elemento[id]->m_iniciado = 0;
    pipeline->m_elemento[id]->m_argumentos = (char **) argumentos;
    pipeline->m_elemento[id]->m_numero_argumentos = numero_argumentos;
    pipeline->m_elemento[id]->m_ruta = strdup(ruta);
    pipeline->m_elemento[id]->m_nombre = strdup(nombre);
    pipeline_poner_a_cero(pipeline, id);
    pipeline_cambiar_biblioteca(pipeline, id);
    pipeline->m_numero++;
    return pipeline->m_elemento[id];
}



char* pipeline_cerrar_biblioteca(pipeline_t * pipeline, int i)
//int pipeline_cerrar_biblioteca(elemento_t * elemento)
{
    char *dev = 0;
    elemento_t *elemento = pipeline->m_elemento[i];
    if (elemento->m_funcion_cerrar) {
	dev = elemento->m_funcion_cerrar();
    }
    pipeline_free_library(elemento->m_handler);
    pipeline_poner_a_cero(pipeline, i);
    return dev;
}

char* pipeline_cerrar_todas/*_bibliotecas*/(pipeline_t * pipeline)
{
    int id;
    char *dev = 0;
    int tam = 0;
    for (id = 0; id < pipeline->m_numero; ++id) {
	/*elemento_t **aux = pipeline->m_elemento;
	   while(aux) { */
	elemento_t *aux = pipeline->m_elemento[id];
	if (aux->m_handler != NULL) {
	    char *aux2 = pipeline_cerrar_biblioteca(pipeline, aux->m_id);
        tam += strlen(aux2);
        dev = (char *)realloc(dev, sizeof(char) * tam);
        strcat(dev, aux2);
	}
	//aux++;
    }
    //}
    return dev;
}

int pipeline_vaciar(pipeline_t ** pipeline)
{
    int id;
    for (id = 0; id < (*pipeline)->m_numero; ++id) {
    	elemento_t *aux = (*pipeline)->m_elemento[id];
    	if (aux->m_handler != 0) {
    	    pipeline_cerrar_biblioteca((*pipeline), aux->m_id);
    	}
    	free(aux->m_destino);
        free(aux->m_nombre);
        free(aux->m_ruta);
    	free(aux);
    }
    free((*pipeline)->m_elemento);
    //free(*pipeline->m_error);
    free((*pipeline));
    *pipeline = 0;

    return 0;
}

int pipeline_borrar(pipeline_t * pipeline, int id)
{
    int i;
    if (pipeline->m_elemento[id]->m_handler) {
	   pipeline_cerrar_biblioteca(pipeline, id);
    }

    for (i = 0; i < pipeline->m_numero; ++i) {
	   int j;
    	elemento_t *aux = pipeline->m_elemento[i];
    	for (j = 0; j < aux->m_numero_conexiones; ++j) {
   	        elemento_t *aux2 = aux->m_destino[j];
            if (aux2->m_id == id) {
        		pipeline_desconectar(pipeline, aux->m_id, id);
    	    }
	   }
    }

    elemento_t *aux3 = pipeline->m_elemento[id];

    free(aux3->m_nombre);
    free(aux3->m_ruta);   
    free(aux3->m_destino);       
    pipeline_borrar_argumentos(pipeline, aux3->m_id);
    free(aux3);

    for (i = id; i < pipeline->m_numero - 1; ++i) {;
	   pipeline->m_elemento[i] = pipeline->m_elemento[i + 1];
       pipeline->m_elemento[i]->m_id = i;
    }
        

    pipeline->m_elemento = (elemento_t **) realloc(pipeline->m_elemento,
						   sizeof(elemento_t*) * (pipeline->m_numero - 1));
               
    pipeline->m_numero--;                           
    return pipeline->m_numero;
}
int pipeline_guardar(const pipeline_t * pipeline, const char *ruta)
{
    xmlNodePtr pipe;
    xmlNodePtr modulo;
    xmlNodePtr c;
    xmlDocPtr doc;

    doc = xmlNewDoc(BAD_CAST "1.0");
    pipe = xmlNewNode(NULL, BAD_CAST "pipeline");
   /* xmlNewProp(pipe, "error_pipe", BAD_CAST pipeline->m_elemento[pipeline->m_error]->
              m_nombre);*/
    int i, j;
    for (i = 0; i < pipeline->m_numero; ++i) {
	elemento_t *aux = pipeline->m_elemento[i];
	modulo = xmlNewNode(NULL, BAD_CAST "modulo");
    xmlNewProp(modulo, "nombre", BAD_CAST aux->m_nombre);
    xmlNewProp(modulo, "ruta", BAD_CAST aux->m_ruta);
/*    char buffer;
    sprintf(&buffer, "%i", aux->m_error);
    xmlNewProp(modulo, "error", BAD_CAST &buffer);*/
	xmlAddChild(pipe, modulo);

	for (j = 0; j < pipeline->m_elemento[i]->m_numero_conexiones; ++j) {
	    elemento_t *aux2 = aux->m_destino[j];
	    c = xmlNewNode(NULL, BAD_CAST "conexion");
	    xmlAddChild(modulo, c);
	    xmlNodeSetContent(c, BAD_CAST aux2->m_nombre);
	}

	for (j = 0; j < aux->m_numero_argumentos; ++j) {
	    xmlNodePtr arg = xmlNewNode(NULL, BAD_CAST "argumento");
	    xmlAddChild(modulo, arg);
	    xmlNodeSetContent(arg, BAD_CAST aux->m_argumentos[j]);
	}
    }

    xmlDocSetRootElement(doc, pipe);
    FILE *f = fopen(ruta, "w");
    xmlDocDump(f, doc);
    fclose(f);
    return 0;
}

xmlChar **pipeline_parse_modulo(xmlDocPtr doc, xmlNodePtr cur,
				pipeline_t * pipeline,
				int *numero_conexiones, const char *nombre,
                const char*ruta/*, const char *error*/)
{
    xmlChar *key;
    xmlChar **argumentos;
    int numero_argumentos = 0;
    xmlChar **conexiones;
    xmlChar *arg = 0;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	/*if ((!xmlStrcmp(cur->name, (const xmlChar *) "nombre"))) {
	    if (nombre)
		free(nombre);
	    nombre = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!nombre) {
		nombre = strdup("");
	    }
	}
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "ruta"))) {
	    if (ruta)
		free(ruta);
	    ruta = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!ruta) {
		ruta = strdup("");
	    }
	}*/
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "argumento"))) {
	    arg = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!arg) {
		arg = strdup("");
	    }

	    argumentos = (xmlChar **) realloc(argumentos,
					      sizeof(xmlChar *) *
					      numero_argumentos + 1);
	    argumentos[numero_argumentos++] = arg;
	}

	if ((!xmlStrcmp(cur->name, (const xmlChar *) "conexion"))) {
	    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    if (!key) {
		key = strdup("");
	    }
	    conexiones = (xmlChar **) realloc(conexiones,
					      sizeof(xmlChar *) *
					      *numero_conexiones + 1);
	    conexiones[(*numero_conexiones)++] = key;
	}
	cur = cur->next;
    }
    pipeline_nuevo(pipeline, nombre, ruta, (const char **) argumentos,
		   numero_argumentos/*, atoi(error)*/);
    /*free(nombre);
    free(ruta);*/

    return conexiones;
}

pipeline_t *pipeline_cargar(const char *ruta)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar ***conexiones = 0;

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

    /*char *key;
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "error_pipe"))) {
	key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	if (!key) {
	    key = strdup("");
	}
    }*/


    int n = 0;
    int *numero_conexiones = 0;
    cur = cur->next;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "modulo"))) {
	    conexiones = (xmlChar ***) realloc(conexiones,
					       sizeof(xmlChar **) * n + 1);
	    numero_conexiones = (int *) realloc(conexiones,
						sizeof(int) * n + 1);

	    conexiones[n] =
		pipeline_parse_modulo(doc, cur, pipe,
				      &numero_conexiones[n], xmlGetProp(cur, "nombre"),
                        xmlGetProp(cur, "ruta")/*, xmlGetProp(cur, "error_pipe")*/);
	    n++;
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);

    int k;
    for (k = 0; k < pipe->m_numero; ++k) {
	for (i = 0; i < numero_conexiones[k]; ++i) {
	    pipeline_conectar(pipe, k, pipeline_get_elemento_por_nombre(pipe, conexiones[k][i])->m_id);
	}
    }

    for (k = 0; k < pipe->m_numero; ++k) {
	for (i = 0; i < numero_conexiones[k]; ++i) {
	    free(conexiones[k][i]);
	}
	free(conexiones[k]);
    }
    free(numero_conexiones);
    free(conexiones);


    //pipe->m_error = pipeline_get_elemento_por_nombre(pipe, char_error)->m_id;
    /*for (i = 0; i < pipe->m_numero; ++i) {
	if (!strcmp(pipe->m_elemento[i]->m_nombre, key)) {
	    pipe->m_error = i;
	}
    }*/

    return pipe;
}

int pipeline_conectar(pipeline_t * pipeline, int origen, int destino)
{
    int id = pipeline->m_elemento[origen]->m_numero_conexiones;
    pipeline->m_elemento[origen]->m_destino = (elemento_t **) realloc
    	(pipeline->m_elemento[origen]->m_destino, sizeof(elemento_t*) * (id + 1));
    pipeline->m_elemento[origen]->m_destino[id] =
    	pipeline->m_elemento[destino];
    pipeline->m_elemento[origen]->m_numero_conexiones++;
    return 0;
}


void pipeline_cambiar_biblioteca(pipeline_t * pipeline, int i)
{
    elemento_t *elemento = pipeline->m_elemento[i];
    if (elemento->m_handler) {
	pipeline_free_library(elemento->m_handler);
    }
    elemento->m_handler = pipeline_load_library(elemento->m_ruta);

    if (elemento->m_handler) {
	elemento->m_funcion_ciclo =
	    (funcion_ciclo_t) pipeline_get_function(elemento->m_handler,
						F_CICLO);
	elemento->m_funcion_iniciar =
	    (funcion_iniciar_t) pipeline_get_function(elemento->
							      m_handler,
							      F_INICIAR);
	/*elemento->m_funcion_propiedades =
	    (funcion_propiedades) pipeline_get_function(elemento->m_handler,
						F_PROPIEDADES);*/
	elemento->m_funcion_cerrar =
	    (funcion_cerrar_t) pipeline_get_function(elemento->m_handler,
						F_CERRAR);
	/*elemento->m_funcion_get_datos =
	    (funcion_void_a) pipeline_get_function(elemento->m_handler,
						   F_GET_DATOS);
	elemento->m_funcion_set_datos =
	    (funcion_int_param_void_a) pipeline_get_function(elemento->
							     m_handler,
							     F_SET_DATOS);
	elemento->m_funcion_error =
	    (funcion_char_a) pipeline_get_function(elemento->m_handler,
						   F_ERROR);*/
    } else {
	pipeline_poner_a_cero(pipeline, i);
    }
}
/*
int pipeline_error(const pipeline_t * pipeline,
		   int i,
		   const char *error)
{
    elemento_t *elemento = pipeline->m_elemento[i];
    if (error) {// && pipeline->m_error != -1) {
        int i;
        for(i = 0; i < pipeline->m_numero; ++i) {
	if (pipeline->m_elemento[i]->m_error &&
            pipeline->m_elemento[i]->m_funcion_set_datos) {
	    char buffer[128];
	    sprintf(buffer, "%s: %s", elemento->m_nombre,
		    (const char *) error);
	    pipeline->m_elemento[i]->
		m_funcion_set_datos(buffer);
	}
        }
    }
    return 0;
}
*/
char * pipeline_ciclo(const pipeline_t * pipeline)
{
    char *dev = 0;
    int tam = 0;
    void * arg1 = 0;
    void * arg2 = 0;
    int i;
    for (i = 0; i < pipeline->m_numero; ++i) {
        if(pipeline->m_elemento[i]->m_funcion_ciclo) {
            char *aux = pipeline->m_elemento[i]->m_funcion_ciclo(arg1, &arg2);
            tam += strlen(aux);
            dev = (char *)realloc(dev, sizeof(char) * tam);
            strcat(dev, aux);
            arg1 = arg2;
        }
    }
    return dev;
    /*int i;
    int j;
    for (i = 0; i < pipeline->m_numero; ++i) {
	elemento_t *aux = pipeline->m_elemento[i];
	if (aux->m_iniciado) {
	    if (aux->m_funcion_get_datos) {
		void *datos = aux->m_funcion_get_datos();
		pipeline_error(pipeline, aux->m_id,
			       aux->m_funcion_error());
		for (j = 0; j < aux->m_numero_conexiones; ++j) {
		    elemento_t *aux2 = aux->m_destino[j];
		    if (aux2->m_funcion_set_datos) {
			aux2->m_funcion_set_datos(datos);
			pipeline_error(pipeline, aux2->m_id,
				       aux2->m_funcion_error());
		    }
		}
	    }
	}
    }
    for (i = 0; i < pipeline->m_numero; ++i) {
	elemento_t *aux3 = pipeline->m_elemento[i];
	if (aux3->m_iniciado) {
	    if (aux3->m_funcion_ciclo) {
		aux3->m_funcion_ciclo();
		pipeline_error(pipeline, aux3->m_id,
			       aux3->m_funcion_error());
	    }
	}
    }
    return 0;*/
//    return pipeline->m_error;
}

char * pipeline_iniciar(const pipeline_t * pipeline,
		      int i)
{
    elemento_t *elemento = pipeline->m_elemento[i];
    
    return elemento->m_funcion_iniciar ? 
     elemento->m_funcion_iniciar(elemento->m_numero_argumentos, (const char **) elemento->
                    m_argumentos) : 0;
    //if (!elemento->m_iniciado) {
	//elemento->m_iniciado = 1;
	//if (elemento->m_funcion_iniciar) {
	    //pipeline_error(pipeline, i, elemento->m_funcion_error());

	  /*  return elemento->m_funcion_iniciar((const char **) elemento->
                    m_argumentos);
	//}
    }
    return 0;*/
}
/*
char * pipeline_parar(const pipeline_t * pipeline, int i)
{
    elemento_t *elemento = pipeline->m_elemento[i];
    elemento->m_iniciado = 0;
    if (elemento->m_funcion_cerrar) {
	return elemento->m_funcion_cerrar();
//	return pipeline_error(pipeline, i, elemento->m_funcion_error());
//	return 0;
    }
    return 0;
}*/
/*
char * pipeline_abrir_propiedades(const pipeline_t * pipeline, int i)
{
   * elemento_t *elemento = pipeline->m_elemento[i];
    if (elemento->m_funcion_propiedades) {
	return elemento->m_funcion_propiedades();
//                        pipeline_enviar_error(pipeline, elemento);
	//pipeline_error(pipeline, i, elemento->m_funcion_error());
//	return 0;
    } else {
	return 0;
    }
}*/


int pipeline_desconectar(pipeline_t * pipeline, int origen, int destino)
{
    int i;
    for (i = 0; i < pipeline->m_elemento[origen]->m_numero_conexiones; ++i) {
	elemento_t *aux = pipeline->m_elemento[origen]->m_destino[i];
	if (aux->m_id == destino) {
	    int j;
	    for (j = i;
		 j < pipeline->m_elemento[origen]->m_numero_conexiones;
		 ++j) {
		pipeline->m_elemento[origen]->m_destino[j] =
		    pipeline->m_elemento[origen]->m_destino[j + 1];
	    }
	    pipeline->m_elemento[origen]->m_numero_conexiones--;
	    pipeline->m_elemento[origen]->m_destino =
		(elemento_t **) realloc(pipeline->m_elemento[origen]->
					m_destino,
					sizeof(elemento_t *) *
					pipeline->m_elemento[origen]->
					m_numero_conexiones);
	}
    }
    return 0;
}

char* pipeline_iniciar_todas(pipeline_t * pipeline)
{
    int i;
    char *dev = 0;
    int tam = 0;
    //pipeline_iniciar(pipeline, pipeline->m_error);
    for (i = 0; i < pipeline->m_numero; ++i) {
	elemento_t *aux = pipeline->m_elemento[i];
	// while(*aux) {
	//for (i = 0; i < pipeline->m_numero; ++i) {
	//if (aux->m_id != pipeline->m_error) {
	    char *aux2 = pipeline_iniciar(pipeline, aux->m_id);
        tam += strlen(aux2);
        dev = (char *)realloc(dev, sizeof(char) * tam);
        strcat(dev, aux2);
	//}
	//aux++;
    }
    return dev;
}



void pipeline_borrar_argumentos(const pipeline_t * pipeline, int i)
{
// void pipeline_borrar_argumentos(elemento_t * elemento) {
    elemento_t *elemento = pipeline->m_elemento[i];
    int j;
    for (j = 0; j < elemento->m_numero_argumentos; ++j) {
	free(elemento->m_argumentos[j]);
    }
/*    char ** aux = elemento->m_argumentos;
    char ** aux2;
    while(aux) {
        aux2 = aux;
        free(aux);
        aux = ++aux2;
    }*/
    free(elemento->m_argumentos);
    elemento->m_argumentos = 0;
}

void pipeline_establecer_argumentos(const pipeline_t * pipeline, int i,
				    const char **argumentos,
				    int numero_argumentos)
{
//   void pipeline_establecer_argumentos(elemento_t * elemento, const char **argumentos) {
    elemento_t *elemento = pipeline->m_elemento[i];
    elemento->m_numero_argumentos = numero_argumentos;
    elemento->m_argumentos = (char **) argumentos;
}

elemento_t *pipeline_get_elemento_por_nombre(const pipeline_t *p, const char* n) {
    int i;
    for(i = 0; i < p->m_numero; ++i) {
        if(!strcmp(n, p->m_elemento[i]->m_nombre)) {
            return p->m_elemento[i];
        }
    }
    return 0;
}
int pipeline_set_nombre(const pipeline_t *pipeline, int i, const char *nombre) {
    free(pipeline->m_elemento[i]->m_nombre);
    pipeline->m_elemento[i]->m_nombre = strdup(nombre);
    return 0;
}

int pipeline_set_ruta(pipeline_t *pipeline, int i, const char *ruta) {
    elemento_t * elemento = pipeline->m_elemento[i];
    free(elemento->m_ruta);
    elemento->m_ruta = strdup(ruta);  
    pipeline_cambiar_biblioteca(pipeline, elemento->m_id);  
    return 0;
}


