//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "FuncionesAdicionales.h"

//---------------------------------------------------------------------------


void Carga_objetivo(Imagen* imagen, RedNeuronal* red, tipoFoto tipo)
{
  char gesto[60], numero[60], formato[60], salida[60], salida2[60], tipos[60];
  int escala;
  gesto[0] = formato[0] = numero[0] = salida[0] = salida2[0]= tipos[60]= '\0';

  AnsiString nombreArchivo= imagen->GetNombre();

  //ejemplo: abierta_1.bmp    contraejemplo: nada_2.bmp
  sscanf(nombreArchivo.c_str(), "%[^/]/%[^/]/%[^_]_%[^_].%[^_]",
    salida,salida2,tipos,gesto,numero,formato);

  switch(tipo){
        case ORDEN:
                if((AnsiString)gesto=="parada") red->ActualizarObjetivo(1);
                else if((AnsiString)gesto=="angulo") red->ActualizarObjetivo(2);
                else if((AnsiString)gesto=="anguloNeg") red->ActualizarObjetivo(3);
                else if((AnsiString)gesto=="avanza") red->ActualizarObjetivo(4);
                else red->ActualizarObjetivo(5);
        break;
        case PARAM:
                if((AnsiString)gesto=="nula") red->ActualizarObjetivo(1);
                else if((AnsiString)gesto=="baja") red->ActualizarObjetivo(2);
                else if((AnsiString)gesto=="media") red->ActualizarObjetivo(3);
                else if((AnsiString)gesto=="alta") red->ActualizarObjetivo(4);
                else if((AnsiString)gesto=="maxima") red->ActualizarObjetivo(5);
                else red->ActualizarObjetivo(-1);
        break;
  }
}

//---------------------------------------------------------------------------

void CargaInputImagen(Imagen* imagen, RedNeuronal* red)
{
  double *entrada;
  Byte* linea;

  int filas= imagen->GetDibujo()->Bitmap->Height;
  int columnas= imagen->GetDibujo()->Bitmap->Width*4;

  entrada = red->GetCapaEntrada();
  int k=0;
  for(int i=0;i<filas;i++){
    linea= (Byte*)imagen->GetDibujo()->Bitmap->ScanLine[i];
    for (int j=0;j<columnas;j++){
      entrada[k]= (double)(linea[j]/255);
      k++; j+=3;
    }
  }
}

//---------------------------------------------------------------------------
