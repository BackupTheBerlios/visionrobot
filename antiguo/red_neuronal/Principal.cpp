//---------------------------------------------------------------------------

//#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop
#pragma package(smart_init)
#include "Principal.h"

//---------------------------------------------------------------------------

Principal::Principal(){
  red= NULL;
}

//---------------------------------------------------------------------------

void Principal::AbrirRed(char* fileName)
{
  red= Abrir(fileName);
}

//---------------------------------------------------------------------------

char* Principal::Reconocer(BYTE* dibujo, int ancho, int alto, tipoFoto tipo)
{
  if(!red) return "";
  Imagen* imagen= new Imagen(dibujo,ancho,alto);
  Sleep(100);
  CargaInputImagen(dibujo,ancho,alto,red);
  red->ComputarCapas();
  switch(tipo){
        case ORDEN:
                if(red->GetCapaSalida()[1]>0.5) return "Parar";
                else if(red->GetCapaSalida()[2]>0.5) return "Girar";
                else if(red->GetCapaSalida()[3]>0.5) return "GirarNeg";
                else if(red->GetCapaSalida()[4]>0.5) return "Avanzar";
                else return "No_Gesto";
        break;
        case PARAM:
                if(red->GetCapaSalida()[1]>0.5) return "Nula";
                else if(red->GetCapaSalida()[2]>0.5) return "Baja";
                else if(red->GetCapaSalida()[3]>0.5) return "Media";
                else if(red->GetCapaSalida()[4]>0.5) return "Alta";
                else return "Maxima";
        break;
  }
  return "";
}

//---------------------------------------------------------------------------

void Principal::CargaInputImagen(BYTE* dibujo, int ancho, int alto, RedNeuronal* red)
{
  int k=0;
  if(!red) return;
  double* entrada = red->GetCapaEntrada();
  for(int i=0;i<alto;i++){
    for (int j=0;j<ancho*4;j++){
      entrada[k]= (double)(dibujo[(i*ancho*4)+j]/255);
      k++; j+=3;
    }
  }
}

//---------------------------------------------------------------------------
