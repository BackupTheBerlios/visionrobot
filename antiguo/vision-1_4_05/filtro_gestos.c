/*!
  \file   filtro_gestos.c
  \author Diego S�nchez, Carlos Le�n
  \version 1.0
  
  \brief  M�dulo de filtrado gen�rico de imagenes.
  
  
*/
#include "pipeline_sdk.h"
#include "filtro_gestos_sdk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>
#include <math.h>

#define PUERTO_IMAGEN "entrada_imagen"
#define PUERTO_PARAMETROS "entrada_parametros"
#define PUERTO_SALIDA "salida_imagen"

typedef struct {
  char m_buffer_error[128];  
  char m_iniciado;
  const char *m_filtrar;
  lua_State *m_lua;
  filtro_gestos_in_imagen_t *m_captura;
  filtro_gestos_in_imagen_t m_salida;
  filtro_gestos_in_parametros_t m_parametros_filtro;
} dato_filtro_t;

#define min(x, y) (x) < (y) ? (x) : (y)
#define max(x, y) (x) > (y) ? (x) : (y)

/*
typedef struct {
        color_t ru;
        color_t rd;
        color_t gu;
        color_t gd;
        color_t bu;
        color_t bd;
} special_colour_t;
*/

typedef filtro_gestos_in_parametros_t special_colour_t;

typedef struct {
        int x;
        int y;
} coord_t;

typedef struct {
        coord_t ii;
        coord_t si;
        coord_t id;
        coord_t sd;
        int grados;
}bounds_t;

/*
typedef struct{
    color_t* dibujo;
    int ancho;
    int alto;
}dato_t;
*/
typedef filtro_gestos_in_imagen_t dato_t;


//********************************************************************************************
//Nuevas funciones para el filtrado de carteles. (Diego)
//********************************************************************************************

//Para fijar el color especial de los carteles
/*void SetSpecialColour(special_colour_t* sc, unsigned char ru, unsigned char rd,
        unsigned char gu, unsigned char gd, unsigned char bu, unsigned char bd)
{
  sc->m_rojo_sup = ru;
  sc->m_rojo_inf = rd;
  sc->m_verde_sup = gu;
  sc->m_verde_inf = gd;
  sc->m_azul_sup = bu;
  sc->m_azul_inf = bd;
}
*/
int inRange(unsigned char valor, unsigned char sup, unsigned char inf)
{
  return (valor<=sup && valor>=inf);
}

int isColour(dato_t* in, int h, int w, special_colour_t* sc)
{
  int pos= (h*in->m_ancho*in->m_bytes)+(w*in->m_bytes);
  return (inRange(in->m_imagen[pos], sc->m_azul_sup, sc->m_azul_inf)&&
          inRange(in->m_imagen[pos+1], sc->m_verde_sup, sc->m_verde_inf) &&
          inRange(in->m_imagen[pos+2], sc->m_rojo_sup, sc->m_rojo_inf));
}


