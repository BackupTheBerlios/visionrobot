//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "CListaImagenes.h"

//---------------------------------------------------------------------------


ListaImagenes::ListaImagenes()
{
 numImagenes=0;
 lista= new Imagen*[tamLista];
}


//---------------------------------------------------------------------------


ListaImagenes::~ListaImagenes()
{
 delete lista;
}


//---------------------------------------------------------------------------


bool ListaImagenes::InsertarImagen(Imagen* imagen)
{
 if(numImagenes<tamLista){
   lista[numImagenes]=imagen;
   numImagenes++;
   return true;
   }
 else return false;
}


//---------------------------------------------------------------------------


int ListaImagenes::GetNumImagenes(){return numImagenes;}

Imagen* ListaImagenes::GetImagen(int pos){return lista[pos];}


//---------------------------------------------------------------------------


void ListaImagenes::CargarLista(AnsiString fileName)
{
 FILE *archivo;
 char f1[60], im[60];
 Imagen* imagen;
 archivo= fopen(fileName.c_str(),"r");

  while(fscanf(archivo,"%s",im)!=EOF){
    imagen= new Imagen();
    imagen->Leer(im);
    InsertarImagen(imagen);
  }
  fclose(archivo);
}


//---------------------------------------------------------------------------
