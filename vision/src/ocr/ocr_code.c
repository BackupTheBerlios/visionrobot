
/*! \file ocr_code.c
    \brief Implementación del ocr.
    \author Diego Sánchez
    \version 0.2
 */  
    
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */ 


#include "ocr_code.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <values.h>
#include <string.h>


//---------------------------------------------------------------------------


const char* FREC_CAR="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-*/=%";
const  int LONG_CAR= 42;
const double FACTOR= 1.9;
//const int FIELDS=30;
#define FIELDS 30
const int NUMWORDS=22;

#ifndef MAXFLOAT
#define MAXFLOAT 999999999999.9f
#endif

typedef struct {
        int m_x;
        int m_y;
}coord_t;

typedef struct{
        int columnasIzq[FIELDS];
        int columnasDcha[FIELDS];
        int filasTop[FIELDS];
        int filasDown[FIELDS];
        int cimaFT;
        int cimaFD;
        int cimaCD;
        int cimaCI;
        coord_t** ptosSI;
        coord_t** ptosID;
        int* espacios;
}fields_t;


//---------------------------------------------------------------------------


double v_abs(double in){if(in<0)return -in; else return in;}


double match_hexadecagon(hexadecagon_t* local,hexadecagon_t* visitor)
{
  double result=0; int i;
  for(i=0; i<NUMPTOS; i++)
    result+=v_abs(visitor->m_list[i]-local->m_list[i]);
  return result;
}


char match_constallation(constellation_t* local, hexadecagon_t* visitor)
{
  int pos,i;
  double smaller,dif;
  pos=0; smaller=MAXFLOAT;
  for(i=0; i<local->m_top;i++){
     dif= match_hexadecagon(local->m_list[i],visitor);
     if(dif<smaller){smaller=dif; pos=i;}
  }
  return local->m_list[pos]->m_id;
}


int whiteColumn(int x, int top, int down, filtro_gestos_in_imagen_t* image)
{
   int i,pos;
   pos= image->m_ancho*image->m_bytes;
   for(i=top; i<down; i++)
     if(!image->m_imagen[(i*pos)+x])return 0;
   return 1;
}


int whiteRow(int x,int left, int right, filtro_gestos_in_imagen_t* image)
{
 int i,pos;
 pos= x*image->m_bytes*image->m_ancho;
 for(i=left;i<right;i+=3)
   if(!image->m_imagen[pos+i])return 0;
 return 1;
}


