//---------------------------------------------------------------------------
#include "CRedNeuronal.h"
#include <math.h>
#ifndef FuncionesAdicionalesH
#define FuncionesAdicionalesH
#endif
//---------------------------------------------------------------------------


#define ALTA 0.9
#define BAJA 0.1


void Carga_objetivo(Imagen* imagen, RedNeuronal* red, tipoFoto tipo);
void CargaInputImagen(Imagen* imagen, RedNeuronal* red);
void GuardarOculta(RedNeuronal* red, AnsiString filename);


