//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "CImagen.h"
#include <assert.h>

//---------------------------------------------------------------------------


Imagen::Imagen()
{
  nombre="";
  dibujo=NULL;
}

//---------------------------------------------------------------------------

void Imagen::Leer(AnsiString fileName)
{
   nombre= fileName;
   dibujo= new TPicture();
   dibujo->LoadFromFile(fileName);
   TPicture* dibujo2=new TPicture();
   dibujo2->Bitmap=dibujo->Bitmap;
   dibujo=Filtro(dibujo,dibujo2);
}

//---------------------------------------------------------------------------

Imagen::~Imagen()
{
 delete dibujo;
}

//---------------------------------------------------------------------------

AnsiString Imagen::GetNombre()
{
 return nombre;
}

//---------------------------------------------------------------------------

TPicture* Imagen::GetDibujo()
{
 return dibujo;
}


//---------------------------------------------------------------------------

TPicture* Imagen::Filtro(TPicture* dibujo, TPicture* dibujo2)
{
   Byte* linea1=NULL;
   Byte* linea2=NULL;
   Byte* linea3=NULL;
   Byte* linea4=NULL;
   Byte* linea5=NULL;
   Byte* lineaYap=NULL;
   int cont=0; int acX=0; int acY=0;

   for (int y = 0; y < dibujo->Bitmap->Height; y++){
     if(y-2>=0)linea1= (Byte*)dibujo->Bitmap->ScanLine[y-2]; else linea1=NULL;
     if(y-1>=0)linea2= (Byte*)dibujo->Bitmap->ScanLine[y-1]; else linea2=NULL;
     linea3= (Byte*)dibujo->Bitmap->ScanLine[y];
     lineaYap= (Byte*)dibujo2->Bitmap->ScanLine[y];
     if(y+1<dibujo->Bitmap->Height)linea4= (Byte*)dibujo->Bitmap->ScanLine[y+1]; else linea4=NULL;
     if(y+2<dibujo->Bitmap->Height)linea5= (Byte*)dibujo->Bitmap->ScanLine[y+2]; else linea5=NULL;

     for (int x = 0; x < dibujo->Bitmap->Width*4; x++){
        int rojo=0; int verde=0; int azul=0;
        if(linea1!=NULL){
          if(x>=8){rojo+=linea1[x-8]; verde+=linea1[x-7]; azul+=linea1[x-6];}
          if(x>=4){rojo+=linea1[x-4]; verde+=linea1[x-3]; azul+=linea1[x-2];}
          if(x+2<dibujo->Bitmap->Width*4) {rojo+=linea1[x]; verde+=linea1[x+1]; azul+=linea1[x+2];}
          if(x+6<dibujo->Bitmap->Width*4) {rojo+=linea1[x+4]; verde+=linea1[x+5]; azul+=linea1[x+6];}
          if(x+10<dibujo->Bitmap->Width*4) {rojo+=linea1[x+8]; verde+=linea1[x+9]; azul+=linea1[x+10];}
        }
        if(linea2!=NULL){
          if(x>=8){rojo+=linea2[x-8]; verde+=linea2[x-7]; azul+=linea2[x-6];}
          if(x>=4){rojo+=linea2[x-4]; verde+=linea2[x-3]; azul+=linea2[x-2];}
          if(x+2<dibujo->Bitmap->Width*4) {rojo+=linea2[x]; verde+=linea2[x+1]; azul+=linea2[x+2];}
          if(x+6<dibujo->Bitmap->Width*4) {rojo+=linea2[x+4]; verde+=linea2[x+5]; azul+=linea2[x+6];}
          if(x+10<dibujo->Bitmap->Width*4) {rojo+=linea2[x+8]; verde+=linea2[x+9]; azul+=linea2[x+10];}
        }
        if(linea3!=NULL){
          if(x>=8){rojo+=linea3[x-8]; verde+=linea3[x-7]; azul+=linea3[x-6];}
          if(x>=4){rojo+=linea3[x-4]; verde+=linea3[x-3]; azul+=linea3[x-2];}
          if(x+2<dibujo->Bitmap->Width*4) {rojo+=linea3[x]; verde+=linea3[x+1]; azul+=linea3[x+2];}
          if(x+6<dibujo->Bitmap->Width*4) {rojo+=linea3[x+4]; verde+=linea3[x+5]; azul+=linea3[x+6];}
          if(x+10<dibujo->Bitmap->Width*4) {rojo+=linea3[x+8]; verde+=linea3[x+9]; azul+=linea3[x+10];}
        }
        if(linea4!=NULL){
          if(x>=8){rojo+=linea4[x-8]; verde+=linea4[x-7]; azul+=linea4[x-6];}
          if(x>=4){rojo+=linea4[x-4]; verde+=linea4[x-3]; azul+=linea4[x-2];}
          if(x+2<dibujo->Bitmap->Width*4) {rojo+=linea4[x]; verde+=linea4[x+1]; azul+=linea4[x+2];}
          if(x+6<dibujo->Bitmap->Width*4) {rojo+=linea4[x+4]; verde+=linea4[x+5]; azul+=linea4[x+6];}
          if(x+10<dibujo->Bitmap->Width*4) {rojo+=linea4[x+8]; verde+=linea4[x+9]; azul+=linea4[x+10];}
        }
        if(linea5!=NULL){
          if(x>=8){rojo+=linea5[x-8]; verde+=linea5[x-7]; azul+=linea5[x-6];}
          if(x>=4){rojo+=linea5[x-4]; verde+=linea5[x-3]; azul+=linea5[x-2];}
          if(x+2<dibujo->Bitmap->Width*4) {rojo+=linea5[x]; verde+=linea5[x+1]; azul+=linea5[x+2];}
          if(x+6<dibujo->Bitmap->Width*4) {rojo+=linea5[x+4]; verde+=linea5[x+5]; azul+=linea5[x+6];}
          if(x+10<dibujo->Bitmap->Width*4) {rojo+=linea5[x+8]; verde+=linea5[x+9]; azul+=linea5[x+10];}
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
        if(!((azul>=160 && azul<=240) && (verde>=70 && verde<=120) && (rojo>=70 || rojo<=120))){
          lineaYap[x]=0; lineaYap[x+1]=0; lineaYap[x+2]=0;}
        else{lineaYap[x]=255; lineaYap[x+1]=255; lineaYap[x+2]=255; cont++; acX+=x; acY+=y;}
        x+=3;
    }
   }
   if(cont>0){
     int difY= floor(dibujo2->Bitmap->Height/2 - floor(acY/cont));
     int difX= floor(dibujo2->Bitmap->Width/2 - floor(acX/(4*cont)));
     return Centrar(dibujo2,difY,difX);
   }
   else return dibujo2;
}

TPicture* Imagen::Centrar(TPicture* dibujo, int difY, int difX)
{
 Byte* aux;
 Byte* linea;
  if(difY<0){
    int y;
    for (y = 0; y-difY < dibujo->Bitmap->Height; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y-difY];
      aux= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*4; x++)aux[x]=linea[x];
    }
    while(y< dibujo->Bitmap->Height){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*4; x++)linea[x]=0;
      y++;
    }
  }


  if(difY>0){
    int y;
    for (y = dibujo->Bitmap->Height-1; y-difY >= 0; y--){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y-difY];
      aux= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*4; x++)aux[x]=linea[x];
    }
    while(y>= 0){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      for (int x = 0; x < dibujo->Bitmap->Width*4; x++)linea[x]=0;
      y--;
    }
  }

  if(difX<0){
    for (int y = 0; y< dibujo->Bitmap->Height; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      int x;
      for (x = 0; x-(difX*4)+3 < dibujo->Bitmap->Width*4; x++){
        linea[x]= linea[x-(difX*4)];
        linea[x+1]= linea[x-(difX*4)+1];
        linea[x+2]= linea[x-(difX*4)+2];
        x+=3;
      }
      while(x+2< dibujo->Bitmap->Width*4){
        linea[x]=0; linea[x+1]=0; linea[x+2]=0; x+=3;
      }
    }
  }

  if(difX>0){
    for (int y = 0; y< dibujo->Bitmap->Height-1; y++){
      linea= (Byte*)dibujo->Bitmap->ScanLine[y];
      int x;
      for (x = (dibujo->Bitmap->Width*4)-2; x-(difX*4)+3 >= 0; x--){
        linea[x]= linea[x-(difX*4)];
        linea[x-1]= linea[x-(difX*4)-1];
        linea[x-2]= linea[x-(difX*4)-2];
        x-=3;
      }
      while(x>=0){
        linea[x]=0; linea[x-1]=0; linea[x-2]=0; x-=3;
      }
    }
  } 
  return dibujo;
}