fields_t* limits(filtro_gestos_in_imagen_t* image)
{
 fields_t* fields= (fields_t*)malloc(sizeof(fields_t));
 fields->cimaCI=fields->cimaCD=fields->cimaFT=fields->cimaFD=0;
 int arriba,izq,i,l,cont,punt,posi,media,dif,jumps;
 arriba=izq=cont=punt=posi=media=jumps=0;
 for(i=1; i<image->m_alto-1; i++)
     if(whiteRow(i,0,image->m_ancho*image->m_bytes,image)){
       if(!whiteRow(i-1,0,image->m_ancho*image->m_bytes,image) && arriba){
         fields->filasDown[fields->cimaFD]=i; fields->cimaFD++; arriba=0;
       }
       if(!whiteRow(i+1,0,image->m_ancho*image->m_bytes,image)){
         fields->filasTop[fields->cimaFT]=i; fields->cimaFT++; arriba=1;
       }
   }
 int* caracXfila= (int*)malloc(sizeof(int)*fields->cimaFT);

 for(l=0; l<fields->cimaFT; l++){
   for(i=image->m_bytes; i<(image->m_ancho-1)*image->m_bytes; i+=image->m_bytes){
     if(whiteColumn(i,fields->filasTop[l],fields->filasDown[l],image)){
       if(!whiteColumn(i-image->m_bytes,fields->filasTop[l],fields->filasDown[l],image) && izq){
         fields->columnasIzq[fields->cimaCI]=i; fields->cimaCI++; izq=0;
       }
       if(!whiteColumn(i+image->m_bytes,fields->filasTop[l],fields->filasDown[l],image)){
         fields->columnasDcha[fields->cimaCD]=i; fields->cimaCD++; izq=1;
       }
     }
   }
   caracXfila[l] = (l>0) ? fields->cimaCI-caracXfila[l-1]: fields->cimaCI;
 }

 fields->ptosSI= (coord_t**)malloc(sizeof(coord_t*)*fields->cimaCD);
 for(i=0; i<fields->cimaCD; i++){
     if(punt==caracXfila[posi]){punt=0; posi++;}
     fields->ptosSI[cont]= (coord_t*)malloc(sizeof(coord_t));
     fields->ptosSI[cont]->m_x= fields->columnasDcha[i];
     fields->ptosSI[cont]->m_y= fields->filasTop[posi];
     cont++; punt++;
   }

 fields->ptosID= (coord_t**)malloc(sizeof(coord_t*)*fields->cimaCI);
 cont=punt=posi=0;
 for(i=0; i<fields->cimaCI; i++){
     if(punt==caracXfila[posi]){punt=0; posi++;}
     fields->ptosID[cont]= (coord_t*)malloc(sizeof(coord_t));
     fields->ptosID[cont]->m_x= fields->columnasIzq[i];
     fields->ptosID[cont]->m_y= fields->filasDown[posi];
     cont++; punt++;
   }
 free(caracXfila);

 //Saca espacios
 if(fields->cimaCI > 0) {
   fields->espacios= (int*)malloc(sizeof(int)*fields->cimaCI-1);
   for(i=0; i<fields->cimaCI-1; i++)fields->espacios[i]=0;
   for(i=0; i<fields->cimaCI-1; i++){
     dif= fields->ptosSI[i+1]->m_x-fields->ptosID[i]->m_x;
     if(dif<0){fields->espacios[i]=1;jumps++;}else media+=dif;
   }
   if(fields->cimaCI-jumps>0) {
     media/=fields->cimaCI-jumps;
   }
   else {
     media=0;
   }
   for(i=0; i<fields->cimaCI-1; i++){
     dif= fields->ptosSI[i+1]->m_x-fields->ptosID[i]->m_x;
     if(dif>media*FACTOR)fields->espacios[i]=1;
   }
 }
 else {
   fields->espacios = 0;
 }

 //Ajustar
  int left,right,up,down;
  for(i=0; i<fields->cimaCI; i++){
    left=fields->ptosSI[i]->m_x; right=fields->ptosID[i]->m_x;
    down=fields->ptosID[i]->m_y; up=fields->ptosSI[i]->m_y;
    int upi,downi,lefti,righti;
    upi=downi=lefti=righti=0;
    while(!upi)(!whiteRow(up,left,right,image)) ? upi=1: up++;
    while(!downi)(!whiteRow(down,left,right,image)) ? downi=1: down--;
    while(!lefti)(!whiteColumn(left,up,down,image)) ? lefti=1: left++;
    while(!righti)(!whiteColumn(right,up,down,image)) ? righti=1: right--;
    fields->ptosID[i]->m_x=right; fields->ptosSI[i]->m_x=left;
    fields->ptosSI[i]->m_y=up; fields->ptosID[i]->m_y=down;
  }
  return fields;
}


double modulo(int x1, int y1, int x2, int y2)
{
  return sqrt((x2-x1)*(x2-x1)+(y1-y2)*(y1-y2));
}