//De momento no exactamente el centrar q ya existe
//void Centrar(dato_t* in, special_colour_t* sc)
static int filtro_gestos_centrar2(lua_State *L)
{
  dato_t *in = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  special_colour_t *sc = (special_colour_t *)lua_touserdata(L, 2);
  int i,j,cont,/*centroX,centroY,*/difY,difX;
  float acX,acY;
  acX=acY=cont=0;
  for (i=0;i<in->m_alto;i++)
     for (j=0;j<in->m_ancho;j++)
       if(isColour(in,i,j,sc)){acX+=j; acY+=i; cont++;}

  acX/=cont; acY/=cont;
  difY= floor(in->m_alto/2 - acY);
  difX= floor(in->m_ancho/2 - acX);

  if(difY<0){
    for (i=0;i-difY<in->m_alto;i++)
      for(j=0;j<in->m_ancho*in->m_bytes;j++)
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+j]=in->m_imagen[((i-difY)*in->m_ancho*in->m_bytes)+j];
    while(i<in->m_alto){
      for (j=0;j<in->m_ancho*in->m_bytes;j++)in->m_imagen[(i*in->m_ancho*in->m_bytes)+j]=BLANCO;
      i++;
    }
  }

  if(difY>0){
    for(i=in->m_alto-1;i-difY>=0;i--)
      for (j=0;j<in->m_ancho*in->m_bytes;j++)in->m_imagen[(i*in->m_ancho*in->m_bytes)+j]=in->m_imagen[((i-difY)*in->m_ancho*in->m_bytes)+j];
    while(i>=0){
      for(j=0;j<in->m_ancho*in->m_bytes;j++)in->m_imagen[(i*in->m_ancho*in->m_bytes)+j]=BLANCO;
      i--;
    }
  }

  if(difX<0){
    for (i=0;i<in->m_alto;i++){
      for (j=0;j-(difX*in->m_bytes)+in->m_bytes<in->m_ancho*in->m_bytes;j+=in->m_bytes){
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes))];
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j+1)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes)+1)];
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j+2)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes)+2)];
      }
      while(j+in->m_bytes<in->m_ancho*in->m_bytes){
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j)]=BLANCO; in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j+1)]=BLANCO; in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j+2)]=BLANCO; j+=in->m_bytes;
      }
    }
  }

  if(difX>0){
    for (i = 0; i< in->m_alto-1; i++){
      for (j=(in->m_ancho*in->m_bytes)-1;j-(difX*in->m_bytes)+2>=0;j-=in->m_bytes){
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes))];
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-1)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes)-1)];
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-2)]= in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-(difX*in->m_bytes)-2)];
      }
      while(j>=in->m_bytes){
        in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j)]=BLANCO; in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-1)]=BLANCO; in->m_imagen[(i*in->m_ancho*in->m_bytes)+(j-2)]=BLANCO; j-=in->m_bytes;
      }
    }
  }
  return 0;
}

coord_t Corner1(dato_t* in, special_colour_t* sc)
{
 coord_t salida;
 int i,j;
 for(i=0; i<in->m_alto; i++)
   for(j=0; j<in->m_ancho; j++)
     if(isColour(in,i,j,sc)){salida.x=j; salida.y=i; return salida;}
 return salida;
}

coord_t Corner2(dato_t* in, special_colour_t* sc)
{
 coord_t salida;
 int i,j;
 for(i=in->m_alto-1; i>=0; i--)
   for(j=0; j<in->m_ancho; j++)
     if(isColour(in,i,j,sc)){salida.x=j; salida.y=i; return salida;}
 return salida;
}

coord_t Corner3(dato_t* in, special_colour_t* sc)
{
 coord_t salida;
 int i,j;
 for(j=0; j<in->m_ancho; j++)
   for(i=0; i<in->m_alto-1; i++)
     if(isColour(in,i,j,sc)){salida.x=j; salida.y=i; return salida;}
 return salida;
}

coord_t Corner4(dato_t* in, special_colour_t* sc)
{
 coord_t salida;
 int i,j;
 for(j=in->m_ancho; j>=0; j--)
   for(i=0; i<in->m_alto-1; i++)
     if(isColour(in,i,j,sc)){salida.x=j; salida.y=i; return salida;}
 return salida;
}

void Identify(dato_t* in, bounds_t* bounds, coord_t c)
{
 if(c.x<in->m_ancho/2 && c.y>in->m_alto/2)bounds->ii=c;
 else if(c.x>in->m_ancho/2 && c.y>in->m_alto/2)bounds->id=c;
 else if(c.x<in->m_ancho/2 && c.y<in->m_alto/2)bounds->si=c;
 else if(c.x>in->m_ancho/2 && c.y<in->m_alto/2)bounds->sd=c;
}

void Identify2(bounds_t* bounds, coord_t c1, coord_t c2, coord_t c3, coord_t c4)
{
 int minY= min(min(c1.y,c2.y),min(c3.y,c4.y));
 int minX= min(min(c1.x,c2.x),min(c3.x,c4.x));
 int maxY= max(max(c1.y,c2.y),max(c3.y,c4.y));
 int maxX= max(max(c1.x,c2.x),max(c3.x,c4.x));
 bounds->si.y= minY; bounds->si.x= minX;
 bounds->sd.y= minY; bounds->sd.x= maxX;
 bounds->ii.y= maxY; bounds->ii.x= minX;
 bounds->id.y= maxY; bounds->id.x= maxX;
}

