#include "filtro.h"
#include "red_neuronal_sdk.h"
#include <stdlib.h>
#include <math.h>

filtro_t * filtro_gestos_crear() {
    filtro_t *filtro = (filtro_t *)malloc(sizeof(filtro_t));
    filtro->m_salida = (red_neuronal_in_t *)malloc(sizeof(red_neuronal_in_t));
    return filtro;
}

void filtro_gestos_borrar(filtro_t **filtro) {
    if(filtro && (*filtro)) {
    free(*filtro);
    free((*filtro)->m_salida);
    *filtro = 0;
    }
}
red_neuronal_in_t* filtro_gestos_filtrar(filtro_t * filtro){
    if(!filtro || !filtro->m_buffer) {
       filtro->m_salida->m_tipo_orden = 0;
       filtro->m_salida->m_orden = 0;
        return filtro->m_salida;
     }
    int h = filtro->m_buffer->m_alto;
    int w = filtro->m_buffer->m_ancho;
    int m_rojoInf_orden = filtro->m_orden.m_inferior.m_r;
    int m_rojoSup_orden = filtro->m_orden.m_superior.m_r;
    int m_verdeInf_orden = filtro->m_orden.m_inferior.m_v;
    int m_verdeSup_orden = filtro->m_orden.m_superior.m_v;
    int m_azulInf_orden = filtro->m_orden.m_inferior.m_a;
    int m_azulSup_orden = filtro->m_orden.m_superior.m_a;
    
    int m_rojoInf_param = filtro->m_parametro.m_inferior.m_r;
    int m_rojoSup_param = filtro->m_parametro.m_superior.m_r;
    int m_verdeInf_param = filtro->m_parametro.m_inferior.m_v;
    int m_verdeSup_param = filtro->m_parametro.m_superior.m_v;
    int m_azulInf_param = filtro->m_parametro.m_inferior.m_a;
    int m_azulSup_param = filtro->m_parametro.m_superior.m_a;
    
    int bytes = filtro->m_buffer->m_bytes;
    char *buffer = filtro->m_buffer->m_imagen;
   char* tipo_orden = (char*)malloc(sizeof(char) * h * w * bytes); //new BYTE[h*w*bytes];
   char* orden = (char*)malloc(sizeof(char) * h * w * bytes); //new BYTE[h*w*bytes];
   int cont,cont2,acX,acX2,acY,acY2,posY;
   cont=cont2=acX=acX2=acY=acY2=0;
    int y, x, j, i;
   for (y = 0; y < h; y++){
     for (x = 0; x < w * bytes; x++){
        int rojo=0; int verde=0; int azul=0;
        if(y-2>=0 && y+2<h)
          for(i=y-2; i<y+2; i++)
            if(x-8>=0 && x+10<w*bytes)
              for(j=x-8; j<x+10; j++){
                rojo+=buffer[(i*w*bytes)+j]; verde+=buffer[(i*w*bytes)+j+1]; azul+=buffer[(i*w*bytes)+j+2];
                j+=bytes-1;
              }
        rojo= ((int)floor(rojo/25));
        verde= ((int)floor(verde/25));
        azul= ((int)floor(azul/25));
        while(rojo%10 && rojo>=0) rojo--;
        while(verde%10 && verde>=0) verde--;
        while(azul%10 && azul>=0) azul--;
        posY=y*w*bytes;
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
        x+=bytes-1;
     }
   }
   if(cont!=0){
     int difY= ((int)floor(h/2 - floor(acY/cont)));
     int difX= ((int)floor(w/2 - ((int)floor(acX/(bytes*cont)))));
     tipo_orden= filtro_gestos_centrar(filtro, tipo_orden, difY, difX/*tipo_orden,difY,difX,h,w, bytes*/);
   }
   if(cont2!=0){
     int difY2= ((int)floor(h/2 - ((int)floor(acY2/cont2))));
     int difX2= ((int)floor(w/2 - ((int)floor(acX2/(bytes*cont2)))));
     orden= filtro_gestos_centrar(filtro, orden, difY2, difX2);//Centrar(orden,difY2,difX2,h,w, bytes);
   }
   filtro->m_salida->m_tipo_orden=tipo_orden;
   filtro->m_salida->m_orden= orden;
   filtro->m_salida->m_ancho= w;
   filtro->m_salida->m_alto= h;
   //WARNING: y los bytes?
   return filtro->m_salida;
}
void filtro_gestos_set_color(filtro_t *filtro, int rs,int ri,int vs,int vi,int as,int ai,int rs2,int ri2,int vs2,int vi2,int as2,int ai2){
    filtro->m_orden.m_superior.m_r = rs;
    filtro->m_orden.m_inferior.m_r = ri;
    filtro->m_orden.m_superior.m_v = vs;
    filtro->m_orden.m_inferior.m_v = vi;
    filtro->m_orden.m_superior.m_a = as;
    filtro->m_orden.m_inferior.m_a = ai;
    filtro->m_parametro.m_superior.m_r =rs2;
    filtro->m_parametro.m_inferior.m_r =ri2;
    filtro->m_parametro.m_superior.m_v =vs2;
    filtro->m_parametro.m_inferior.m_v = vi2;
    filtro->m_parametro.m_superior.m_a = as2;
    filtro->m_parametro.m_inferior.m_a = ai2;
}
char * filtro_gestos_centrar(filtro_t *filtro, char *dibujo, int difY, int difX){
    int h = filtro->m_buffer->m_alto;
    int w = filtro->m_buffer->m_ancho;
    int bytes = filtro->m_buffer->m_bytes;
    //char *dibujo = filtro->m_buffer->m_imagen;
    
  if(difY<0){
    int y, x;
    for (y=0; y-difY<h; y++)
      for (x = 0; x < w*bytes; x++)
        dibujo[(y*w*bytes)+x]=dibujo[((y-difY)*w*bytes)+x];
    while(y<h){
      for (x=0; x<w*bytes; x++)dibujo[(y*w*bytes)+x]=0;
      y++;
    }
  }
  if(difY>0){
    int y, x;
    for (y = h-1; y-difY>=0; y--)
      for (x=0; x<w*bytes; x++)
         dibujo[(y*w*bytes)+x]=dibujo[((y-difY)*w*bytes)+x];
    while(y>= 0){
      for (x=0; x<w*bytes; x++)dibujo[(y*w*bytes)+x]=0;
      y--;
    }
  }
  if(difX<0){
    int y;
    for (y=0; y<h; y++){
      int x;
      for (x=0; x-(difX*bytes)+bytes-1<w*bytes; x++){
        dibujo[(y*w*bytes)+x]= dibujo[(y*w*bytes)+(x-(difX*bytes))];
        dibujo[(y*w*bytes)+x+1]= dibujo[(y*w*bytes)+(x-(difX*bytes)+1)];
        dibujo[(y*w*bytes)+x+2]= dibujo[(y*w*bytes)+(x-(difX*bytes)+2)];
        x+=bytes-1;
      }
      while(x+2<w*bytes){
        dibujo[(y*w*bytes)+x]=0; dibujo[(y*w*bytes)+x+1]=0; dibujo[(y*w*bytes)+x+2]=0; x+=bytes-1;
      }
    }
  }
  if(difX>0){
    int y;
    for (y=0; y<h-1; y++){
      int x;
      for (x=(w*bytes)-2; x-(difX*bytes)+bytes-1 >= 0; x--){
        dibujo[(y*w*bytes)+x]=dibujo[(y*w*bytes)+(x-(difX*bytes))];
        dibujo[(y*w*bytes)+x-1]=dibujo[(y*w*bytes)+(x-(difX*bytes)-1)];
        dibujo[(y*w*bytes)+x-2]=dibujo[(y*w*bytes)+(x-(difX*bytes)-2)];
        x-=bytes-1;
      }
      while(x>=0){
        dibujo[(y*w*bytes)+x]=0; dibujo[(y*w*bytes)+x-1]=0; dibujo[(y*w*bytes)+x-2]=0; x-=bytes-1;
      }
    }
  }
  return dibujo;
}
