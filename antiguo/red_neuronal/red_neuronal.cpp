/*! \file red_neuronal_vc.h
    \brief CPP de la DLL de la red neuronal
    \author Diego
    \version 0.2
*/

#include "dll_pipeline.h"
#include "Principal.h"
#include "propiedades_red.h"
#include "Codigo_Filtro.h"


data_in* buffer_in;
char* buffer_out;
Principal* principal;
BYTE* buffer;
bool entrenada;
int ancho,alto;
tipoFoto tipofoto=ORDEN;
red_t opciones;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//Incluir tipoFoto y data_in por punto h
//cambiar el data_in tiene q incluir ancho y alto

int ciclo (){
       if(entrenada) buffer_out= principal->Reconocer(buffer,ancho,alto,tipofoto);
       else buffer_out= "No_Gesto";
       return 0;
}
int set_datos(const void * datos){
       buffer_in= (data_in*)datos;
       buffer_out= NULL;
       return 0;
}
void * get_datos(){
	return buffer_out;
}
int iniciar() {
      opciones.ruta = "";
      opciones.tipo = ORDEN;
      principal= new Principal();
      
      if(abrir_propiedades_red(&opciones))
      {
                                          
        /* T� sabr�s qu� hay que poner aqu�, que a m� no me compila
           y no lo entiendo        
        principal->AbrirRed(opciones.ruta);
        entrenada=true;
        tipofoto= opciones.tipo;
        if(tipofoto==ORDEN)buffer= buffer_in->tipo_orden;
        else buffer= buffer_in->orden;
        ancho= buffer_in->ancho;
        alto= buffer_in->alto;*/
      }
      else entrenada=false;
      return 0;
}
int propiedades(){
  // Ventana para cargar otra red y otro tipo de gesto
}
int cerrar(){
      delete principal;
      return 0;
}