//bounds_t* BuscarLimites(dato_t* in, int op, special_colour_t* sc)
static int filtro_gestos_buscar_limites(lua_State *L)
{
  dato_t *in = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int op = luaL_checkint(L, 2);
  special_colour_t *sc = (special_colour_t *)lua_touserdata(L, 3);
 bounds_t* out = (bounds_t*)malloc(sizeof(bounds_t));
 coord_t c1= Corner1(in,sc);
 coord_t c2= Corner2(in,sc);
 coord_t c3= Corner3(in,sc);
 coord_t c4= Corner4(in,sc);
 if(op){
   Identify(in,out,c1);
   Identify(in,out,c2);
   Identify(in,out,c3);
   Identify(in,out,c4);
 }
 else Identify2(out,c1,c2,c3,c4);
 int b= out->ii.y-out->id.y;
 int a= (out->id.x-out->ii.x);
 double divis= (double)b/a;
 double angulo= atan(divis);
 out->grados= (angulo*180)/M_PI;
 lua_pushlightuserdata(L, out);
 return 1;
}

//dato_t* Rotate(dato_t* in, bounds_t* bounds)
static int filtro_gestos_rotar(lua_State *L)
{
  dato_t *in = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  bounds_t *bounds =  (bounds_t *)lua_touserdata(L, 2);
    dato_t* salida= (dato_t*)malloc(sizeof(dato_t));
    int k,ii,jj,lin,pos,pos2,i,j;
    double fi,alfa,a,b,subAncho1,subAncho2;
    fi= (((double)bounds->grados)*M_PI)/180;
    alfa= M_PI/2 - fi;
    subAncho1= abs(in->m_alto*cos(alfa));
    subAncho2= abs(in->m_ancho*cos(fi));
    salida->m_ancho= subAncho1 + subAncho2;
    if(bounds->grados<0){subAncho1=0; subAncho2=salida->m_ancho;}
    salida->m_alto= abs(in->m_alto*sin(alfa)) + abs(in->m_ancho*sin(fi));
    salida->m_imagen= (color_t *)malloc(sizeof(color_t) * salida->m_alto * 
					salida->m_ancho * salida->m_bytes);
    for(i=0; i<salida->m_alto; i++){
      k=0;
      lin= i*salida->m_ancho*in->m_bytes;
      if(bounds->grados<0)
        {a= (i-abs(in->m_ancho*sin(fi)))*sin(fi);b= (i-abs(in->m_ancho*sin(fi)))*cos(fi);}
      else
        {a=i*sin(fi); b=i*cos(fi);}
      for(j=-subAncho1; j<subAncho2; j++){
         ii= j*cos(fi)+a;
         jj= -j*sin(fi)+b;
         pos= lin+(k*in->m_bytes);
         if(ii<0 || ii>=in->m_ancho || jj<0 || jj>=in->m_alto){
            salida->m_imagen[pos]=NEGRO;
            salida->m_imagen[pos+1]=NEGRO;
            salida->m_imagen[pos+2]=NEGRO;
         }
         else{
            pos2= (jj*in->m_ancho*in->m_bytes)+(ii*in->m_bytes);
            salida->m_imagen[pos]= in->m_imagen[pos2];
            salida->m_imagen[pos+1]= in->m_imagen[pos2+1];
            salida->m_imagen[pos+2]= in->m_imagen[pos2+2];
         }
         k++;
      }
    }
    //    delete in;
    free(in);
    free(bounds);//delete bounds;
    lua_pushlightuserdata(L,salida);
    return 1;
    //    return salida;
}