int makeHexadecagonAux(coord_t* si, coord_t* id, hexadecagon_t* hexa, filtro_gestos_in_imagen_t* image, int k)
{
  int colum,row,alt,anch,up,down,left,right,upi,lefti,downi,righti,a,b,negroU,negroD,negroL,negroR;
  negroU=negroD=negroL=negroR=0;
  if(k==0){a=1;b=2;}else if(k==4){a=1;b=4;}else{a=3;b=4;}
  right=id->m_x; down=id->m_y;
  left=si->m_x; up=si->m_y;
  anch= right-left; alt= down-up;
  colum= left+((right-left)*a)/b;
  row= up+((down-up)*a)/b;
  upi=downi=lefti=righti=0;
  while(!negroU && upi<=alt)
     if(!image->m_imagen[(up*image->m_bytes*image->m_ancho)+colum])negroU=1; else{up++; upi++;}
  while(!negroD && downi<=alt)
     if(!image->m_imagen[(down*image->m_bytes*image->m_ancho)+colum])negroD=1; else{down--; downi++;}
  while(!negroL && lefti<=anch)
    if(!image->m_imagen[(row*image->m_bytes*image->m_ancho)+left])negroL=1; else{left++; lefti++;}
  while(!negroR && righti<=anch)
    if(!image->m_imagen[(row*image->m_bytes*image->m_ancho)+right])negroR=1; else{right--; righti++;}
  hexa->m_list[k]= (double)(upi*100)/alt;
  hexa->m_list[k+1]= (double)(downi*100)/alt;
  hexa->m_list[k+2]= (double)(lefti*100)/anch;
  hexa->m_list[k+3]= (double)(righti*100)/anch;
  return 0;
}


int makeHexadecagon(coord_t* si, coord_t* id, hexadecagon_t* hexa, filtro_gestos_in_imagen_t* image)
{
  int up,down,left,right,upi,downi,lefti,righti,anch,alt,negroUL,negroDL,negroUR,negroDR,h;
  makeHexadecagonAux(si,id,hexa,image,0);
  makeHexadecagonAux(si,id,hexa,image,4);
  makeHexadecagonAux(si,id,hexa,image,8);
  double diagonal= modulo(si->m_x,si->m_y,id->m_x,id->m_y);
  negroUL=negroDL=negroUR=negroDR=0;
  up= si->m_y; left= si->m_x;
  down= id->m_y; right= id->m_x;
  anch= right-left; alt= down-up;

  h=1; lefti=upi=0;
  while(!negroUL && upi<=alt && lefti<=anch)
    if(!image->m_imagen[(up*image->m_bytes*image->m_ancho)+left])negroUL=1;
    else{up++;upi++;left+=floor((h*anch)/alt);h++;lefti+=floor((h*anch)/alt);}
  hexa->m_list[12]= (double)(modulo(lefti,upi,0,0)*100)/diagonal;

  up=si->m_y; h=1; righti=upi=0;
  while(!negroUR && upi<=alt && righti<=anch)
    if(!image->m_imagen[(up*image->m_bytes*image->m_ancho)+right])negroUR=1;
    else{up++;upi++;right-=floor((h*anch)/alt);h++;righti+=floor((h*anch)/alt);}
  hexa->m_list[13]= (double)(modulo(righti,upi,0,0)*100)/diagonal;

  left=si->m_x; h=1; lefti=downi=0;
  while(!negroDL && downi<=alt && lefti<=anch)
    if(!image->m_imagen[(down*image->m_bytes*image->m_ancho)+left])negroDL=1;
    else{down--;downi++;left+=floor((h*anch)/alt);h++;lefti+=floor((h*anch)/alt);}
  hexa->m_list[14]= (double)(modulo(lefti,downi,0,0)*100)/diagonal;

  right=id->m_x; down=id->m_y; h=1; righti=downi=0;
  while(!negroDR && downi<=alt && righti<=anch)
    if(!image->m_imagen[(down*image->m_bytes*image->m_ancho)+right])negroDR=1;
    else{down--;downi++;right-=floor((h*anch)/alt);h++;righti+=floor((h*anch)/alt);}
  hexa->m_list[15]= (double)(modulo(righti,downi,0,0)*100)/diagonal;
  return 0;
}


