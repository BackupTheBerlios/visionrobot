#include <red_neuronal/red_neuronal.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

red_neuronal_t * red_neuronal_crear(int en, int oc, int sa)
{
    red_neuronal_t *red = (red_neuronal_t *)malloc(sizeof(red_neuronal_t));
  red->numEntrada = en;
  red->numOculta = oc;
  red->numSalida = sa;

  red->capaEntrada = (double*)malloc(sizeof(double)*red->numEntrada + 1); //new double[numEntrada+1];
  red->capaOculta = (double*)malloc(sizeof(double)*red->numOculta + 1); //new double[numOculta+1];
  red->capaSalida = (double*)malloc(sizeof(double)*red->numSalida + 1); //new double[numSalida+1];

  red->ocultaD = (double*)malloc(sizeof(double)*red->numOculta + 1); //new double[numOculta+1];
  red->salidaD = (double*)malloc(sizeof(double)*red->numSalida + 1); //new double[numSalida+1];
  red->objetivo = (double*)malloc(sizeof(double)*red->numSalida + 1); //new double[numSalida+1];

  red->pesosEntrada = (double*)malloc(sizeof(double)*((red->numEntrada+1)*(red->numOculta+1))); //new double[(numEntrada+1)*(numOculta+1)];
  red->pesosOculta = (double*)malloc(sizeof(double)*((red->numOculta+1)*(red->numSalida+1))); //new double[(numOculta+1)*(numSalida+1)];

  red->pesosEntradaAnterior = (double*)malloc(sizeof(double)*((red->numEntrada+1)*(red->numOculta+1))); //new double[(numEntrada+1)*(numOculta+1)];
  red->pesosOcultaAnterior = (double*)malloc(sizeof(double)*((red->numOculta+1)*(red->numSalida+1))); //new double[(numOculta+1)*(numSalida+1)];

    int i, j;
  for(i=0;i<=red->numEntrada;i++)
    for(j=0;j<=red->numOculta;j++){
      int ra= rand() * 100 / RAND_MAX;
      if(ra==50)ra=51;
      double ran= (((ra*0.01)* 2)-1);
      red->pesosEntrada[(i*(red->numOculta+1))+j]=  ran;
      red->pesosEntradaAnterior[(i*(red->numOculta+1))+j]=0.0;
      }
       
  for(i=0;i<=red->numOculta;i++)
    for(j=0;j<=red->numSalida;j++){
       int ra= rand() * 100 / RAND_MAX;
       if(ra==50)ra=51;
       double ran= (((ra*0.01)* 2)-1);
       red->pesosOculta[(i*(red->numSalida+1))+j]= ran;
       red->pesosOcultaAnterior[(i*(red->numSalida+1))+j]=0.0;
    }
    return red;
}


//---------------------------------------------------------------------------


void red_neuronal_borrar(red_neuronal_t **red)
{
    if(red && (*red)) {
  free ((*red)->capaEntrada);
  free ((*red)->capaOculta);
  free ((*red)->capaSalida);
  free ((*red)->ocultaD);
  free ((*red)->salidaD);
  free ((*red)->objetivo);

  free ((*red)->pesosEntrada);
  free ((*red)->pesosEntradaAnterior);

  free ((*red)->pesosEntrada);
  free ((*red)->pesosEntradaAnterior);

  free ((*red)->pesosOculta);
  free ((*red)->pesosOcultaAnterior);

  free ((*red)->pesosOculta);
  free ((*red)->pesosOcultaAnterior);
  *red = 0;
    }
}


//---------------------------------------------------------------------------
//Calcula el valor de la unidades de la capa2

void red_neuronal_calcular_capa(/*red_neuronal_t * red, */int num1, int num2, double* capa1, double* capa2, double* pesos)
{
  double sum;
  capa1[0]=1;
  int j;
  for (j=1;j<=num2;j++) {
    sum=0;
    int k;
    for(k=0;k<=num1;k++)
      sum+=pesos[(k*(num2+1))+j]*capa1[k];
    capa2[j] = (1/(1+exp(-sum)));
  }
}


//---------------------------------------------------------------------------
//Calcula el valor de la capa oculta y la de salida

void red_neuronal_computar_capas(red_neuronal_t * red)
{
  red_neuronal_calcular_capa(red->numEntrada, red->numOculta, red->capaEntrada, red->capaOculta, red->pesosEntrada);
  red_neuronal_calcular_capa(red->numOculta, red->numSalida, red->capaOculta, red->capaSalida,red->pesosOculta);
}


//---------------------------------------------------------------------------

red_neuronal_t* red_neuronal_abrir(const char* file)
{
   FILE* archivo;
   int ne,no,ns;
   double numero;
   archivo= fopen(file,"rb");
   if(!archivo) {return 0;}
   fscanf(archivo,"%d %d %d",&ne,&no,&ns);
   red_neuronal_t * redA= red_neuronal_crear(ne,no,ns);
   double *pesosEntrada2 = (double*)malloc(sizeof(double) * ((ne+1)*(no+1)));//new double[(ne+1)*(no+1)];
   double *pesosOculta2 = (double*)malloc(sizeof(double) * ((no+1)*(ns+1)));//new double[(no+1)*(ns+1)];
   int i;
   for(i=0;i<=ne;i++) {
   int j;
     for(j=0;j<=no;j++){
       fread(&numero, sizeof(numero), 1, archivo);
       pesosEntrada2[(i*(no+1))+j]=numero;
     }
   }
   for(i=0;i<=no;i++){
    int j;
     for(j=0;j<=ns;j++){
       fread(&numero, sizeof(numero), 1, archivo);
       pesosOculta2[(i*(ns+1))+j]=numero;
     }
   }
   redA->pesosEntrada = pesosEntrada2;
    redA->pesosOculta = pesosOculta2;
   fclose(archivo);
   return redA;
}

void red_neuronal_cargar_input_imagen(red_neuronal_t * red, char * dibujo, int ancho, int alto)
{
  int k=0;
  if(!red) return;
  double* entrada = red->capaEntrada;
  int i;
  for(i=0;i<alto;i++){
    int j;
    for (j=0;j<ancho*4;j++){
      entrada[k]= (double)(dibujo[(i*ancho*4)+j]/255);
      k++; j+=3;
    }
  }
}

char* red_neuronal_reconocer(red_neuronal_t * red, char* dibujo, int ancho, int alto, tipo_foto_t tipo)
{
  if(!red) return "red no cargada";
//  Imagen* imagen= new Imagen(dibujo,ancho,alto);
  //Sleep(100);
  if(!dibujo) return "no hay dibujo";
  red_neuronal_cargar_input_imagen(red, dibujo,ancho,alto);
  red_neuronal_computar_capas(red);
  switch(tipo){
        case ORDEN:
                if(red->capaSalida[1]>0.5) return "Parar";
                else if(red->capaSalida[2]>0.5) return "Girar";
                else if(red->capaSalida[3]>0.5) return "GirarNeg";
                else if(red->capaSalida[4]>0.5) return "Avanzar";
                else return "No_Gesto";
        break;
        case PARAM:
                if(red->capaSalida[1]>0.5) return "Nula";
                else if(red->capaSalida[2]>0.5) return "Baja";
                else if(red->capaSalida[3]>0.5) return "Media";
                else if(red->capaSalida[4]>0.5) return "Alta";
                else return "Maxima";
        break;
  }
  return "";
}