void CleanLine(dato_t* in, int i, int left, int right, int op)
{
  int j,pos1,pos2;//,r,l;
 pos1=(i*in->m_ancho*in->m_bytes);
 for(j=0; j<in->m_ancho; j++)
 {
  pos2= pos1+(j*in->m_bytes);
  if((op)||(!op && j<left)||(!op && j>=right)){
    in->m_imagen[pos2]=BLANCO; in->m_imagen[pos2+1]=BLANCO; in->m_imagen[pos2+2]=BLANCO;
  }
  else{
    if(in->m_imagen[pos2]<SEMI_NEGRO && in->m_imagen[pos2+1]<SEMI_NEGRO && in->m_imagen[pos2+2]<SEMI_NEGRO){
      in->m_imagen[pos2]=NEGRO; in->m_imagen[pos2+1]=NEGRO; in->m_imagen[pos2+2]=NEGRO;
    }
    else{
      in->m_imagen[pos2]=BLANCO; in->m_imagen[pos2+1]=BLANCO; in->m_imagen[pos2+2]=BLANCO;
    }
  }
 }
}


//void Clean(dato_t* in, bounds_t* bounds)
static int filtro_gestos_clean(lua_State *L)
{
dato_t *in = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
bounds_t *bounds = (bounds_t *)lua_touserdata(L, 2);
 int i;
 for(i=0; i<in->m_alto; i++){
   if(i<min(bounds->si.y,bounds->sd.y) || i>max(bounds->ii.y,bounds->id.y))
        CleanLine(in,i,0,in->m_ancho,1);
        CleanLine(in,i,bounds->ii.x,bounds->id.x,0);
 }
 // delete bounds;
 free(bounds);
 return 0;
}

unsigned char Mask(dato_t* in, int i, int j){
  int k, l;
        int cont=0;
        if(i>TOL && i<in->m_alto-TOL && j>TOL && j<in->m_ancho-TOL){
          for(k=i-TOL; k<i+TOL; k++)
            for(l=j-TOL; l<j+TOL; l++)
              if(in->m_imagen[(k*in->m_ancho*in->m_bytes)+(l*in->m_bytes)]<SEMI_NEGRO &&
                 in->m_imagen[(k*in->m_ancho*in->m_bytes)+(l*in->m_bytes)+1]<SEMI_NEGRO &&
                 in->m_imagen[(k*in->m_ancho*in->m_bytes)+(l*in->m_bytes)+2]<SEMI_NEGRO)cont++;
        }
        if((((2*TOL+1)*(2*TOL+1))/5)<cont)return NEGRO;
        else return BLANCO;
}

//void MakeUp(dato_t* in){
static int filtro_gestos_make_up(lua_State *L) {
  //        unsigned char* salida= new unsigned char[in->m_ancho*in->m_alto*3];
  dato_t *in = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  color_t *salida = (color_t *)malloc(sizeof(color_t) *in->m_ancho*in->m_alto*in->m_bytes);
  int i,j;
  for(i=0;i<in->m_alto;i++) {
    for(j=0;j<in->m_ancho;j++){
      salida[(i*in->m_ancho*in->m_bytes)+(j*in->m_bytes)]=
	salida[(i*in->m_ancho*in->m_bytes)+(j*in->m_bytes)+1]=
	salida[(i*in->m_ancho*in->m_bytes)+(j*in->m_bytes)+2]=Mask(in,i,j);
    }
  }
  free(in->m_imagen);
  //delete in->m_imagen;
  
  in->m_imagen=salida;
  return 0;
}

/*Esta es la funcion que hace todo el filtro y llama a todas las func anteriores, si algo tiene
que ir en scripts seria esto.

1� se centra el cartel
2� se calcula su rotacion
3� se rota
4� se mira q zona ocupa y nos quedamos solo con las letras
5� se rellenan posibles zonas blancas dentro de la letra

Restricciones:
	No sabe leer carteles en perspectiva. Asi que el cartel ha de estar paralelo a la camara.
	La distancia idonea esta entre los 30 y los 50 centimetros.
	La luz es mejor que sea difusa.
*/
/*dato_t* OCR_Filter(dato_t* in, special_colour_t* sc)
{
    Centrar(in,sc);
    bounds_t* bounds= BuscarLimites(in,1,sc);
    in=Rotate(in,bounds);
    bounds= BuscarLimites(in,0,sc);
    Clean(in,bounds);
    MakeUp(in);
    return in;
}
*/
//*********************************************************************************************





