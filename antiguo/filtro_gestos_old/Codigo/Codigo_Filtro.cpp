//---------------------------------------------------------------------------

#pragma hdrstop

#include "Codigo_Filtro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TPicture* Filtro(TPicture* dibujo, TPicture* dibujo2)
{
   Byte* linea1=NULL;
   Byte* linea2=NULL;
   Byte* linea3=NULL;
   Byte* linea4=NULL;
   Byte* linea5=NULL;
   Byte* lineaYap=NULL;
   int cont=0; int acX=0; int acY=0;

   for (int y = 0; y < dibujo->Bitmap->Height; y++){
    if(y%4==1){
     if(y-2>=0)linea1= (Byte*)dibujo->Bitmap->ScanLine[y-2]; else linea1=NULL;
     if(y-1>=0)linea2= (Byte*)dibujo->Bitmap->ScanLine[y-1]; else linea2=NULL;
     linea3= (Byte*)dibujo->Bitmap->ScanLine[y];
     lineaYap= (Byte*)dibujo2->Bitmap->ScanLine[floor(y/4)];
     if(y+1<dibujo->Bitmap->Height)linea4= (Byte*)dibujo->Bitmap->ScanLine[y+1]; else linea4=NULL;
     if(y+2<dibujo->Bitmap->Height)linea5= (Byte*)dibujo->Bitmap->ScanLine[y+2]; else linea5=NULL;

     for (int x = 0; x < dibujo->Bitmap->Width*3; x++){
      if(((x/3)%4)==1){
        int rojo=0; int verde=0; int azul=0;
        if(linea1!=NULL){
          if(x>=6){rojo+=linea1[x-6]; verde+=linea1[x-5]; azul+=linea1[x-4];}
          if(x>=3){rojo+=linea1[x-3]; verde+=linea1[x-2]; azul+=linea1[x-1];}
          if(x+2<dibujo->Bitmap->Width*3) {rojo+=linea1[x]; verde+=linea1[x+1]; azul+=linea1[x+2];}
          if(x+5<dibujo->Bitmap->Width*3) {rojo+=linea1[x+3]; verde+=linea1[x+4]; azul+=linea1[x+5];}
          if(x+8<dibujo->Bitmap->Width*3) {rojo+=linea1[x+6]; verde+=linea1[x+7]; azul+=linea1[x+8];}
        }
        if(linea2!=NULL){
          if(x>=6){rojo+=linea2[x-6]; verde+=linea2[x-5]; azul+=linea2[x-4];}
          if(x>=3){rojo+=linea2[x-3]; verde+=linea2[x-2]; azul+=linea2[x-1];}
          if(x+2<dibujo->Bitmap->Width*3) {rojo+=linea2[x]; verde+=linea2[x+1]; azul+=linea2[x+2];}
          if(x+5<dibujo->Bitmap->Width*3) {rojo+=linea2[x+3]; verde+=linea2[x+4]; azul+=linea2[x+5];}
          if(x+8<dibujo->Bitmap->Width*3) {rojo+=linea2[x+6]; verde+=linea2[x+7]; azul+=linea2[x+8];}
        }
        if(linea3!=NULL){
          if(x>=6){rojo+=linea3[x-6]; verde+=linea3[x-5]; azul+=linea3[x-4];}
          if(x>=3){rojo+=linea3[x-3]; verde+=linea3[x-2]; azul+=linea3[x-1];}
          if(x+2<dibujo->Bitmap->Width*3) {rojo+=linea3[x]; verde+=linea3[x+1]; azul+=linea3[x+2];}
          if(x+5<dibujo->Bitmap->Width*3) {rojo+=linea3[x+3]; verde+=linea3[x+4]; azul+=linea3[x+5];}
          if(x+8<dibujo->Bitmap->Width*3) {rojo+=linea3[x+6]; verde+=linea3[x+7]; azul+=linea3[x+8];}
        }
        if(linea4!=NULL){
          if(x>=6){rojo+=linea4[x-6]; verde+=linea4[x-5]; azul+=linea4[x-4];}
          if(x>=3){rojo+=linea4[x-3]; verde+=linea4[x-2]; azul+=linea4[x-1];}
          if(x+2<dibujo->Bitmap->Width*3) {rojo+=linea4[x]; verde+=linea4[x+1]; azul+=linea4[x+2];}
          if(x+5<dibujo->Bitmap->Width*3) {rojo+=linea4[x+3]; verde+=linea4[x+4]; azul+=linea4[x+5];}
          if(x+8<dibujo->Bitmap->Width*3) {rojo+=linea4[x+6]; verde+=linea4[x+7]; azul+=linea4[x+8];}
        }
        if(linea5!=NULL){
          if(x>=6){rojo+=linea5[x-6]; verde+=linea5[x-5]; azul+=linea5[x-4];}
          if(x>=3){rojo+=linea5[x-3]; verde+=linea5[x-2]; azul+=linea5[x-1];}
          if(x+2<dibujo->Bitmap->Width*3) {rojo+=linea5[x]; verde+=linea5[x+1]; azul+=linea5[x+2];}
          if(x+5<dibujo->Bitmap->Width*3) {rojo+=linea5[x+3]; verde+=linea5[x+4]; azul+=linea5[x+5];}
          if(x+8<dibujo->Bitmap->Width*3) {rojo+=linea5[x+6]; verde+=linea5[x+7]; azul+=linea5[x+8];}
        }

        rojo= floor(rojo/25);
        verde= floor(verde/25);
        azul= floor(azul/25);
        while(rojo%10 && rojo>5) rojo--;
        while(verde%10 && verde>5) verde--;
        while(azul%10 && azul>5) azul--;
        if(rojo<=5)rojo=0;
        if(verde<=5)verde=0;
        if(azul<=5)azul=0;
        //En realidad el azul es el rojo y viceversa
        int pos= floor(x/4);
        if(!((azul>=200 && azul<=240) && (verde>=0 && verde<=70) && (rojo>=0 || rojo<=80))){
          lineaYap[pos]=0; lineaYap[pos+1]=0; lineaYap[pos+2]=0;}
        else{lineaYap[pos]=255; lineaYap[pos+1]=255; lineaYap[pos+2]=255; cont++; acX+=pos; acY+=floor(y/4);}
        x+=2;
      }
     }
    }
   }
   dibujo2->Bitmap->Height/=4;
   dibujo2->Bitmap->Width/=4;
   int difY= floor(dibujo2->Bitmap->Height/2 - floor(acY/cont));
   int difX= floor(dibujo2->Bitmap->Width/2 - floor(acX/(3*cont)));
   return Centrar(dibujo2,difY,difX);
}

