#include <windows.h>
#include "pipeline_sdk.h"
#include "Captura.h"
#include "filtro_gestos_sdk.h"
#include <string.h>

static modulo_t modulo_pipeline;
static Captura *c;
static filtro_gestos_in_t imagen;

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

 return 0;
}



char* iniciar(int argc, const char **argv){
  if(argc < 2) return "faltan argumentos de tamaño de imagen";
  imagen.m_alto = valor("ancho", argc, argv);
  imagen.m_ancho = valor("alto", argc, argv);
  c = new Captura();
  c->Iniciar(0, 0, 
  return "iniciado";
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
    modulo_pipeline.m_nombre = "Módulo de cámara";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
}
void __attribute__ ((destructor)) fini(void) {
}