/*! \brief Llama a una funci�n de Lua al estilo <code>printf</code>
  
\author Variaci�n de Carlos Le�n del libro "Programming in Lua" de Roberto Ierusalimschy
\param L El lua_State en el que est� la funci�n
\param func El nombre de la funci�n que llamamos
\param sig Una cadena que define los par�metros, seg�n la forma:
           <ul>
	     <li>i: entero.
	     <li>d: coma flotante.
	     <li>s: cadena (char *).
	     <li>p: puntero.
	     <li>b: booleano.
	   </ul>
	   Los par�metros de entrada est�n separados de los de salida por ">".
*/
static void filtro_llamar_funcion (lua_State * L, const char *func, const char *sig, ...) {
  va_list vl;
  int narg, nres;     
  va_start(vl, sig);
  lua_getglobal(L, func);
  narg = 0;
  while (*sig) {
    switch (*sig++) {    
    case 'd': 
      lua_pushnumber(L, va_arg(vl, double));
      break;    
    case 'i': 
      lua_pushnumber(L, va_arg(vl, int));
      break;    
    case 's': 
      lua_pushstring(L, va_arg(vl, char *));
      break;
    case 'p': 
      lua_pushlightuserdata(L, va_arg(vl, void *));
      break;    
    case 'b':
      lua_pushboolean(L, va_arg(vl, int));
      break;    
    case '>':
      goto endwhile;
    }
    narg++;
    luaL_checkstack(L, 1, "too many arguments");
  } endwhile:    
    nres = strlen(sig);
    lua_pcall(L, narg, nres, 0);
    nres = -nres; 
    while (*sig) {
      switch (*sig++) {    
      case 'd':
	*va_arg(vl, double *) = lua_tonumber(L, nres);
	break;    
      case 'i':
	*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
	break;    
      case 's':
	*va_arg(vl, const char **) = lua_tostring(L, nres);
	break;
      case 'p':	
	*va_arg(vl, const void **) = lua_topointer(L, nres);
	break;
      case 'b':	
	*va_arg(vl, int*) = lua_toboolean(L, nres);
	break;
      }
      nres++;
    }
    va_end(vl);
}


static char *filtro_ciclo(modulo_t *modulo, const char *puerto, const void *value)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  char *aux = 0;
  char *devolver = 0;
  GHashTable *tabla = modulo->m_tabla;
  if (value && dato) {
    if(!strcmp(puerto, PUERTO_IMAGEN)) {
      if(1 != dato->m_iniciado) {
	dato->m_iniciado = 1;
	filtro_gestos_in_imagen_t* imagen = (filtro_gestos_in_imagen_t *) value;
	dato->m_captura = imagen;
	color_t *imagen_salida = (color_t *)malloc(sizeof(color_t) *
						    imagen->m_ancho *
						    imagen->m_alto *
						    imagen->m_bytes) ;
	dato->m_salida.m_ancho = imagen->m_ancho;
	dato->m_salida.m_alto = imagen->m_alto;
	dato->m_salida.m_bytes = imagen->m_bytes;
	dato->m_salida.m_imagen = imagen_salida;
      }
      else {
	// El filtro espera que la funcion de filtro le devuelva
	// 0 si todo es negro
	// 1 si no todo es negro
	int color;
	filtro_llamar_funcion(dato->m_lua, dato->m_filtrar, "ppp>bs",
			      dato->m_captura,
			      &dato->m_salida,
			      &dato->m_parametros_filtro,
			      &color, &aux);	
	if(!color) {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, 0);
	}
	else {
	  g_hash_table_insert(tabla, PUERTO_SALIDA, &dato->m_salida);
	}
	if(aux) {
	  strcpy(dato->m_buffer_error, aux);
	  devolver = dato->m_buffer_error;
	}
      }
    }
    else if(!strcmp(puerto, PUERTO_PARAMETROS)) {
      filtro_gestos_in_parametros_t* parametros = (filtro_gestos_in_parametros_t*)value;
      dato->m_parametros_filtro.m_rojo_sup = parametros->m_rojo_sup;
      dato->m_parametros_filtro.m_rojo_inf = parametros->m_rojo_inf;
      dato->m_parametros_filtro.m_verde_sup = parametros->m_verde_sup;
      dato->m_parametros_filtro.m_verde_inf = parametros->m_verde_inf;
      dato->m_parametros_filtro.m_azul_sup = parametros->m_azul_sup;
      dato->m_parametros_filtro.m_azul_inf = parametros->m_azul_inf;
      sprintf(dato->m_buffer_error, "color: [%i-%i], [%i-%i], [%i-%i]",
	      parametros->m_rojo_inf,
	      parametros->m_rojo_sup,
	      parametros->m_verde_inf,
	      parametros->m_verde_sup,
	      parametros->m_azul_inf,
	      parametros->m_azul_sup);
      devolver = dato->m_buffer_error;
    }
  }
  else {
    g_hash_table_insert(tabla, PUERTO_SALIDA, 0);
  }
  return devolver;//dato->m_error;
}

