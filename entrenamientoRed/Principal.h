//---------------------------------------------------------------------------

#include "FuncionesAdicionales.h"
#ifndef PrincipalH
#define PrincipalH
#endif

//---------------------------------------------------------------------------


class Principal
{
  private:
        RedNeuronal* red;
        double* elitePI;
        double* elitePH;
        int correcto;

  public:
        Principal();
        Principal(AnsiString nE, AnsiString nV, AnsiString nP,
                  int v, tipoFoto t, TMemo* mem, TProgressBar *Prog, double tasa);
        void ReconocimientoImagenes(ListaImagenes* listaEntrena, ListaImagenes* listaValid,
                                    ListaImagenes* listaPrueba, int vueltas, tipoFoto tipo,
                                    TMemo* mem, TProgressBar* Prog, double tasa);
        void CalculaListaImagenes(ListaImagenes* lista,tipoFoto tipo, TMemo* mem, int entre);
        bool EvaluacionImagen(double * err);
        AnsiString Reconocer(AnsiString filename, tipoFoto t);
        void GuardarRed(AnsiString fileName);
        void AbrirRed(AnsiString fileName);
};
