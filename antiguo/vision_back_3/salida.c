/*! \file salida.c
    \brief Muestra una salida en forma de texto
    \author Carlos Le�n
    \version 0.1
*/

#include "pipeline_sdk.h"
#include <stdio.h>
#include <string.h>

static modulo_t modulo_pipeline;

char* ciclo (const void *in, void **out){
 if(in) printf("%s\n", (char *)in);
 return 0;
}


char* iniciar(int argc, const char **argv){
    int i;
    for(i = 0; i < argc; i+=2) {
        if(!strcmp("texto", argv[i]))
            printf("%s\n", argv[i + 1]);
    }
  return "iniciado";
}

char * cerrar() {
  return "cerrado";
}

modulo_t *get_modulo() {
    return &modulo_pipeline;
}

void __attribute__ ((constructor)) init(void) {
    modulo_pipeline.m_nombre = "Módulo de salida de texto";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
}
void __attribute__ ((destructor)) fini(void) {
}