static int filtro_gestos_setarray (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  double value = luaL_checknumber(L, 3);
  if(index >= 1) {
    a->m_imagen[index - 1] = value;
  }
  return 0;
}

static int filtro_gestos_getarray (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  lua_pushnumber(L, a->m_imagen[index - 1]);
  return 1;
}

static int filtro_gestos_getsize (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  lua_pushnumber(L, a->m_ancho * a->m_alto * a->m_bytes);
  return 1;
}

static int filtro_gestos_copiar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  filtro_gestos_in_imagen_t *b = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 2);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;
  int tam = ancho * bytes * alto;
  color_t *aux_a = a->m_imagen;
  color_t *aux_b = b->m_imagen;
  color_t *fin = &b->m_imagen[tam - 1];
  while(aux_b != fin) {
    *aux_b++ = *aux_a++;
  }

  lua_pushboolean(L, 1);
  return 1;
}

static int filtro_gestos_centrar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  int cont = luaL_checkint(L, 2);
  int ac_x = luaL_checkint(L, 3);
  int ac_y = luaL_checkint(L, 4);
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;
  color_t *dibujo = a->m_imagen;
  int tam = bytes * ancho;
  int sum, sum2, sum3, sum4;
  if (cont != 0) { 
    int difY = ((int) floor(alto / 2 - floor(ac_y / cont)));
    int difX = ((int) floor(ancho / 2 - ((int) floor(ac_x / (bytes * cont)))));
    if (difY < 0) {
      int y, x;
      for (y = 0; y - difY < alto; y++) {
	sum = y * tam;
	sum2 = (y - difY) * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = dibujo[sum2 + x];
	}
      }
      while (y < alto) {
	sum = y * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = 0;
	}
	y++;
      }
    }
    else if (difY > 0) {
      int y, x;
      for (y = alto - 1; y - difY >= 0; y--) {
	sum = y * tam;
	sum2 = (y - difY) * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = dibujo[sum2 + x];
	}
      }
      while (y >= 0) {
	sum = y * tam;
	for (x = 0; x < tam; x++) {
	  dibujo[sum + x] = 0;
	}
	y--;
      }
    }
    if (difX < 0) {
      int y;
      for (y = 0; y < alto; y++) {
	int x;	
	sum3 = y * tam;
	x = 0;
	sum2 = x - (difX * bytes);
	for (x = 0; sum2 + bytes - 1 < tam; x+=bytes) {
	  sum = sum3 + x;
	  sum2 = x - (difX * bytes);
	  sum4 = sum3 + sum2;
	  dibujo[sum] = dibujo[sum4];
	  dibujo[sum + 1] = dibujo[sum4 + 1];
	  dibujo[sum + 2] = dibujo[sum4 + 2];
	}
	while (x + 2 < tam) {
	  sum = sum3 + x;
	  dibujo[sum] = 0;
	  dibujo[sum + 1] = 0;
	  dibujo[sum + 2] = 0;
	  x += bytes - 1;
	}
      }
    }
    /* He cambiado esto porque si no viola el segmento.
       Antes:
       if(difX > 0) {
       Carlos
    */
    else if (difX > 1) {
      int y;
      for (y = 0; y < alto - 1; y++) {
	int x;
	sum3 = y * tam;
	x = tam - 2;
	sum2 = x - (difX * bytes);
	for (x = tam - 2; sum2 + bytes - 1 >= 0;
	     x-=bytes) {
	  sum2 = x - (difX * bytes);
	  sum = sum2 + sum3;
	  sum4 = sum3 + x;	  
	  dibujo[sum4] = dibujo[sum];
	  dibujo[sum4 - 1] = dibujo[sum - 1];
	  dibujo[sum4 - 2] = dibujo[sum - 2];
	}
	sum4 = y * tam;
	while (x >= 0) {
	  sum = sum4 + x;
	  dibujo[sum] = 0;
	  dibujo[sum - 1] = 0;
	  dibujo[sum - 2] = 0;
	  x -= bytes - 1;
	}
      }
    }
  }
  return 0;
}

