#include <pipeline.h>
#include <stdio.h>

int main(int argc, char **argv){
    if(argc < 1) {
        printf("Faltan argumentos.\n");    
        return -1;
    }
    else {
        pipeline_t *p = pipeline_cargar(argv[1]);
        printf("%s\n", pipeline_iniciar_todas(p));
        while(1) {
            printf("%s\n", pipeline_ciclo(p));
        }
        printf("%s\n", pipeline_cerrar_todas (p));
        pipeline_vaciar(&p);
        return 0;
    }
}
