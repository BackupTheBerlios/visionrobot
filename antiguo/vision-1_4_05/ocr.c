/*!
  \file   ocr.c
  \author Diego Sánchez
  \version 0.1
  \brief  Módulo de reconocimiento óptico de caracteres.
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

#include "ocr_code.h"
#include "pipeline_sdk.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUERTO_TEXTO "salida_texto"
#define PUERTO_IMAGEN "entrada_imagen"

typedef struct {
  char m_buffer_error[128];
  char m_out[128];
  pack_init_t *m_pack_init;
} ocr_dato_t;

static char *ocr_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  if(!strcmp(puerto, PUERTO_IMAGEN)) {
    ocr_dato_t *dato = (ocr_dato_t*)modulo->m_dato;
    pack_init_t *pack_init = dato->m_pack_init;
    filtro_gestos_in_imagen_t *ocr_in = (filtro_gestos_in_imagen_t *)value;

    if(pack_init && ocr_in) {
      sprintf(dato->m_out, "%s", ocr_semantic_match(ocr_in, pack_init));
      g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, &dato->m_out);
    }
    else {
      g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
    }
  }
  return 0;
}
static char *ocr_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
    ocr_dato_t * dato = (ocr_dato_t*)modulo->m_dato;
    char *archivo = g_hash_table_lookup(argumentos, "archivo");
    dato->m_pack_init =  ocr_init(archivo);
    g_hash_table_insert(modulo->m_tabla, PUERTO_TEXTO, 0);
    pack_init_t *init_pack = dato->m_pack_init;
    if (!init_pack) {
      return "No se ha cargado el archivo.";
    }    
    else {
	sprintf(dato->m_buffer_error, "archivo cargado = %s", archivo);
	return dato->m_buffer_error;
    }    
}
static char *ocr_cerrar(modulo_t *modulo)
{
  ocr_dato_t *dato = (ocr_dato_t*)modulo->m_dato;
  pack_init_t *init_pack = dato->m_pack_init;
  ocr_free_pack_init(init_pack);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Ocr";
  modulo->m_iniciar = ocr_iniciar;
  modulo->m_cerrar = ocr_cerrar;
  modulo->m_ciclo = ocr_ciclo;
  modulo->m_dato = (ocr_dato_t *)malloc(sizeof(ocr_dato_t));
  return modulo;
}