static int filtro_gestos_get_colores(lua_State *L) {
  filtro_gestos_in_parametros_t *a = (filtro_gestos_in_parametros_t *)lua_touserdata(L, 1);
  lua_pushnumber(L, a->m_rojo_inf);
  lua_pushnumber(L, a->m_rojo_sup);
  lua_pushnumber(L, a->m_verde_inf);
  lua_pushnumber(L, a->m_verde_sup);
  lua_pushnumber(L, a->m_azul_inf);
  lua_pushnumber(L, a->m_azul_sup);
  return 6;
}

static int filtro_gestos_difuminar (lua_State *L) {
  filtro_gestos_in_imagen_t *a = (filtro_gestos_in_imagen_t *)lua_touserdata(L, 1);
  filtro_gestos_in_imagen_t *b = (filtro_gestos_in_imagen_t*)lua_touserdata(L, 2);
  int parametro_difuminado = luaL_checkint(L, 3);
  int reduccion = luaL_checkint(L, 4);
  filtro_gestos_in_parametros_t *p = (filtro_gestos_in_parametros_t *)lua_touserdata(L, 5);
  color_t m_rojoInf = p->m_rojo_inf;
  color_t m_rojoSup = p->m_rojo_sup;
  color_t m_verdeInf = p->m_verde_inf;
  color_t m_verdeSup =p->m_verde_sup;
  color_t m_azulInf =p->m_azul_inf;
  color_t m_azulSup =p->m_azul_sup;
  int ancho = a->m_ancho;
  int alto = a->m_alto;
  int bytes = a->m_bytes;  
  color_t *buffer = a->m_imagen;

  color_t *tipo_orden = b->m_imagen;

  int cont, cont2, acX, acX2, acY, acY2, pos;
  int tam = ancho * bytes;
  cont = cont2 = acX = acX2 = acY = acY2 = 0;
  int y, x, j, i;
  for (y = 0; y < alto; y++) {
    for (x = 0; x < tam; x+=bytes) {
      int rojo = 0;
      int verde = 0;
      int azul = 0;

      int param_bytes = parametro_difuminado * bytes;
      int param_bytes2 = (parametro_difuminado * bytes) + (bytes - 1);
      if (y - parametro_difuminado >= 0 && y + parametro_difuminado <= alto) {
	if (x - param_bytes >= 0 && x + param_bytes2 <= tam) {
	  for (i = y - parametro_difuminado; i <= y + parametro_difuminado; i++) {	  
	    for (j = x - param_bytes; j <= x + param_bytes2; j+=bytes) {
	      pos = i * tam + j;
	      rojo += buffer[pos];
	      verde += buffer[pos + 1];
	      azul += buffer[pos + 2];
	    }
	  }
	}
      }
		
      int numero = (2 * parametro_difuminado) + 1;
      numero *= numero;
      rojo = ((int)floor(rojo / numero));
      verde = ((int) floor(verde / numero));
      azul = ((int) floor(azul / numero));

      rojo -= rojo % reduccion;
      verde -= verde % reduccion;
      azul -= azul % reduccion;
      
      pos = y * tam + x;

      /* Atenci�n: los colores rojo y azul est�n cambiados,
	 nosotro usamos el orden RGB */
      color_t azul_comparar = (color_t)rojo;
      color_t rojo_comparar = (color_t)azul;
      color_t verde_comparar = (color_t)verde;
      color_t valor = 0;
      if ((rojo_comparar >= m_rojoInf && rojo_comparar <= m_rojoSup)
	   && (verde_comparar >= m_verdeInf
	       && verde_comparar <= m_verdeSup)
	   && (azul_comparar >= m_azulInf
	       && azul_comparar <= m_azulSup)) {
	valor = 255;
	cont++;
	acX += x;
	acY += y;
      }
      tipo_orden[pos] = valor;
      tipo_orden[pos + 1] = valor;
      tipo_orden[pos + 2] = valor;
    }
  }

  
  lua_pushnumber(L, cont);
  lua_pushnumber(L, acX);
  lua_pushnumber(L, acY);
  return 3;
}


