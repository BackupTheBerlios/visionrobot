//---------------------------------------------------------------------------

#pragma hdrstop
#include "Codigo_Filtro.h"
#pragma package(smart_init)


//---------------------------------------------------------------------------


Filtro::Filtro()
{
        m_Bytes = 3;
        m_rojoSup_orden=240;
        m_rojoInf_orden=160;
        m_verdeSup_orden=120;
        m_verdeInf_orden=70;
        m_azulSup_orden=120;
        m_azulInf_orden=70;
        m_rojoSup_param=100;
        m_rojoInf_param=50;
        m_verdeSup_param=100;
        m_verdeInf_param=50;
        m_azulSup_param=100;
        m_azulInf_param=50;
        m_Salida = new data_out;
}


//---------------------------------------------------------------------------


data_out* Filtro::Filtrar(BYTE* buffer, int h, int w)
{
   BYTE* tipo_orden= new BYTE[h*w*m_Bytes];
   BYTE* orden= new BYTE[h*w*m_Bytes];
   int cont,cont2,acX,acX2,acY,acY2,posY;
   cont=cont2=acX=acX2=acY=acY2=0;
   //data_out* salida;

   for (int y = 0; y < h; y++){
     for (int x = 0; x < w*m_Bytes; x++){
        int rojo=0; int verde=0; int azul=0;
        if(y-2>=0 && y+2<h)
          for(int i=y-2; i<y+2; i++)
            if(x-8>=0 && x+10<w*m_Bytes)
              for(int j=x-8; j<x+10; j++){
                rojo+=buffer[(i*w*m_Bytes)+j]; verde+=buffer[(i*w*m_Bytes)+j+1]; azul+=buffer[(i*w*m_Bytes)+j+2];
                j+=3;
              }
        rojo= ((int)floor(rojo/25));
        verde= ((int)floor(verde/25));
        azul= ((int)floor(azul/25));
        while(rojo%10 && rojo>=0) rojo--;
        while(verde%10 && verde>=0) verde--;
        while(azul%10 && azul>=0) azul--;
        posY=y*w*m_Bytes;
        if(!((azul>=m_rojoInf_orden && azul<=m_rojoSup_orden) && (verde>=m_verdeInf_orden && verde<=m_verdeSup_orden) && (rojo>=m_azulInf_orden || rojo<=m_azulSup_orden))){
          tipo_orden[posY+x]=0;
          tipo_orden[posY+x+1]=0;
          tipo_orden[posY+x+2]=0;
        }
        else{
          tipo_orden[posY+x]=255;
          tipo_orden[posY+x+1]=255;
          tipo_orden[posY+x+2]=255;
          cont++; acX+=x; acY+=y;
        }
        //Cambiar parametros para verde
        if(!((azul>=m_rojoInf_param && azul<=m_rojoSup_param) && (verde>=m_verdeInf_param && verde<=m_verdeSup_param) && (rojo>=m_azulInf_param || rojo<=m_azulSup_param))){
          orden[posY+x]=0;
          orden[posY+x+1]=0;
          orden[posY+x+2]=0;
        }
        else{
          orden[posY+x]=255;
          orden[posY+x+1]=255;
          orden[posY+x+2]=255;
          cont2++; acX2+=x; acY2+=y;
        }
        x+=3;
     }
   }
   if(cont!=0){
     int difY= ((int)floor(h/2 - floor(acY/cont)));
     int difX= ((int)floor(w/2 - ((int)floor(acX/(m_Bytes*cont)))));
     tipo_orden= Centrar(tipo_orden,difY,difX,h,w);
   }
   if(cont2!=0){
     int difY2= ((int)floor(h/2 - ((int)floor(acY2/cont2))));
     int difX2= ((int)floor(w/2 - ((int)floor(acX2/(m_Bytes*cont2)))));
     orden= Centrar(orden,difY2,difX2,h,w);
   }
   m_Salida->tipo_orden=tipo_orden;
   m_Salida->orden=orden;
   return m_Salida;
}


//---------------------------------------------------------------------------


BYTE* Filtro::Centrar(BYTE* dibujo, int difY, int difX, int h, int w)
{
  if(difY<0){
    int y;
    for (y=0; y-difY<h; y++)
      for (int x = 0; x < w*m_Bytes; x++)
        dibujo[(y*w*m_Bytes)+x]=dibujo[((y-difY)*w*m_Bytes)+x];
    while(y<h){
      for (int x=0; x<w*m_Bytes; x++)dibujo[(y*w*m_Bytes)+x]=0;
      y++;
    }
  }
  if(difY>0){
    int y;
    for (y = h-1; y-difY>=0; y--)
      for (int x=0; x<w*m_Bytes; x++)
         dibujo[(y*w*m_Bytes)+x]=dibujo[((y-difY)*w*m_Bytes)+x];
    while(y>= 0){
      for (int x=0; x<w*m_Bytes; x++)dibujo[(y*w*m_Bytes)+x]=0;
      y--;
    }
  }
  if(difX<0){
    for (int y=0; y<h; y++){
      int x;
      for (x=0; x-(difX*m_Bytes)+3<w*m_Bytes; x++){
        dibujo[(y*w*m_Bytes)+x]= dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes))];
        dibujo[(y*w*m_Bytes)+x+1]= dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes)+1)];
        dibujo[(y*w*m_Bytes)+x+2]= dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes)+2)];
        x+=3;
      }
      while(x+2<w*m_Bytes){
        dibujo[(y*w*m_Bytes)+x]=0; dibujo[(y*w*m_Bytes)+x+1]=0; dibujo[(y*w*m_Bytes)+x+2]=0; x+=3;
      }
    }
  }
  if(difX>0){
    for (int y=0; y<h-1; y++){
      int x;
      for (x=(w*m_Bytes)-2; x-(difX*m_Bytes)+3 >= 0; x--){
        dibujo[(y*w*m_Bytes)+x]=dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes))];
        dibujo[(y*w*m_Bytes)+x-1]=dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes)-1)];
        dibujo[(y*w*m_Bytes)+x-2]=dibujo[(y*w*m_Bytes)+(x-(difX*m_Bytes)-2)];
        x-=3;
      }
      while(x>=0){
        dibujo[(y*w*m_Bytes)+x]=0; dibujo[(y*w*m_Bytes)+x-1]=0; dibujo[(y*w*m_Bytes)+x-2]=0; x-=3;
      }
    }
  }
  return dibujo;
}


//---------------------------------------------------------------------------


void Filtro::SetColor(int rs,int ri,int vs,int vi,int as,int ai,int rs2,int ri2,int vs2,int vi2,int as2,int ai2){
        m_rojoSup_orden=rs;
        m_rojoInf_orden=ri;
        m_verdeSup_orden=vs;
        m_verdeInf_orden=vi;
        m_azulSup_orden=as;
        m_azulInf_orden=ai;
        m_rojoSup_param=rs2;
        m_rojoInf_param=ri2;
        m_verdeSup_param=vs2;
        m_verdeInf_param=vi2;
        m_azulSup_param=as2;
        m_azulInf_param=ai2;
}


//---------------------------------------------------------------------------

