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
  if(buffer_in && buffer_in->m_imagen != 0) {
	buffer_out= filtro->Filtrar(buffer_in->m_imagen, buffer_in->m_alto ,buffer_in->m_ancho, buffer_in->m_bytes);
	buffer_out->ancho = buffer_in->m_ancho;
  buffer_out->alto = buffer_in->m_alto;	
  }
  PON_ERROR("Ciclo en el modulo de gestos.\n");
  return 0;
}
int set_datos(const void * datos){
  buffer_in= (data_in*)datos;
  if(buffer_in) {
    sprintf(cadena_error, "Filtro de gestos recibe imagen de %i x %i.\n", buffer_in->m_ancho, buffer_in->m_alto);
    hay_error = 1;    
  }
  return 0;
}
void * get_datos(){
  data_out * aux = buffer_out;
  buffer_out= NULL;
  return aux;
}
int iniciar() {
    PON_ERROR("Iniciando filtro de gestos...\n");
      filtro= new Filtro();
	return 0;
}

int color_tolerancia_superior(int color, float tolerancia) {
  return color + ((int)(tolerancia * color));
}

int color_tolerancia_inferior(int color, float tolerancia) {
  return color - ((int)(tolerancia * color));
}


int propiedades(){
    PON_ERROR("Abriendo propiedades...\n");
  // Carlos -->
  if(abrir_preferencias(&opciones)) {
    opciones.orden.color.red;
     //filtro->SetColor(240,160,120,70,120,70,100,50,100,50,100,50);
     filtro->SetColor(
        color_tolerancia_superior(opciones.orden.color.red, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.red, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.orden.color.green, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.green, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.orden.color.blue, opciones.orden.tolerancia),
        color_tolerancia_inferior(opciones.orden.color.blue, opciones.orden.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.red, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.red, opciones.argumento.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.green, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.green, opciones.argumento.tolerancia),
        color_tolerancia_superior(opciones.argumento.color.blue, opciones.argumento.tolerancia),
        color_tolerancia_inferior(opciones.argumento.color.blue, opciones.argumento.tolerancia));
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
