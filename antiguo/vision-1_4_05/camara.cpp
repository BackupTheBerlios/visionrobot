#include "pipeline_sdk.h"
//#include "Captura.h" no borrar
#include "filtro_gestos_sdk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static modulo_t modulo_pipeline;
//static Captura *c; no borrar
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
  /*c->CogerFrame();
    int tam = c->GetFrame((BYTE**)&imagen.m_imagen);*/
  int tam = imagen.m_alto * imagen.m_ancho * 3; //Añadido esto
  imagen.m_bytes = (tam / imagen.m_alto) / imagen.m_ancho;


  *out = &imagen;
  sprintf(modulo_pipeline.m_error, "Ciclo: Información de la imagen de salida de la cámara: %p, %ix%i, de %i bytes", ((filtro_gestos_in_t *)(*out))->m_imagen, ((filtro_gestos_in_t *)(*out))->m_ancho, ((filtro_gestos_in_t *)(*out))->m_alto, ((filtro_gestos_in_t *)(*out))->m_bytes);
  return modulo_pipeline.m_error;
}

char* iniciar(int argc, const char **argv){
  if(argc < 2) return "faltan argumentos de tamaño de imagen";
  imagen.m_ancho = valor("ancho", argc, argv);
  imagen.m_alto = valor("alto", argc, argv);
  // Cambio, antes poníamos esto a cero, borrar
  imagen.m_imagen = (char*)malloc(sizeof(char)*imagen.m_ancho *  imagen.m_alto * 3);

  int i, j;
  for(i = 0; i < imagen.m_alto; ++i) {
    for(j = 0; j < imagen.m_ancho * 3; ++j) {
      imagen.m_imagen[(imagen.m_alto * i) + j] = 128;
    }
  }

  //  c = new Captura(); no borrar
  //c->Iniciar(valor("camara", argc, argv), 0, imagen.m_ancho, imagen.m_alto); no borrar
  sprintf(modulo_pipeline.m_error, "Iniciado. Creado contexto de cámara de %ix%i", imagen.m_ancho, imagen.m_alto);
  return modulo_pipeline.m_error;
}

char * cerrar() {
  //if(c) delete c; no borrar
  //c = 0; no borrar
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

