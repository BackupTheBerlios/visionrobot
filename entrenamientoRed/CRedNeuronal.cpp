//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop
#pragma package(smart_init)
#include "CRedNeuronal.h"

#define ABS(x)          (((x)>0.0) ? (x) : (-(x)))

//---------------------------------------------------------------------------


RedNeuronal::RedNeuronal(int en, int oc, int sa)
{

  numEntrada = en;
  numOculta = oc;
  numSalida = sa;

  correcto=0;

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
      int ra= random(100);
      if(ra==50)ra=51;
      double ran= (((ra*0.01)* 2)-1);
      pesosEntrada[(i*(numOculta+1))+j]=  ran;
      pesosEntradaAnterior[(i*(numOculta+1))+j]=0.0;
      }
       
  for(int i=0;i<=numOculta;i++)
    for(int j=0;j<=numSalida;j++){
       int ra= random(100);
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

int RedNeuronal::GetNumEntrada(){return numEntrada;}

int RedNeuronal::GetNumOculta(){return numOculta;}

int RedNeuronal::GetNumSalida(){return numSalida;}

int RedNeuronal::GetCorrecto(){return correcto;}

double* RedNeuronal::GetCapaEntrada(){return capaEntrada;}

double* RedNeuronal::GetCapaSalida(){return capaSalida;}

double* RedNeuronal::GetObjetivo(){return objetivo;}

double* RedNeuronal::GetPesosEntrada(){return pesosEntrada;}

double* RedNeuronal::GetPesosOculta(){return pesosOculta;}

void RedNeuronal::SetCorrecto(int c){correcto=c;}

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


//Calcula el error cometido en la capa de salida
void RedNeuronal::ErrorSalida(double* salidaD, double* objetivo, double* capaSalida, int nSalida, double* error)
{
  double Esum=0;
  for(int j=1;j<=nSalida;j++){
    double o=capaSalida[j];
    double t=objetivo[j];
    salidaD[j]=o*(1-o)*(t-o);
    Esum+=ABS(salidaD[j]);
  }
  *error = Esum;
}


//---------------------------------------------------------------------------


//Calcula el error en la capa oculta
void RedNeuronal::ErrorOculta(double* ocultaD, int nOculta, double* salidaD, int nSalida, double* pesosOculta, double* capaOculta, double* error)
{
  double Esum=0;
  for(int j=1;j<=nOculta;j++){
    double h=capaOculta[j];
    double sum=0;
    for(int k=1;k<=nSalida;k++)
      sum+=salidaD[k]*pesosOculta[(j*(nSalida+1))+k];
    ocultaD[j]=h*(1-h)*sum;
    Esum+=ABS(ocultaD[j]);
  }
  *error= Esum;
}


//---------------------------------------------------------------------------


void RedNeuronal::AjustarPesos(double* delta, int numCapa2, double *capa1, int numCapa1, double* pesosCapa1, double* pesosCapa1Ant, double tasa)
{
  double momento= 0.3; //Cuanto mas alto, antes aprende
  capa1[0]=1;
  for(int j=1;j<=numCapa2;j++)
    for(int k=0;k<=numCapa1;k++){
      double increm=((tasa*delta[j]*capa1[k])+(momento*pesosCapa1Ant[(k*(numCapa2+1))+j]));
      pesosCapa1[(k*(numCapa2+1))+j]+=increm;
      pesosCapa1Ant[(k*(numCapa2+1))+j]=increm;
    }
}


//---------------------------------------------------------------------------


void RedNeuronal::ActualizarObjetivo(int pos){
  for(int i=1;i<=numSalida;i++)
    if(pos==-1)objetivo[i]=0.1;
    else
      if(i==pos)objetivo[i]=0.9;
      else objetivo[i]=0.1;
}


//---------------------------------------------------------------------------


void RedNeuronal::Entrenamiento(double tasa, double *eo, double *eh)
{
  double out_err, hid_err;

  ComputarCapas();

  // Calcular el error en las unidades hidden y output
  ErrorSalida(salidaD, objetivo, capaSalida, numSalida, &out_err);
  ErrorOculta(ocultaD, numOculta, salidaD, numSalida, pesosOculta, capaOculta, &hid_err);

  *eo = out_err;
  *eh = hid_err;

  //Ajustar pesos de entrada y hidden
  AjustarPesos(salidaD, numSalida, capaOculta, numOculta, pesosOculta, pesosOcultaAnterior, tasa);
  AjustarPesos(ocultaD, numOculta, capaEntrada, numEntrada, pesosEntrada, pesosEntradaAnterior, tasa);
}


//---------------------------------------------------------------------------


void RedNeuronal::Guardar(AnsiString fileName)
{
   FILE *archivo;

   archivo= fopen(fileName.c_str(),"wb");
   fprintf(archivo,"%d %d %d",numEntrada,numOculta,numSalida);
   double numero;
   for(int i=0;i<=numEntrada;i++)
     for(int j=0;j<=numOculta;j++){
       numero= pesosEntrada[(i*(numOculta+1))+j];
       fwrite(&numero, sizeof(numero), 1, archivo); 
     }
   for(int i=0;i<=numOculta;i++)
     for(int j=0;j<=numSalida;j++){
       numero= pesosOculta[(i*(numSalida+1))+j];
       fwrite(&numero, sizeof(numero), 1, archivo);
     }
   fclose(archivo);
}


//---------------------------------------------------------------------------


RedNeuronal* Abrir(AnsiString fileName)
{
   FILE* archivo;
   int ne,no,ns;
   double numero;
   archivo= fopen(fileName.c_str(),"rb");
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
