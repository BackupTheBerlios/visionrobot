#ifndef _RED_NEURONAL_H_
#define _RED_NEURONAL_H_

typedef struct {
                int numEntrada;
                int numOculta;
                int numSalida;
                double* capaEntrada;
                double* capaOculta;
                double* capaSalida;
                double *ocultaD;
                double *salidaD;
                double *objetivo;
                double *pesosEntrada;
                double *pesosOculta;
                double *pesosEntradaAnterior;
                double *pesosOcultaAnterior;
} red_neuronal_t;

typedef enum {ORDEN,PARAM} tipo_foto_t;

red_neuronal_t *red_neuronal_crear(int en, int oc, int sa);
void red_neuronal_borrar(red_neuronal_t **);
void red_neuronal_computar_capas(red_neuronal_t *red);
red_neuronal_t* red_neuronal_abrir(const char* file);
char * red_neuronal_reconocer(red_neuronal_t * red,char* dibujo, int ancho, int alto, tipo_foto_t tipo);

#endif //_RED_NEURONAL_H_
