//---------------------------------------------------------------------------

#ifndef CRedNeuronalH
#define CRedNeuronalH
//#include <fstream.h>
#include "CImagen.h"

//---------------------------------------------------------------------------

class RedNeuronal{

        private:
                int numEntrada;
                int numOculta;
                int numSalida;

                double* capaEntrada;
                double* capaOculta;
                double* capaSalida;
                double *ocultaD;
                double *salidaD;
                double * objetivo;
                double *pesosEntrada;
                double *pesosOculta;
                double *pesosEntradaAnterior;
                double *pesosOcultaAnterior;

                //Metodos privados
                void CalculaCapa(int num1, int num2, double* capa1, double* capa2, double* pesos);
        public:
                RedNeuronal(int en, int oc, int sa);
                ~RedNeuronal();
                double* GetCapaEntrada();
                double* GetCapaSalida();
                void SetPesosEntrada(double* in);
                void SetPesosOculta(double* in);
                void ComputarCapas();
};

RedNeuronal* Abrir(char* fileName);

#endif
