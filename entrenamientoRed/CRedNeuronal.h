//---------------------------------------------------------------------------

#include "CListaImagenes.h"
#ifndef CRedNeuronalH
#define CRedNeuronalH
#endif

//---------------------------------------------------------------------------

class RedNeuronal{

        private:
                int numEntrada;
                int numOculta;
                int numSalida;
                int correcto;

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
                void ErrorSalida(double* salidaD, double* objetivo, double* capaSalida, int nSalida, double* error);
                void ErrorOculta(double* ocultaD, int nOculta, double* salidaD, int nSalida, double* pesosOculta, double* capaOculta, double* error);
                void AjustarPesos(double* delta, int numCapa2, double *capa1, int numCapa1, double* pesosCapa1, double* pesosCapa1Ant, double tasa);

        public:
                RedNeuronal(int en, int oc, int sa);
                ~RedNeuronal();
                int GetNumEntrada();
                int GetNumOculta();
                int GetNumSalida();
                int GetCorrecto();
                double* GetCapaEntrada();
                double* GetCapaSalida();
                double* GetObjetivo();
                double* GetPesosEntrada();
                double* GetPesosOculta();
                void SetCorrecto(int c);
                void SetPesosEntrada(double* in);
                void SetPesosOculta(double* in);
                void ActualizarObjetivo(int pos);
                void ComputarCapas();
                void Entrenamiento(double tasa, double *eo, double *eh);
                void Guardar(AnsiString fileName);
};

RedNeuronal* Abrir(AnsiString fileName);
