/*! \file filtro_gestos_vc.h
    \brief CPP de la DLL del filtro de gestos
    \author Diego y Carlos
    \version 0.1
*/

#ifdef WIN32
#include "dll_pipeline.h"
#endif
#include "Codigo_Filtro.h"
#include "preferencias.h"
#include <stdio.h>

data_in* buffer_in;
data_out*  buffer_out;
Filtro* filtro;
respuesta_t opciones;

#define PON_ERROR(x) sprintf(cadena_error, (x));hay_error = 1;

char cadena_error[64];
char hay_error = 0;

#ifdef WIN32
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

#endif

int ciclo (){
	//buffer_out= filtro->Filtrar(buffer_in->m_imagen, buffer_in->m_alto ,buffer_in->m_ancho);
	return 0;
}
int set_datos(const void * datos){
	buffer_in= (data_in*)datos;
	if(buffer_in) {
 	  sprintf(cadena_error, "Filtro de gestos recibe imagen de %i x %i.\n", buffer_in->m_ancho, buffer_in->m_alto);
	  hay_error = 1;
    buffer_out= NULL;
 }
	return 0;
}
void * get_datos(){
	return buffer_out;
}
int iniciar() {
    PON_ERROR("Iniciando filtro de gestos...\n");
      filtro= new Filtro();
	return 0;
}
int propiedades(){
    PON_ERROR("Abriendo propiedades...\n");
  // Carlos -->
  if(abrir_preferencias(&opciones)) {
     filtro->SetColor(240,160,120,70,120,70,100,50,100,50,100,50);
    // Temporalmente
  }
  // <--
}
int cerrar(){
    PON_ERROR("Cerrando filtro de gestos...\n");
      delete filtro;
	return 0;
}
char * error() {	
	char * dev = hay_error ? cadena_error : 0;
	hay_error = 0;
	return dev;
 }