TPicture* Centrar(TPicture* dibujo, int difY, int difX)
{
 Byte* aux;
 Byte* linea;
  if(difY<0){
    int y;
    for (y = 0; y-difY < dibujo->Bitmap->Height; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y-difY];
      aux= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*3; x++)aux[x]=linea[x];
    }
    while(y< dibujo->Bitmap->Height){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*3; x++)linea[x]=0;
      y++;
    }
  }


  if(difY>0){
    int y;
    for (y = dibujo->Bitmap->Height-1; y-difY >= 0; y--){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y-difY];
      aux= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*3; x++)aux[x]=linea[x];
    }
    while(y>= 0){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*3; x++)linea[x]=0;
      y--;
    }
  }


  if(difX<0){
    for (int y = 0; y< dibujo->Bitmap->Height; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      int x;
      for (x = 0; x-(difX*3)+2 < dibujo->Bitmap->Width*3; x++){
        linea[x]= linea[x-(difX*3)];
        linea[x+1]= linea[x-(difX*3)+1];
        linea[x+2]= linea[x-(difX*3)+2];
        x+=2;
      }
      while(x< dibujo->Bitmap->Width*3){
        linea[x]=0; linea[x+1]=0; linea[x+2]=0; x+=2;
      }
    }
  }


  if(difX>0){
    for (int y = 0; y< dibujo->Bitmap->Height-1; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      int x;
      for (x = (dibujo->Bitmap->Width*3)-1; x-(difX*3)+2 >= 0; x--){
        linea[x]= linea[x-(difX*3)];
        linea[x-1]= linea[x-(difX*3)-1];
        linea[x-2]= linea[x-(difX*3)-2];
        x-=2;
      }
      while(x>=0){
        linea[x]=0; linea[x-1]=0; linea[x-2]=0; x-=2;
      }
    }
  }
  return dibujo;
}