constellation_t* makeConstellation(fields_t* fields,filtro_gestos_in_imagen_t* image)
{
 int i;
 hexadecagon_t* hexadecagon;
 constellation_t* constelacion= (constellation_t*)malloc(sizeof(constellation_t));
 constelacion->m_list= (hexadecagon_t**)malloc(sizeof(hexadecagon_t*)*fields->cimaCI);
 constelacion->m_top=0;
 for(i=0;i<fields->cimaCI;i++){
   hexadecagon= (hexadecagon_t*)malloc(sizeof(hexadecagon_t));
   makeHexadecagon(fields->ptosSI[i],fields->ptosID[i],hexadecagon,image);
   constelacion->m_list[constelacion->m_top]= hexadecagon;
   constelacion->m_top++;
 }
 return constelacion;
}


int freeFields(fields_t* fields)
{
  int i;
  free(fields->espacios);
  for(i=0;i<fields->cimaCD;i++)free(fields->ptosSI[i]);
  free(fields->ptosSI);
  for(i=0;i<fields->cimaCI;i++)free(fields->ptosID[i]);
  free(fields->ptosID);
  free(fields);
  return 0;
}


int numCar(char letra)
{
 int i;
 char* FC= "abcdefghijklmnopqrstuvwxyz1234567890+-*/=%";
 for(i=0; i<LONG_CAR; i++)
   if(FC[i]==letra)return i;
 return 0;
}


double numHits(char* word1, int size, char* word2,int k, constellation_t* dataBase, constellation_t* constellation)
{
 double out=0.0;
 int cont,i;
 cont=0;
 for(i=0; i<size; i++){
   if(!(word1[i]==word2[i] || word1[i]==word2[i]+('a'-'A')))
      out+=match_hexadecagon(dataBase->m_list[numCar(word1[i])],constellation->m_list[k+i]);
   if(word2[i]>='0' && word2[i+1]<='9')cont++;
 }
 if(cont==size)return -1;
 else return out;
}


char* MatchDicc(char* text, int size, int k, pack_init_t* packInit, constellation_t* constellation)
{
 if(size<NUMWORDS+1){
   int num; double cota=MAXFLOAT; int pos=0;
   //Verificar q ya existe (HashTable)
   int i;
   for(i=0; i<packInit->list[size-1]; i++){
     num= numHits(packInit->blocks[size-1][i],size,text,k,packInit->dataBase,constellation);
     if(num<0)return text;
     if(num<cota){cota=num; pos=i; if(num==0)break;}
   }
   return strdup(packInit->blocks[size-1][pos]);
 }
 else return strdup(text);
}


int freeConstellation(constellation_t* constellation)
{
  int i;
  for(i=0;i<constellation->m_top;i++)free(constellation->m_list[i]);
  free(constellation->m_list);
  free(constellation);
  return 0;
}


const char* ocr_semantic_match(filtro_gestos_in_imagen_t* dibujo, pack_init_t* packInit)
{
  char *devolver = 0;
  int size,i;
  char buffer[128];
  char* text;
  size=0;
  if(dibujo && packInit){
    fields_t* fields= limits(dibujo);
    constellation_t* constelacion= makeConstellation(fields,dibujo);
    for(i=0; i<constelacion->m_top; i++){
      buffer[size]= match_constallation(packInit->dataBase,constelacion->m_list[i]);
      if(i<constelacion->m_top-1 && fields->espacios[i]){size++; buffer[size]=' ';}
      size++;
    }
    buffer[size]='\0';
    freeFields(fields);
    text= /*strdup(*/buffer/*)*/;
    if(strcmp(text,"")){
      char total[128]="";
      int cont,k,a,i;
      char word[128]=""; cont=k=a=0;
      for(i=0; i<=size; i++){
        if(text[i]==' '){
          word[cont]='\0';
          strcat(total,strcat(MatchDicc(word,cont,k,packInit,constelacion)," "));
          word[0]='\0'; k=a; cont=0;}
        else if(i==size){
          word[cont]='\0';
          strcat(total,strcat(MatchDicc(word,cont,k,packInit,constelacion)," "));
        }
        else {word[cont]=text[i]; a++; cont++;}
      }
      //free(dibujo->m_imagen);
      //free(dibujo);
      //freeConstellation(constelacion);
      //return strdup(total);
      devolver = strdup(total);
    }
    free(dibujo->m_imagen);
    free(dibujo);
    freeConstellation(constelacion);
  }
  /*  free(dibujo->m_imagen);
  free(dibujo);
  freeConstellation(constelacion);*/
  return devolver;
}


