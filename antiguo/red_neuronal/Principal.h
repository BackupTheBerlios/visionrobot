//---------------------------------------------------------------------------

#ifndef PrincipalH
#define PrincipalH
#include "CRedNeuronal.h"

//---------------------------------------------------------------------------

class Principal
{
  private:
        RedNeuronal* red;
        void CargaInputImagen(BYTE* dibujo, int ancho, int alto, RedNeuronal* red);
  public:
        Principal();
        char * Reconocer(BYTE* dibujo, int ancho, int alto, tipoFoto tipo);
        void AbrirRed(char* fileName);
};

#endif
