#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>

void imprimir_arbol(elemento_t *e, int nivel) {
  int i;
  for(i = 0; i < nivel; ++i) {
    printf("*");
  }
  printf("%s\n", e->m_modulo->m_nombre);
  for(i = 0; i < e->m_numero_conexiones; ++i) {
    imprimir_arbol(e->m_destino[i], nivel + 1);
  }
}

int main(int argc, char **argv)
{
  if (argc < 1) {
    printf("Faltan argumentos.\n");
    return -1;
  }
  
  else {

    printf("- COMIENZO DE LA EJECUCIÓN ------------------------------\n");
    elemento_t * e = pipeline_cargar(argv[1]);
    imprimir_arbol(e, 0);
    pipeline_iniciar_todo(e);	
    int c;
    while ((c = getchar())) {      
      if(c == 's' || c == 'S') {
	break;
      }
      pipeline_ciclo(e, 0);
    }
    printf("- FIN DE LA EJECUCIÓN -----------------------------------\n");
    pipeline_cerrar_todo(e);
    pipeline_borrar(e);
    return 0;
  }
}


