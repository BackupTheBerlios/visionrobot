//---------------------------------------------------------------------------

#include "Codigo_Filtro.h"

//---------------------------------------------------------------------------


salida* Filtro(BYTE* buffer, int h, int w)
{
   BYTE* tipo_orden= new BYTE[((int)floor(h/4))*((int)floor(w/4)*3)];
   BYTE* orden= new BYTE[((int)floor(h/4))*((int)floor(w/4)*3)];
   int cont=0; int acX=0; int acY=0;
   int cont2=0; int acX2=0; int acY2=0;

   for (int y = 0; y < h; y++){
    if(y%4==1){
     for (int x = 0; x < w*3; x++){
      if(((x/3)%4)==1){
        int rojo=0; int verde=0; int azul=0;
        if(y-2>=0 && y+2<h)
          for(int i=y-2; i<y+2; i++)
            if(x-6>=0 && x+8<w*3)
              for(int j=x-6; j<x+8; j++){
                rojo+=buffer[(i*w*3)+j]; verde+=buffer[(i*w*3)+j+1]; azul+=buffer[(i*w*3)+j+2];
                j+=2;
              }              
        rojo= ((int)floor(rojo/25));
        verde= ((int)floor(verde/25));
        azul= ((int)floor(azul/25));
        while(rojo%10 && rojo>5) rojo--;
        while(verde%10 && verde>5) verde--;
        while(azul%10 && azul>5) azul--;
        if(rojo<=5)rojo=0;
        if(verde<=5)verde=0;
        if(azul<=5)azul=0;
        int pos= ((int)floor(x/4));
        int posY=(((int)floor(y/4))*((int)floor(w/4)*3));
        //if(!((azul>=200 && azul<=240) && (verde>=0 && verde<=70) && (rojo>=0 || rojo<=80))){
        if(!(verde>=200)){
          tipo_orden[posY+pos]=0;
          tipo_orden[posY+pos+1]=0;
          tipo_orden[posY+pos+2]=0;
        }
        else{
          tipo_orden[posY+pos]=255;
          tipo_orden[posY+pos+1]=255;
          tipo_orden[posY+pos+2]=255;
          cont++; acX+=pos; acY+=((int)floor(y/4));
        }
        //Cambiar parametros para verde
        if(!((azul>=100 && azul<=240) && (verde>=0 && verde<=170) && (rojo>=0 || rojo<=80))){
          orden[posY+pos]=0;
          orden[posY+pos+1]=0;
          orden[posY+pos+2]=0;
        }
        else{
          orden[posY+pos]=255;
          orden[posY+pos+1]=255;
          orden[posY+pos+2]=255;
          cont2++; acX2+=pos; acY2+=((int)floor(y/4));
        }
        x+=2;
      }
     }
    }
   }
  if(cont!=0){
     int difY= ((int)floor(h/8 - ((int)floor(acY/cont))));
     int difX= ((int)floor(w/8 - ((int)floor(acX/(3*cont)))));
     tipo_orden= Centrar(tipo_orden,difY,difX,((int)floor(h/4)),((int)floor(w/4)));
   }
   if(cont2!=0){
     int difY2= ((int)floor(h/8 - floor(acY2/cont2)));
     int difX2= ((int)floor(w/8 - floor(acX2/(3*cont2))));
     orden= Centrar(orden,difY2,difX2,((int)floor(h/4)),((int)floor(w/4)));
   }
   salida* exit;
   exit->tipo_orden=tipo_orden;
   exit->orden=orden;
   return exit;
}


//---------------------------------------------------------------------------


BYTE* Centrar(BYTE* dibujo, int difY, int difX, int h, int w)
{
  if(difY<0){
    int y;
    for (y=0; y-difY<h; y++)
      for (int x = 0; x < w*3; x++)
        dibujo[(y*w*3)+x]=dibujo[((y-difY)*w*3)+x];
    while(y<h){
      for (int x=0; x<w*3; x++)dibujo[(y*w*3)+x]=0;
      y++;
    }
  }
  if(difY>0){
    int y;
    for (y = h-1; y-difY>=0; y--)
      for (int x=0; x<w*3; x++)
         dibujo[(y*w*3)+x]=dibujo[((y-difY)*w*3)+x];
    while(y>= 0){
      for (int x=0; x<w*3; x++)dibujo[(y*w*3)+x]=0;
      y--;
    }
  }
  if(difX<0){
    for (int y=0; y<h; y++){
      int x;
      for (x=0; x-(difX*3)+2<w*3; x++){
        dibujo[(y*w*3)+x]= dibujo[(y*w*3)+(x-(difX*3))];
        dibujo[(y*w*3)+x+1]= dibujo[(y*w*3)+(x-(difX*3)+1)];
        dibujo[(y*w*3)+x+2]= dibujo[(y*w*3)+(x-(difX*3)+2)];
        x+=2;
      }
      while(x<w*3){
        dibujo[(y*w*3)+x]=0; dibujo[(y*w*3)+x+1]=0; dibujo[(y*w*3)+x+2]=0; x+=2;
      }
    }
  }
  if(difX>0){
    for (int y=0; y<h-1; y++){
      int x;
      for (x=(w*3)-3; x-(difX*3)+2 >= 0; x--){
        dibujo[(y*w*3)+x]=dibujo[(y*w*3)+(x-(difX*3))];
        dibujo[(y*w*3)+x+1]=dibujo[(y*w*3)+(x-(difX*3)-1)];
        dibujo[(y*w*3)+x+2]=dibujo[(y*w*3)+(x-(difX*3)-2)];
        x-=2;
      }
      while(x>=0){
        dibujo[(y*w*3)+x]=0; dibujo[(y*w*3)+x-1]=0; dibujo[(y*w*3)+x-2]=0; x-=2;
      }
    }
  }
  return dibujo;
}


//---------------------------------------------------------------------------

