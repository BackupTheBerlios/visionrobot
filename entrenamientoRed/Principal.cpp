//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop
#pragma package(smart_init)
#include "Principal.h"

//---------------------------------------------------------------------------


Principal::Principal(){}


//---------------------------------------------------------------------------


Principal::Principal(AnsiString nE, AnsiString nV, AnsiString nP,
                          int v, tipoFoto t, TMemo* mem, TProgressBar* Prog, double tasa)
{
 ListaImagenes* listaEntrenamiento= new ListaImagenes();
 ListaImagenes* listaValidacion= new ListaImagenes();   //No haran falta una vez entrenada
 ListaImagenes* listaPrueba= new ListaImagenes();

 listaEntrenamiento->CargarLista(nE);
 listaValidacion->CargarLista(nV);
 listaPrueba->CargarLista(nP);

 mem->Lines->Clear();
 mem->Lines->Add("Numero de imagenes de entrenamiento:  "+IntToStr(listaEntrenamiento->GetNumImagenes()));
 mem->Lines->Add("Numero de imagenes de validacion:     "+IntToStr(listaValidacion->GetNumImagenes()));
 mem->Lines->Add("Numero de imagenes de prueba:         "+IntToStr(listaPrueba->GetNumImagenes())+'\n');

 randomize();
 Prog->Visible=true;
 ReconocimientoImagenes(listaEntrenamiento, listaValidacion, listaPrueba, v, t, mem, Prog, tasa);
}


//---------------------------------------------------------------------------


void Principal::ReconocimientoImagenes(ListaImagenes* listaEntrena, ListaImagenes* listaValid, ListaImagenes* listaPrueba,
                            int vueltas, tipoFoto tipo, TMemo* mem, TProgressBar* Prog, double tasa)
{
  double out_err, hid_err, sumerr;
  int out=5; int hid=15;
  Imagen* imagen = listaEntrena->GetImagen(0);
  red = new RedNeuronal(imagen->GetDibujo()->Bitmap->Height * imagen->GetDibujo()->Bitmap->Width, hid, out);

  elitePI= new double[((imagen->GetDibujo()->Bitmap->Height * imagen->GetDibujo()->Bitmap->Width)+1)*(hid+1)];
  elitePH= new double[(hid+1)*(out+1)];
  correcto=0;


  // Entrenarla
  for(int vuelta=1;vuelta<=vueltas; vuelta++){
    mem->Lines->Add("Numero de vuelta:  "+IntToStr(vuelta));
    sumerr= 0;
    for(int i=0;i<listaEntrena->GetNumImagenes();i++){
      //Actualizar las unidades de entrada en la red con la imagen i
      CargaInputImagen(listaEntrena->GetImagen(i), red);
      //Actualizar el vector objetivo para la imagen i
      Carga_objetivo(listaEntrena->GetImagen(i), red, tipo);
      //Entrena la redNeuronal
      red->Entrenamiento(tasa, &out_err, &hid_err);
      sumerr+=(out_err+hid_err);
    }

    Prog->Position=(ceil((vuelta*100)/vueltas));
    //Ver como ha mejorado la red sobre el entrenamiento
    mem->Lines->Add("Entrenamiento:");
    CalculaListaImagenes(listaEntrena, tipo, mem, 1);
    mem->Lines->Add('\t');
  }

  //Cargar el mejor peso obtenido. Elitismo
  red->SetPesosEntrada(elitePI);
  red->SetPesosOculta(elitePH);
  //Mostrar los valores de la mejor red, las de los mejores pesos
  mem->Lines->Add("Mejores pesos obtenidos en las vuelta dadas:");
  mem->Lines->Add("Entrenamiento:");
  CalculaListaImagenes(listaEntrena, tipo, mem, 0);
  mem->Lines->Add("Validacion:");
  CalculaListaImagenes(listaValid, tipo, mem, 0);
  mem->Lines->Add("Prueba:");
  CalculaListaImagenes(listaPrueba, tipo, mem, 0);
  mem->Lines->Add('\t');

  red->SetCorrecto((correcto*100)/listaEntrena->GetNumImagenes());
  Prog->Visible=false;
}


