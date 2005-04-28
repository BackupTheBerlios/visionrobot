#include "pipeline_sdk.h"
#include "Captura.h"
#include "filtro_gestos_sdk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Captura *c;
  filtro_gestos_in_imagen_t imagen;
  char buffer_error[128];
} camara_dato_t;

#define PUERTO "salida_imagen"

static char* camara_ciclo (modulo_t *modulo, const char *puerto, const void *value){
  camara_dato_t *dato = (camara_dato_t*)modulo->m_dato;
  dato->c->CogerFrame();
  int tam = dato->c->GetFrame((BYTE**)&dato->imagen.m_imagen);
  dato->imagen.m_bytes = (tam / dato->imagen.m_alto) / dato->imagen.m_ancho;
  return 0;
}

static char* camara_iniciar(modulo_t *modulo, GHashTable *argumentos) {
  camara_dato_t *dato = (camara_dato_t*)modulo->m_dato;
  if(g_hash_table_size(argumentos) < 3) return "faltan argumentos de tamanno de imagen";
  dato->imagen.m_ancho = atoi((const char *)g_hash_table_lookup(argumentos,"ancho"));
  dato->imagen.m_alto = atoi((const char *)g_hash_table_lookup(argumentos,"alto"));
  dato->imagen.m_imagen = 0;
  g_hash_table_insert(modulo->m_tabla, (void *)PUERTO, &dato->imagen);
  int cam = atoi((const char *)g_hash_table_lookup(argumentos, "camara"));
  dato->c = new Captura(); 
  dato->c->Iniciar(cam,
		   0, dato->imagen.m_ancho,
		   dato->imagen.m_alto);  
  //sprintf(dato->buffer_error, "Iniciado. Creado contexto de camara de %ix%i, camara %i", dato->imagen.m_ancho, dato->imagen.m_alto, cam);
  return 0;//dato->buffer_error;
}

static char *camara_cerrar(modulo_t *modulo) {
  camara_dato_t *dato = (camara_dato_t*)modulo->m_dato;
  if(dato->c) delete dato->c;
  dato->c = 0; 
  free(modulo->m_dato);
  free(modulo);
  return 0;//"cerrado";
}

modulo_t *get_modulo() {
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));
  modulo->m_nombre = "Camara";
  modulo->m_iniciar = camara_iniciar;
  modulo->m_cerrar = camara_cerrar;
  modulo->m_ciclo = camara_ciclo;
  modulo->m_dato = (camara_dato_t*)malloc(sizeof(camara_dato_t*));
  return modulo;
}
