#include "red_neuronal.h"
#include <red_neuronal_sdk.h>

#include <pipeline_sdk.h>
#include <stdio.h>
#include <string.h>

static modulo_t modulo_pipeline;
static red_neuronal_t * red = 0;

char* ciclo (const void *in, void **out){
 red_neuronal_in_t * red_in = (red_neuronal_in_t *)in;
 if(red) {
 *out = Reconocer(red, red_in->m_orden,red_in->m_ancho,red_in->m_alto,ORDEN);
 }
 else {
    *out = 0;
 }
 return 0;
}


char* iniciar(int argc, const char **argv){ 
    if(argc < 2) {return "faltan el nombre de archivo para cargar";}
    if(!strcmp(argv[0], "archivo")) {
        red = Abrir(argv[1]); 
    }
    if(!red) return "no se ha cargado el archivo";
  return "iniciado";
}

char * cerrar() {
    RedNeuronalBorrar(&red);
  return "cerrado";
}

modulo_t *get_modulo() {
    return &modulo_pipeline;
}

void __attribute__ ((constructor)) init(void) {
    modulo_pipeline.m_nombre = "Módulo de red neuronal";
    modulo_pipeline.m_iniciar = iniciar;
    modulo_pipeline.m_cerrar = cerrar;
    modulo_pipeline.m_ciclo = ciclo;
}
void __attribute__ ((destructor)) fini(void) {
}