//---------------------------------------------------------------------------


void Principal::GuardarRed(AnsiString fileName)
{
  red->Guardar(fileName);
}


//---------------------------------------------------------------------------


void Principal::AbrirRed(AnsiString fileName)
{
  red= Abrir(fileName);
}


//---------------------------------------------------------------------------


AnsiString Principal::Reconocer(AnsiString filename, tipoFoto tipo){
 double val;
 Imagen* imagen= new Imagen();
 imagen->Leer(filename);
 Sleep(100);
 CargaInputImagen(imagen, red);
 red->ComputarCapas();

  switch(tipo){
        case ORDEN:
                if(red->GetCapaSalida()[1]>0.5) return "Parar";
                else if(red->GetCapaSalida()[2]>0.5) return "Girar";
                else if(red->GetCapaSalida()[3]>0.5) return "GirarNeg";
                else if(red->GetCapaSalida()[4]>0.5) return "Avanzar";
                else if(red->GetCapaSalida()[5]>0.5) return "No_Gesto";
                else return "";
        break;
        case PARAM:
                if(red->GetCapaSalida()[1]>0.5) return "Nula";
                else if(red->GetCapaSalida()[2]>0.5) return "Baja";
                else if(red->GetCapaSalida()[3]>0.5) return "Media";
                else if(red->GetCapaSalida()[4]>0.5) return "Alta";
                else if(red->GetCapaSalida()[5]>0.5) return "Maxima";
                else return "No reconocido";
        break;
  }
  return "";
}


//---------------------------------------------------------------------------



/*  Calcula el rendimiento de la red sobre las imagenes d la lista de imagenes
    Escribe el porcentaje correcto de la imagen y el error medio entre el objetivo y el conjunto
    de unidades de salida. */
void Principal::CalculaListaImagenes(ListaImagenes* lista,
                                     tipoFoto tipo, TMemo* mem, int entre)
{
  double val;

  double err = 0;
  int correct = 0;
  int numImagen = lista->GetNumImagenes();
  if(numImagen>0){
    for(int i=0;i<numImagen;i++){
      // Carga de la imagen en la capa de enrtada de la red
      CargaInputImagen(lista->GetImagen(i), red);
      // Calcula la red con esta entrada
      red->ComputarCapas();
      // Escribe el vector objetivo para esta imagen
      Carga_objetivo(lista->GetImagen(i), red, tipo);
      //Mira si es correcta
      if(EvaluacionImagen(&val)) correct++;
      err += val;
    }
    err= err/(double)numImagen;

    //Lo considera el mejor pesado
    if(correct>correcto && entre==1){
      elitePI= red->GetPesosEntrada();
      elitePH= red->GetPesosOculta();
      correcto=correct;
    }

    //Muestra por pantalla el error y el porcentaje bien reconocido hasta el momento
    double errorMedio= err;
    mem->Lines->Add("Error Medio:  "+FloatToStr(errorMedio));
    double porcentajeCorrecto= ((correct*100)/numImagen);
    mem->Lines->Add("Porcentaje de imagenes bien reconocidas:  "+FloatToStr(porcentajeCorrecto));
  }
}


//---------------------------------------------------------------------------


bool Principal::EvaluacionImagen(double * err)
{
  double* target=red->GetObjetivo();
  double* capaSalida= red->GetCapaSalida();
  double sumDelta=0;
  for(int i=1;i<=red->GetNumSalida();i++){
     double delta = target[i] - capaSalida[i];
     sumDelta+=delta;
  }

  *err= (0.5*sumDelta*sumDelta); 

  bool si=true;
  for(int i=1; i<=red->GetNumSalida();i++){
     if (target[i] > 0.5 && capaSalida[i]<0.5)si=false;
     else if(target[i]<0.5 && capaSalida[i]>0.5)si=false;
  }
  return si;
}

//---------------------------------------------------------------------------
