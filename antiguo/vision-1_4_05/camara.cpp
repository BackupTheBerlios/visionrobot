#include "pipeline_sdk.h"
#include "Captura.h"
#include "filtro_gestos_sdk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static modulo_t modulo_pipeline;
static Captura *c;
static filtro_gestos_in_t imagen;
static char buffer_error[128];

int valor(const char *tipo, int argc, const char **argv) {
  int i;
  for(i = 0; i < argc; i+=2) {
    if(!strcmp(tipo, argv[i])){
      return atoi(argv[i + 1]);
    }
  }
  return 0;
}


char* ciclo (const void *in, void **out){
  c->CogerFrame();
  int tam = c->GetFrame((BYTE**)&imagen.m_imagen);
  imagen.m_bytes = (tam / imagen.m_alto) / imagen.m_ancho;
  *out = &imagen;
  sprintf(buffer_error, "[%p, %ix%ix%i]", ((filtro_gestos_in_t *)(*out))->m_imagen, ((filtro_gestos_in_t *)(*out))->m_ancho, ((filtro_gestos_in_t *)(*out))->m_alto, ((filtro_gestos_in_t *)(*out))->m_bytes);
  return buffer_error;
}

char* iniciar(int argc, const char **argv){
  if(argc < 2) return "faltan argumentos de tama�o de imagen";
  imagen.m_ancho = valor("ancho", argc, argv);
  imagen.m_alto = valor("alto", argc, argv);
  imagen.m_imagen = 0;
  
  c = new Captura(); 
  c->Iniciar(valor("camara", argc, argv), 0, imagen.m_ancho, imagen.m_alto);  
  sprintf(buffer_error, "Iniciado. Creado contexto de c�mara de %ix%i", imagen.m_ancho, imagen.m_alto);
  return buffer_error;
}

char * cerrar() {
  if(c) delete c;
  c = 0; 
  return "cerrado";
}

modulo_t *get_modulo() {
    return &modulo_pipeline;
}

void __attribute__ ((constructor)) init(void) {
    modulo_pipeline.m_nombre = "C�mara";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
}
void __attribute__ ((destructor)) fini(void) {
}

