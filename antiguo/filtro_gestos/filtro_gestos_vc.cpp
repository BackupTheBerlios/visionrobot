/*! \file filtro_gestos_vc.h
    \brief CPP de la DLL del filtro de gestos
    \author Diego y Carlos
    \version 0.1
*/

#include "dll_pipeline.h"
#include "Codigo_Filtro.h"
#include "preferencias.h"

data_in* buffer_in;
data_out*  buffer_out;
Filtro* filtro;

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


int ciclo (){
	buffer_out= filtro->Filtrar(buffer_in->m_imagen, buffer_in->m_alto ,buffer_in->m_ancho);
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
      filtro= new Filtro();
	return 0;
}
int propiedades(){
  // Carlos -->
  if(abrir_preferencias(&opciones)) {
     filtro->SetColor(240,160,120,70,120,70,100,50,100,50,100,50);
    // Temporalmente
  }
  // <--
}
int cerrar(){
      delete filtro;
	return 0;
}
