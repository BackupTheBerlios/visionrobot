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

red_neuronal_t *RedNeuronalCrear(int en, int oc, int sa);
void RedNeuronalBorrar(red_neuronal_t **);
void ComputarCapas(red_neuronal_t *red);
red_neuronal_t* Abrir(const char* fileName);
char * Reconocer(red_neuronal_t * red,char* dibujo, int ancho, int alto, tipo_foto_t tipo);

#endif //_RED_NEURONAL_H_
