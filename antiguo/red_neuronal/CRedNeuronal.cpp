//---------------------------------------------------------------------------

#pragma hdrstop
#pragma package(smart_init)
#include "CRedNeuronal.h"

#include <stdlib.h>
#include <stdio.h>

#define ABS(x)          (((x)>0.0) ? (x) : (-(x)))

//---------------------------------------------------------------------------

RedNeuronal::RedNeuronal(int en, int oc, int sa)
{
  numEntrada = en;
  numOculta = oc;
  numSalida = sa;

  capaEntrada = new double[numEntrada+1];
  capaOculta = new double[numOculta+1];
  capaSalida = new double[numSalida+1];

  ocultaD = new double[numOculta+1];
  salidaD = new double[numSalida+1];
  objetivo = new double[numSalida+1];

  pesosEntrada = new double[(numEntrada+1)*(numOculta+1)];
  pesosOculta = new double[(numOculta+1)*(numSalida+1)];

  pesosEntradaAnterior = new double[(numEntrada+1)*(numOculta+1)];
  pesosOcultaAnterior = new double[(numOculta+1)*(numSalida+1)];


  for(int i=0;i<=numEntrada;i++)
    for(int j=0;j<=numOculta;j++){
      int ra= rand() * 100 / RAND_MAX;
      if(ra==50)ra=51;
      double ran= (((ra*0.01)* 2)-1);
      pesosEntrada[(i*(numOculta+1))+j]=  ran;
      pesosEntradaAnterior[(i*(numOculta+1))+j]=0.0;
      }
       
  for(int i=0;i<=numOculta;i++)
    for(int j=0;j<=numSalida;j++){
       int ra= rand() * 100 / RAND_MAX;
       if(ra==50)ra=51;
       double ran= (((ra*0.01)* 2)-1);
       pesosOculta[(i*(numSalida+1))+j]= ran;
       pesosOcultaAnterior[(i*(numSalida+1))+j]=0.0;
    }
}


//---------------------------------------------------------------------------


RedNeuronal::~RedNeuronal()
{
  delete capaEntrada;
  delete capaOculta;
  delete capaSalida;
  delete ocultaD;
  delete salidaD;
  delete objetivo;

  delete pesosEntrada;
  delete pesosEntradaAnterior;

  delete pesosEntrada;
  delete pesosEntradaAnterior;

  delete pesosOculta;
  delete pesosOcultaAnterior;

  delete pesosOculta;
  delete pesosOcultaAnterior;
}

//---------------------------------------------------------------------------

double* RedNeuronal::GetCapaEntrada(){return capaEntrada;}

double* RedNeuronal::GetCapaSalida(){return capaSalida;}

void RedNeuronal::SetPesosEntrada(double* in){pesosEntrada=in;}

void RedNeuronal::SetPesosOculta(double* in){pesosOculta=in;}

//---------------------------------------------------------------------------
//Calcula el valor de la unidades de la capa2

void RedNeuronal::CalculaCapa(int num1, int num2, double* capa1, double* capa2, double* pesos)
{
  double sum;
  capa1[0]=1;
  for (int j=1;j<=num2;j++) {
    sum=0;
    for(int k=0;k<=num1;k++)
      sum+=pesos[(k*(num2+1))+j]*capa1[k];
    capa2[j] = (1/(1+exp(-sum)));
  }
}


//---------------------------------------------------------------------------
//Calcula el valor de la capa oculta y la de salida

void RedNeuronal::ComputarCapas()
{
  CalculaCapa(numEntrada, numOculta, capaEntrada, capaOculta, pesosEntrada);
  CalculaCapa(numOculta, numSalida, capaOculta, capaSalida,pesosOculta);
}


//---------------------------------------------------------------------------

RedNeuronal* Abrir(char* fileName)
{
   FILE* archivo;
   int ne,no,ns;
   double numero;
   archivo= fopen(fileName,"rb");
   fscanf(archivo,"%d %d %d",&ne,&no,&ns);
   RedNeuronal * redA= new RedNeuronal(ne,no,ns);
   double *pesosEntrada2 = new double[(ne+1)*(no+1)];
   double *pesosOculta2 = new double[(no+1)*(ns+1)];
   for(int i=0;i<=ne;i++)
     for(int j=0;j<=no;j++){
       fread(&numero, sizeof(numero), 1, archivo);
       pesosEntrada2[(i*(no+1))+j]=numero;
     }
   for(int i=0;i<=no;i++)
     for(int j=0;j<=ns;j++){
       fread(&numero, sizeof(numero), 1, archivo);
       pesosOculta2[(i*(ns+1))+j]=numero;
     }
   redA->SetPesosEntrada(pesosEntrada2);
   redA->SetPesosOculta(pesosOculta2);
   fclose(archivo);
   return redA;
}

//---------------------------------------------------------------------------
