#include "pipeline_sdk.h"
#include "Captura.h"
#include "filtro_gestos_sdk.h"
#include "ventana_imagen_sdk.h"
#include "camara_sdk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static Captura *c;
static filtro_gestos_in_t imagen;
static ventana_imagen_in_t ventana;
static char buffer_error[128];


static char* camara_ciclo (modulo_t *modulo, char tipo, GHashTable *lista){
  c->CogerFrame();
  int tam = c->GetFrame((BYTE**)&imagen.m_dato.imagen.m_imagen);
  ventana.m_imagen = imagen.m_dato.imagen.m_imagen;
  imagen.m_dato.imagen.m_bytes = (tam / imagen.m_dato.imagen.m_alto) / imagen.m_dato.imagen.m_ancho;
  ventana.m_bytes = imagen.m_dato.imagen.m_bytes;
  sprintf(buffer_error, "[%p, %ix%ix%i] - [%p, %ix%ix%i]", imagen.m_dato.imagen.m_imagen, imagen.m_dato.imagen.m_ancho,
	  imagen.m_dato.imagen.m_alto, imagen.m_dato.imagen.m_bytes, ventana.m_imagen, ventana.m_ancho, ventana.m_alto, ventana.m_bytes);
  return buffer_error;
}

static char* camara_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  if(g_hash_table_size(argumentos) < 3) return "faltan argumentos de tamanno de imagen";
  imagen.m_dato.imagen.m_ancho = atoi((const char *)g_hash_table_lookup(argumentos,"ancho"));
  imagen.m_dato.imagen.m_alto = atoi((const char *)g_hash_table_lookup(argumentos,"alto"));
  imagen.m_dato.imagen.m_imagen = 0;
  imagen.m_tipo = PIPELINE_FILTRO_GESTOS_IMAGEN;
  ventana.m_imagen = imagen.m_dato.imagen.m_imagen;
  ventana.m_alto = imagen.m_dato.imagen.m_alto;
  ventana.m_ancho = imagen.m_dato.imagen.m_ancho;
  ventana.m_bytes = imagen.m_dato.imagen.m_bytes;
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_VENTANA_IMAGEN), &ventana);
  g_hash_table_insert(modulo->m_tabla, GINT_TO_POINTER(PIPELINE_FILTRO_GESTOS), &imagen);
  int cam = atoi((const char *)g_hash_table_lookup(argumentos, "camara"));
  c = new Captura(); 
  c->Iniciar(cam,
	     0, imagen.m_dato.imagen.m_ancho,
	     imagen.m_dato.imagen.m_alto);  
  sprintf(buffer_error, "Iniciado. Creado contexto de camara de %ix%i, camara %i", imagen.m_dato.imagen.m_ancho, imagen.m_dato.imagen.m_alto, cam);
  return buffer_error;
}

static char *camara_cerrar(modulo_t *modulo) {
  if(c) delete c;
  c = 0; 
  free(modulo);
  return "cerrado";
}

modulo_t *get_modulo() {
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_tipo = PIPELINE_CAMARA;
  modulo->m_nombre = "Camara";
  modulo->m_iniciar = camara_iniciar;
  modulo->m_cerrar = camara_cerrar;
  modulo->m_ciclo = camara_ciclo;
  return modulo;
}


