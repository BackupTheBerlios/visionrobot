/*! \file filtro_gestos_vc.h
    \brief CPP de la DLL del filtro de gestos
    \author Diego y Carlos
    \version 0.1
*/

#include "dll_pipeline.h"
#include "Codigo_Filtro.h"
#include "preferencias.h"

entrada* buffer_in;
salida*  buffer_out;
respuesta_t opciones;

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
	buffer_out= Filtro(buffer_in->m_imagen, buffer_in->m_alto ,buffer_in->m_ancho);
	return 0;
}
int set_datos(const void * datos){
	buffer_in= (entrada*)datos;
    buffer_out= NULL;
	return 0;
}
void * get_datos(){
	return buffer_out;
}
int iniciar() {
	return 0;
}
int propiedades(){
  // Carlos -->
  if(abrir_preferencias(&opciones)) {
    // A�adir el c�digo que hay que realizar con esas opciones
  }
  // <--
}
int cerrar(){
	return 0;
}