static lua_State *filtro_abrir_lua(modulo_t *modulo, const char *ruta) {
  dato_filtro_t * dato = (dato_filtro_t*)modulo->m_dato;

  static const struct luaL_reg arraylib [] = {
    {"set", filtro_gestos_setarray},
    {"get", filtro_gestos_getarray},
    {"size", filtro_gestos_getsize},
    {"copiar", filtro_gestos_copiar},
    {"difuminar", filtro_gestos_difuminar},
    {"centrar", filtro_gestos_centrar},
    {"rotar", filtro_gestos_rotar},
    {"centrar2", filtro_gestos_centrar2},
    {"buscar_limites", filtro_gestos_buscar_limites},
    {"clean", filtro_gestos_clean},
    {"make_up", filtro_gestos_make_up},
    {NULL, NULL}
  };

  static const struct luaL_reg arrayparam [] = {
    {"get_colores", filtro_gestos_get_colores},
    {NULL, NULL}
  };

  dato->m_lua = lua_open();
  lua_State *l = dato->m_lua;
  luaL_openlib(l, "imagen", arraylib, 0);
  luaL_openlib(l, "parametros", arrayparam, 0);

  luaopen_base(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_string(l);
  luaopen_math(l);
  luaopen_loadlib(l);    

  luaL_loadfile(l, ruta);
  lua_pcall(l, 0, 0, 0);

  return l;
}

static char *filtro_iniciar(modulo_t *modulo, GHashTable *argumentos)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  dato->m_iniciado = -1;
  const char *archivo = g_hash_table_lookup(argumentos, "guion");
  sprintf(dato->m_buffer_error, "iniciado, cargado \"%s\"", archivo);
  dato->m_lua = filtro_abrir_lua(modulo, archivo);
  dato->m_salida.m_imagen = 0;
  dato->m_filtrar = g_hash_table_lookup(argumentos, "filtrar");
  return dato->m_buffer_error;
}
static char *filtro_cerrar(modulo_t *modulo)
{
  dato_filtro_t * dato = (dato_filtro_t *)modulo->m_dato;
  if(dato->m_salida.m_imagen) {
    free(dato->m_salida.m_imagen);
  }
  lua_close(dato->m_lua);
  free(dato);
  free(modulo);
  return "cerrado";
}

modulo_t * get_modulo()
{
  modulo_t *modulo = (modulo_t*)malloc(sizeof(modulo_t));  
  modulo->m_nombre = "Filtro";
  modulo->m_iniciar = filtro_iniciar;
  modulo->m_cerrar = filtro_cerrar;
  modulo->m_ciclo = filtro_ciclo;
  modulo->m_dato = (dato_filtro_t *)malloc(sizeof(dato_filtro_t));
  return modulo;

}