int ocr_save(const char* ruta, pack_init_t* packInit)
{
  char id;
  int top,i,j,vali,k;
  double val;
  char* word;
  FILE* archivo = fopen(ruta, "wb");
  top= packInit->dataBase->m_top;
  fwrite(&top, sizeof(int), 1, archivo);
  for(i=0; i<top; i++){
    id= packInit->dataBase->m_list[i]->m_id;
    fwrite(&id, sizeof(char), 1, archivo);
    for(j=0; j<NUMPTOS; j++){
      val= packInit->dataBase->m_list[i]->m_list[j];
      fwrite(&val, sizeof(double), 1, archivo);
    }
  }
  for(i=0; i<NUMWORDS; i++){
    vali= packInit->list[i];
    fwrite(&vali, sizeof(int), 1, archivo);
  }
  for(i=0; i<NUMWORDS; i++){
     for(j=0; j<packInit->list[i]; j++){
        word= packInit->blocks[i][j];
        for(k=0;k<i+1;k++){
          fwrite(&word[k], sizeof(char), 1, archivo);
        }
     }
  }
  fclose(archivo);
  return 0;
}


pack_init_t* ocr_init(const char* ruta)
{
  char c;
  double d;
  int num,ent,i,j,k;
  char buffer[128];
  pack_init_t* out= (pack_init_t*)malloc(sizeof(pack_init_t));
  hexadecagon_t* hexadecagon;
  FILE* archivo = fopen(ruta, "rb");
  fread(&num, sizeof(int), 1, archivo);
  out->dataBase= (constellation_t*)malloc(sizeof(constellation_t));
  out->dataBase->m_list= (hexadecagon_t**)malloc(sizeof(hexadecagon_t*)*num);
  out->dataBase->m_top=0;
  out->list= (int*)malloc(sizeof(int)*NUMWORDS);
  out->blocks= (char***)malloc(sizeof(char**)*NUMWORDS);
  for(i=0; i<num; i++){
     hexadecagon= (hexadecagon_t*)malloc(sizeof(hexadecagon_t));
     fread(&c, sizeof(char), 1, archivo);
     hexadecagon->m_id=c;
     for(j=0; j<NUMPTOS; j++){
       fread(&d, sizeof(double), 1, archivo);
       hexadecagon->m_list[j]= (float)d;
     }
     out->dataBase->m_list[out->dataBase->m_top]= hexadecagon;
     out->dataBase->m_top++;
  }
  for(i=0; i<NUMWORDS; i++){
    fread(&ent, sizeof(int), 1, archivo);
    out->list[i]= ent;
    out->blocks[i]= (char**)malloc(sizeof(char*)*ent);
  }
  for(i=0; i<NUMWORDS; i++){
     for(j=0; j<out->list[i]; j++){
        for(k=0; k<i+1; k++){
           fread(&c,sizeof(char),1,archivo);
           buffer[k]=c;
        }
        buffer[i+1]='\0';
        out->blocks[i][j]= strdup(buffer);
     }
  }
  fclose(archivo);
  return out;
}


int ocr_free_pack_init(pack_init_t* packInit)
{
  int i,j;
  for(i=0; i<NUMWORDS; i++){
    for(j=0;j<packInit->list[i];j++)free(packInit->blocks[i][j]);
    free(packInit->blocks[i]);
  }
  free(packInit->blocks);
  free(packInit->list);
  freeConstellation(packInit->dataBase);
  free(packInit);
  return 0;
}




 
